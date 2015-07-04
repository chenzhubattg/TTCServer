#include "WatcherServer.h"
#include <iostream>
#include <QDebug>
using namespace std;
WatcherServer::WatcherServer(QObject *parent,int port)
    :QTcpServer(parent)
{
    m_nListenPort = port;
    m_pMonServer = NULL;
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
    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
   // connect(tcpClientSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    QHostAddress	 add = tcpClientSocket->localAddress();
    quint16 port = tcpClientSocket->localPort();
    WatcherList.append(tcpClientSocket);
    QString strLog = "IP " + tcpClientSocket->peerAddress().toString() + " connect WatcherServer.";
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

void WatcherServer::SendMsg(const char *pdata,int length)
{
    for(int i=0;i<WatcherList.count();i++)
    {
        QTcpSocket *item = WatcherList.at(i);
        if(item->write(pdata,length)!=length)
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
            WatcherList.removeAt(i);
            return;
        }
    }
    return;
}
