#include "WatcherServer.h"
#include <iostream>
#include <QDebug>
using namespace std;
WatcherServer::WatcherServer(QObject *parent,int port)
    :QTcpServer(parent)
{
    m_nListenPort = port;
    m_pMonServer = NULL;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SendStatus()));
}

void WatcherServer::StartListen()
{
    QString strLog = "Watcher server starts listening at port " + QString::number(m_nListenPort) + "." ;
    if(listen(QHostAddress::Any,m_nListenPort))
    {
        LogFile(glbfileLog,strLog);
    }
    else
    {
        strLog = "Watcher server start listening failure.  port : " + QString::number(m_nListenPort);
        LogFile(glbfileLog,strLog);
    }
}

void WatcherServer::incomingConnection(qintptr socketDescriptor)
{
    WatcherClient *tcpClientSocket=new WatcherClient(this);
//    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(updateClients( stFrameHeader *,char * ,qint32, QString )),this,SLOT(slotReceive(stFrameHeader *,char * ,qint32, QString)));

    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    QHostAddress	 add = tcpClientSocket->localAddress();
    quint16 port = tcpClientSocket->localPort();
    WatcherList.append(tcpClientSocket);
    QString strLog = QString("%1(%2) connect WatcherServer.").arg(tcpClientSocket->peerAddress().toString()).arg(tcpClientSocket->peerPort());
    LogFile(glbfileLog,strLog);
}

void WatcherServer::update()
{
    static int nfre = 100;
    if (nfre == 2000)
        nfre = 100;
    QTcpSocket *item = tcpClientSocketList.at(0);
    // char *pdata="abcde";
    //  item->write(pdata,6);
    QString str = QString::number(nfre);
    item->write(str.toStdString().c_str(),str.length());
    nfre += 100;

}

void WatcherServer::SendStatus()
{
    stFrameHeader header;
    header.cmd = CMD_GETSTATUS;
    header.DATA_LEN = glboldString.length();
    MonitorServer *pMserver = (MonitorServer *) m_pMonServer;
    pMserver->Send2Clients((char *)&header,sizeof(stFrameHeader));
    pMserver->Send2Clients((char*)glboldString.toStdString().c_str(), glboldString.length());
}

void WatcherServer::setMonitorServer(QTcpServer *pServer)
{
    m_pMonServer = pServer;
}

void WatcherServer::updateClients(QString msg,int length)
{
    emit updateServer(msg,length);
    for(int i=0;i<WatcherList.count();i++)
    {
        QTcpSocket *item = WatcherList.at(i);
        if(item->write(msg.toLatin1(),length)!=length)
        {
            continue;
        }
    }
}

void WatcherServer::Send2Clients(char *pdata,qint32 nLen)
{
    for(int i=0;i<WatcherList.count();i++)
    {
        QTcpSocket *item = WatcherList.at(i);
        qint64 nTemp = item->write(pdata,nLen);
        cout << "nTemp = " << nTemp << endl;
        if(nTemp != nLen)
        {
            continue;
        }
    }
}

void WatcherServer::SendMsg(const char *pdata,int length)
{
    for(int i=0;i<WatcherList.count();i++)
    {
        QTcpSocket *item = WatcherList.at(i);
        qint64 nTemp = item->write(pdata,length);
        cout << "nTemp = " << nTemp << endl;
        if(nTemp !=length)
        {
            continue;
        }
    }
}

int  WatcherServer::ConnectedNum()
{
    return tcpClientSocketList.count();
}

void WatcherServer::slotDisconnected(int descriptor)
{
    for(int i=0;i<WatcherList.count();i++)
    {
        QTcpSocket *item = WatcherList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            QString strLog = QString("%1(%2) disconnect WatcherServer.").arg(item->peerAddress().toString()).arg(item->peerPort());
            LogFile(glbfileLog,strLog);
            WatcherList.removeAt(i);
            return;
        }
    }
    return;
}

void WatcherServer::slotReceive(stFrameHeader *header, char * body, qint32 bodyLength, QString IpAddress)
{
    qint16 cmd = header->cmd;
    QString strLog;
    switch(cmd)
    {
        case CMD_LOAD:                                //  Load *.grc and *.xml of flow graph

        case CMD_START:                              //  Running signal processing program in each nodes.
            LogFile(glbfileLog,"Receive START cmd");
            break;
        case  CMD_KILL:                                  //  Kill signal processing thread in each nodes.
            LogFile(glbfileLog,"Receive KILL cmd");
            break;
        case  CMD_SETPARAM:                      //   Set system param by relaying *.xml
            LogFile(glbfileLog,"Receive SETPARAM cmd");
            break;
        case  CMD_GETSTATUS:                     //   Get system status by relaying *.xml
        {
            LogFile(glbfileLog,"Receive CMD_GETSTATUS cmd.");
            char FileName[56];
            memcpy(FileName,body,56);

            qint32 fileLen = *(qint32 *)(body + 56);
            char *pdata = body + 56 + sizeof(qint32);

//            QFile oldfile(XMLFileName);
//            if(!oldfile.open(QFile::ReadOnly | QFile::Text))
//                return;
//            QString oldString = oldfile.readAll();
//            oldfile.close();
            QStringList newFileList = QString(pdata).split("<block>");
            QStringList oldFileList = glboldString.split("<block>");
             if (newFileList.length() != oldFileList.length())
             {
                 return;
             }
            glboldString = UpdateXml(QString(pdata), glboldString, IpAddress);
            //MonitorServer *pMserver = (MonitorServer *) m_pMonServer;
            //pMserver->Send2Clients((char *)header,sizeof(stFrameHeader));
            //pMserver->Send2Clients((char*)glboldString.toStdString().c_str(), glboldString.length());
            break;
        }
    }
}

QString WatcherServer::UpdateXml(QString newBlock, QString oldBlock, QString Ip)
{
    QStringList newFileList = newBlock.split("<block>");
    QStringList oldFileList = oldBlock.split("<block>");
    if (newFileList.length() != oldFileList.length())
    {
           LogFile(glbfileLog,"Status data is wrong.");
           return oldBlock;
    }

    QString finishString = oldFileList.at(0);

    for(int i=1; i<oldFileList.size(); i++)
    {
            if( newFileList.at(i).contains(Ip) && oldFileList.at(i).contains(Ip) )
            {
                finishString = finishString + "<block>" + newFileList.at(i);
            }
            else
            {
                finishString = finishString + "<block>" + oldFileList.at(i);
            }
    }
    return finishString;
}
