#ifndef MONITORSERVER_H
#define MONITORSERVER_H
#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include "MonitorClient.h"
#include"WatcherClient.h"
#include "WatcherServer.h"
#include <QTimer>




class MonitorServer : public QTcpServer
{
    Q_OBJECT
public:
    MonitorServer(QObject *parent=0,int port=0);
     QList <MonitorClient *> MonitorList;
    QTimer *timer;
    QTcpServer *m_pWatcherServer;
    int m_nListenPort;
signals:
    void updateServer(QString,int);
    void Msg_log(QString &strLog,int type,bool display);
     void updataClients(stFrameHeader *header,char * body,int bodyLength);
public slots:
    void updateClients(QString,int);
    void slotsTest(QString ,int );
    void slotReceive(stFrameHeader *header,char * body,int bodyLength);
    void slotDisconnected(int);
    void update();

public:
    void SendMsg(const char *pdata,int length);
    void StartListen();
    int  ConnectedNum();
    void setWatcherServer(QTcpServer *pServer);
protected:
    void incomingConnection(qintptr socketDescriptor);
};
#endif // SERVER



