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
#include<QSettings>


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
    QString strLogPath = QString("%1/ServerLog").arg(QDir::currentPath());
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
    glbstrWorkDir = QString("%1/ServerWork").arg(QDir::currentPath());
    dirWork.mkdir(glbstrWorkDir);

   QString strOptionFile = "Option.ini";
   QString strLog("Option.ini is not existed, the default params will be used.");
   QDir dir = QDir::	current();
   if (!dir.exists(strOptionFile))
   {
            LogFile(glbfileLog,strLog);
   }
   else
   {
        QSettings settings(strOptionFile, QSettings::IniFormat); // 当前目录的INI文件
       glbMonitorServerPort = settings.value("TTCServer/MonitorServerPort").toInt();// = 9000;
       glbWatcherServerPort = settings.value("TTCServer/WatcherServerPort").toInt(); // = 9001;
   }


    MonitorServer MServer(NULL,glbMonitorServerPort);
    WatcherServer WServer(NULL,glbWatcherServerPort);
    MServer.setWatcherServer(&WServer);
    WServer.setMonitorServer(&MServer);
    WServer.StartListen();
    MServer.StartListen();




    //dir.mkdir();
   /// QFile file("in.txt");


    // InitSystemParam();

     return a.exec();
    glbfileLog.close();
}
