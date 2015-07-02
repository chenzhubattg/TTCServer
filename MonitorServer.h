#ifndef MONITORSERVER_H
#define MONITORSERVER_H
#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include "MonitorClient.h"
#include"WatcherClient.h"
#include "WatcherServer.h"
#include <QTimer>

#define CMD_LOAD 0x01                               //  Load *.grc and *.xml of flow graph
#define CMD_START 0x0x2                             //  Running signal processing program in each nodes.
#define CMD_KILL 0x03                                  //  Kill signal processing thread in each nodes.
#define CMD_SETPARAM 0x04                      //   Set system param by relaying *.xml
#define CMD_GETSTATUS 0x05                     //   Get system status by relaying *.xml

class MonitorServer : public QTcpServer
{
    Q_OBJECT
public:
    MonitorServer(QObject *parent=0,int port=0);
     QList <MonitorClient *> MonitorList;
    QTimer *timer;
    WatcherServer *m_pWatcherServer;
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
    void setWatcherServer(WatcherServer *pServer);
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER



