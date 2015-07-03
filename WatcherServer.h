#ifndef WATCHERSERVER_H
#define WATCHERSERVER_H

#include <QTcpServer>
#include <QObject>
#include"WatcherClient.h"
#include"MonitorServer.h"
#include <QTimer>
#include <QList>



class WatcherServer : public QTcpServer
{
    Q_OBJECT
public:
    WatcherServer(QObject *parent=0,int port=0);
    QList<TcpClientSocket*> tcpClientSocketList;
     QList <MonitorClient *> MonitorList;
     QList <WatcherClient *> WatcherList;
    QTimer *timer;
    QTcpServer *m_pMonServer;
    int                m_nListenPort;
    QTcpServer *m_pMServer;
signals:
    void updateServer(QString,int);
    void Msg_log(QString &strLog,int type,bool display);
public slots:
    void updateClients(QString,int);
    void slotDisconnected(int);
    void update();
    void setMonitorServer(QTcpServer *pServer);

public:
    void SendMsg(const char *pdata,int length);
    void StartListen();
    int  ConnectedNum();
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER



