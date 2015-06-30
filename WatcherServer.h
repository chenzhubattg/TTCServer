#ifndef WATCHERSERVER_H
#define WATCHERSERVER_H

#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include "MonitorClient.h"
#include"WatcherClient.h"
#include <QTimer>


class WatcherServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPServer(QObject *parent=0,int port=0);
    QList<TcpClientSocket*> tcpClientSocketList;
     QList <MonitorClient> MonitorList;
     QList <WatcherClient> WatcherList;
    QTimer *timer;
signals:
    void updateServer(QString,int);
    void Msg_log(QString &strLog,int type,bool display);
public slots:
    void updateClients(QString,int);
    void slotDisconnected(int);
    void update();

public:
    void SendMsg(const char *pdata,int length);
    void Log(QString &strLog,int type,bool display);
    void StartListen();
    int  ConnectedNum();
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER



