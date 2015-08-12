#include "MonitorServer.h"
#include "WatcherServer.h"
#include <iostream>
#include <QDebug>
using namespace std;

MonitorServer::MonitorServer(QObject *parent,int port)
    :QTcpServer(parent)
{
    m_pWatcherServer = NULL;
    m_nListenPort = port;
}

void MonitorServer::StartListen()
{
    QString strLog = "Monitor server starts listening at port " + QString::number(m_nListenPort) + "." ;
    if(listen(QHostAddress::Any,m_nListenPort))
    {
        LogFile(glbfileLog,strLog);
    }
    else
    {
        strLog = "Monitor server start listening failure.  port : " + QString::number(m_nListenPort);
        LogFile(glbfileLog,strLog);
    }
}

void MonitorServer::incomingConnection(qintptr socketDescriptor)
{
    MonitorClient *tcpClientSocket=new MonitorClient(this);
    connect(tcpClientSocket,SIGNAL(updataClients(stFrameHeader *,char * ,qint32)),this,SLOT(slotReceive(stFrameHeader *,char * ,qint32)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    QHostAddress	 add = tcpClientSocket->localAddress();
    quint16 port = tcpClientSocket->localPort();
    MonitorList.append(tcpClientSocket);

    QString strLog = QString("%1(%2) connect MonitorServer.").arg(tcpClientSocket->peerAddress().toString()).arg(tcpClientSocket->peerPort());
    LogFile(glbfileLog, strLog);
   //timer = new QTimer(this);
 //   connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  // timer->start(1000);
}
void MonitorServer::update()
{
    static int nfre = 100;
    if (nfre == 2000)
        nfre = 100;
    QTcpSocket *item = MonitorList.at(0);
    // char *pdata="abcde";
    //  item->write(pdata,6);
    QString str = QString::number(nfre);
    item->write(str.toStdString().c_str(), str.length());
    cout << "item->write(str.toStdString().c_str(), str.length());" << endl;
    nfre += 100;

}

void MonitorServer::updateClients(QString msg,int length)
{
    emit updateServer(msg,length);
    for(int i=0;i<MonitorList.count();i++)
    {
        QTcpSocket *item = MonitorList.at(i);
        if(item->write(msg.toLatin1(),length)!=length)
        {
            continue;
        }
    }
}

void MonitorServer::slotsTest(QString * str, int * a )
{

}

void MonitorServer::SendMsg(const char *pdata,int length)
{
    for(int i=0;i<MonitorList.count();i++)
    {
        QTcpSocket *item = MonitorList.at(i);
        if(item->write(pdata,length)!=length)
        {
            continue;
        }
    }
}

void MonitorServer::Send2Clients(char *pdata,int nLen)
{
    for(int i=0;i<MonitorList.count();i++)
    {
        QTcpSocket *item = MonitorList.at(i);
        if(item->write(pdata,nLen)!=nLen)
        {
            continue;
        }
    }
}



int  MonitorServer::ConnectedNum()
{
    return MonitorList.count();
}

void MonitorServer::setWatcherServer(QTcpServer *pServer)
{
    m_pWatcherServer = pServer;
}

void MonitorServer::slotDisconnected(int descriptor)
{
    for(int i=0;i<MonitorList.count();i++)
    {
        QTcpSocket *item = MonitorList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
             QString strLog = QString("%1(%2) disconnect MonitorServer.").arg(item->peerAddress().toString()).arg(item->peerPort());
             LogFile(glbfileLog,strLog);
            MonitorList.removeAt(i);
            return;
        }
    }
    return;
}


void MonitorServer::slotReceive(stFrameHeader *header, char * body, qint32 bodyLength)
{

    qint16 cmd = header->cmd;
    QString strLog;
    switch(cmd)
    {
    case CMD_LOAD:                                //  Load *.grc and *.xml of flow graph
    {
        cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;
        if (glbWatcherStatus != Idle)
        {
            strLog = "System is running,LOAD_CMD is rejected.";
            LogFile(glbfileLog,strLog);
        }
        LogFile(glbfileLog,"Receive CMD_LOAD cmd.");
        char FileName[56];
        memcpy(FileName,body,56);
        QString GrcFileName(FileName);
        GrcFileName = glbstrWorkDir + "/" + GrcFileName;
        QFile GrcFile(GrcFileName);
        if (!GrcFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
             strLog = "Can't open" + GrcFileName +"file." ;
            LogFile(glbfileLog,strLog);
            return ;
        }
        qint32 fileLen = *(qint32 *)(body + 56);
        char *pdata = body + 56 + sizeof(qint32);
        if (fileLen != GrcFile.write(pdata,fileLen))
        {
            strLog = "Save GRC file failed" ;
           LogFile(glbfileLog,strLog);
           GrcFile.close();
            return;
        }
        else
        {
            strLog = "Save GRC file successuflly!!!" ;
           LogFile(glbfileLog,strLog);
        }
        GrcFile.close();

        //Save XML file
        pdata = pdata + fileLen;
        memcpy(FileName,pdata,56);
//        QString XMLFileName(FileName);
        XMLFileName = QString(FileName);
        XMLFileName = glbstrWorkDir + "/" + XMLFileName;
        QFile XMLFile(XMLFileName);
        if (!XMLFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
             strLog = "Can't open" + XMLFileName +"file!" ;
            LogFile(glbfileLog,strLog);
            return ;
        }
        fileLen = *(qint32 *)(pdata + 56);
        pdata = pdata + 56 + sizeof(qint32);
        if (fileLen!= XMLFile.write(pdata,fileLen))
        {
            strLog = "Write XML failed" ;
           LogFile(glbfileLog,strLog);
            XMLFile.close();
            return;
        }
        else
        {
            strLog = "Save XML file successuflly!!!" ;
           LogFile(glbfileLog,strLog);
        }
        XMLFile.flush();
        XMLFile.seek(0);
        glboldString = XMLFile.readAll();

        XMLFile.close();
        WatcherServer *pWserver = (WatcherServer *) m_pWatcherServer;
       //  pWserver->Send2Clients(char *pdata,int nLen);
      //   pWserver->Send2Clients(char *pdata,int nLen);
        pWserver->Send2Clients((char *)header,(qint32)sizeof(stFrameHeader));
        pWserver->Send2Clients(body,bodyLength);
        qDebug() << sizeof(stFrameHeader) << bodyLength;
        break;
    }
    case CMD_START:                              //  Running signal processing program in each nodes.
     {
        LogFile(glbfileLog,"Receive START cmd");
                WatcherServer *pWserver = (WatcherServer *) m_pWatcherServer;
        pWserver->Send2Clients((char *)header,sizeof(stFrameHeader));
     //   pWserver->Send2Clients(body,bodyLength);
        break;
    }
    case  CMD_KILL:                                  //  Kill signal processing thread in each nodes.
     {
        LogFile(glbfileLog,"Receive KILL cmd");
                WatcherServer *pWserver = (WatcherServer *) m_pWatcherServer;
        pWserver->Send2Clients((char *)header,sizeof(stFrameHeader));
     //   pWserver->Send2Clients(body,bodyLength);
        break;
    }
    case  CMD_SETPARAM:                      //   Set system param by relaying *.xml
    {
        LogFile(glbfileLog,"Receive SETPARAM cmd");
             WatcherServer *pWserver = (WatcherServer *) m_pWatcherServer;
        pWserver->Send2Clients((char *)header,sizeof(stFrameHeader));
        pWserver->Send2Clients(body,bodyLength);
        break;
    }
    case  CMD_GETSTATUS:                     //   Get system status by relaying *.xml
        break;
    }

    /*
    qint16 cmd = header->cmd;
    switch (cmd) {
    case CMD_LOAD:
    {

        break;
    }
    case CMD_KILL:
    {

    }
    case CMD_START:
    {

     //   pserver->SendMsg();
        break;
    }
    case CMD_STOP:
        break;
    case CMD_SET:
    {
        cout << "Info:Receive CMD_SET cmd!!!" << endl;
        char FileName[56];
        memcpy(FileName,body,56);
        QString XMLFileName(FileName);
        QFile paraFile(XMLFileName);
        QString strlog;
        if (!paraFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            strlog = "Error:Can't open" + XMLFileName +"file!!!" ;
            cout << strlog.toStdString().c_str() << endl;
            return ;        cout << "Info:Receive CMD_SET cmd!!!" << endl;
        char FileName[56];
        memcpy(FileName,body,56);
        QString XMLFileName(FileName);
        QFile paraFile(XMLFileName);
        QString strlog;
        if (!paraFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            strlog = "Error:Can't open" + XMLFileName +"file!!!" ;
            cout << strlog.toStdString().c_str() << endl;
            return ;
        }
        char *pdata = body + 56;
        if ((bodyLength - 56) != paraFile.write(pdata,bodyLength - 56))
        {
            cout <<  "Error:Write param info failed!!!" << endl;
            paraFile.close();
            return;
        }
        else
        {
            cout << "Info:Write para info to para file successuflly!!!" << endl;
        }
       // stFrameHeader  da;
       // da.cmd = CMD_SET;
        //da.DATA_LEN = 0;
        //pserver->SendMsg((char *)&da,sizeof(stFrameHeader));
        //cout << "Info:send Set Param cmd to radio!!!" << endl;
        break;
        }
        char *pdata = body + 56;
        if ((bodyLength - 56) != paraFile.write(pdata,bodyLength - 56))
        {
            cout <<  "Error:Write param info failed!!!" << endl;
            paraFile.close();
            return;
        }
        else
        {
            cout << "Info:Write para info to para file successuflly!!!" << endl;
        }
       // stFrameHeader  da;
       // da.cmd = CMD_SET;
        //da.DATA_LEN = 0;
        //pserver->SendMsg((char *)&da,sizeof(stFrameHeader));
        //cout << "Info:send Set Param cmd to radio!!!" << endl;
        break;
    }
    default:
        break;
    }*/
}



