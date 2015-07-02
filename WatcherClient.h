#ifndef WATCHERCLIENT_H
#define WATCHERCLIENT_H
#include <QTcpSocket>
#include <QObject>
#include"common.h"
#include"MonitorServer.h"

class WatcherClient : public QTcpSocket
{
    Q_OBJECT
public:
    WatcherClient(QObject *parent=0);
    QByteArray 		dataFlow;
public:
    MonitorServer *pMServer;
signals:
    void updateClients(QString,int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody(stFrameHeader &header,char * body,int bodyLength);
    void setMonitorServer(MonitorServer *pServer);
};

#endif // TCPCLIENTSOCKET_H
