#ifndef MONITORCLIENT_H
#define MONITORCLIENT_H
#include <QTcpSocket>
#include <QObject>
#include"common.h"

class MonitorClient : public QTcpSocket
{
    Q_OBJECT
public:
    MonitorClient(QObject *parent=0);
    QByteArray 		dataFlow;
signals:
  void updateClients(QString,int);
    void updateClients(stFrameHeader *,char * ,int);
    void disconnected(int);
    void slotsTest(QString ,int );
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody(stFrameHeader &header,char * body,int bodyLength);
};

#endif // TCPCLIENTSOCKET_H
