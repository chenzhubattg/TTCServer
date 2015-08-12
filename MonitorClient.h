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
    void updateClients( QString,int );
    void updataClients( stFrameHeader *,char * ,qint32 );
    void disconnected(int);
    void slotsT(QString * ,int * );
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody( stFrameHeader &header,char * body,int bodyLength );
};

#endif // TCPCLIENTSOCKET_H
