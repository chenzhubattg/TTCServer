#ifndef WATCHERCLIENT_H
#define WATCHERCLIENT_H
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include"common.h"
#include <QHostAddress>


class WatcherClient : public QTcpSocket
{
    Q_OBJECT
public:
    WatcherClient(QObject *parent=0);
    QByteArray 		dataFlow;
public:
signals:
    void updateClients( QString,int );
    void updateClients( stFrameHeader *,char * ,qint32, QString );
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody( stFrameHeader &header,char * body,int bodyLength );
};

#endif // TCPCLIENTSOCKET_H
