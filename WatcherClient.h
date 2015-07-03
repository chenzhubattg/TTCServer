#ifndef WATCHERCLIENT_H
#define WATCHERCLIENT_H
#include <QTcpSocket>
#include <QObject>
#include"common.h"


class WatcherClient : public QTcpSocket
{
    Q_OBJECT
public:
    WatcherClient(QObject *parent=0);
    QByteArray 		dataFlow;
public:
signals:
    void updateClients(QString,int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody(stFrameHeader &header,char * body,int bodyLength);
};

#endif // TCPCLIENTSOCKET_H
