#include "MonitorServer.h"
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
    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(tcpClientSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    QHostAddress	 add = tcpClientSocket->localAddress();
    quint16 port = tcpClientSocket->localPort();
    qDebug() << add.toString() << " port:"<< port;
    MonitorList.append(tcpClientSocket);

    QString strLog = "IP " + tcpClientSocket->peerAddress().toString() + " connect MonitorServer.";
    LogFile(glbfileLog,strLog);


   // timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  //  timer->start(1000);
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
    item->write(str.toStdString().c_str(),str.length());
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
            MonitorList.removeAt(i);
            return;
        }
    }
    return;
}

