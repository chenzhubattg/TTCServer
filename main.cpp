//#include "treewidget.h"
#include <QCoreApplication>
#include <QFile>
#include "TCPServer.h"
#include <QDebug>
#include<QDir>
#include<QTime>
#include<QDateTime>
#include"common.h"
#include "MonitorServer.h"
#include "WatcherServer.h"


#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    char buf[104];
     buf[0] = 'a';
     buf[1] = 'a';
     buf[2] = '\0';
    QString str = buf;
    cout << str.length()<<endl;
//   return 0 ;



    QCoreApplication a(argc, argv);
    QDir dirLog;
    QString strLogPath = QString("%1/Log").arg(QDir::currentPath());
    dirLog.mkdir(strLogPath);
    QString strLogName = QString("%1.slog").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    strLogName = strLogPath + "/" + strLogName;
    glbfileLog.setFileName(strLogName);
    if (!glbfileLog.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered))
    {
        QString strLog("Creating log file is failure!");
        LogOut(strLog);
    }

    QDir dirWork;
    glbstrWorkDir = QString("%1/work").arg(QDir::currentPath());
    dirWork.mkdir(glbstrWorkDir);



    MonitorServer MServer(NULL,7010);
    WatcherServer WServer(NULL,7002);
    MServer.setWatcherServer(&WServer);
    WServer.setMonitorServer(&MServer);
    MServer.StartListen();
    WServer.StartListen();



    //dir.mkdir();
   /// QFile file("in.txt");


    // InitSystemParam();

     return a.exec();
    glbfileLog.close();
}
