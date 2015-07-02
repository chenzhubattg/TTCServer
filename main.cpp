#include "treewidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include "TCPServer.h"
#include <QDebug>
#include<QDir>
#include<QTime>
#include<QDateTime>
#include"common.h"
treeWidget *pWidget;
TCPServer *pServer;
#include <iostream>
using namespace std;


#define LOGCOUT(strLog) cout << QTime::currentTime().toString("hh:mm:ss").toStdString().c_str() << " "<<strLog.toStdString().c_str() << endl;




int main(int argc, char *argv[])
{
    QString strPath = QCoreApplication::applicationDirPath();
    QDir dirLog;
    QString strLogPath = QString("%1/Log").arg(QDir::currentPath());
    dirLog.mkdir(strLogPath);
    QString strLogName = QString("%1.slog").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    strLogName = strLogPath + "/" + strLogName;
    glbfileLog.setFileName(strLogName);
    if (!glbfileLog.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QString strLog("Creating log file is failure!");
        LogOut(strLog);
    }




    //dir.mkdir();
   /// QFile file("in.txt");
  //  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   //     return;

   // QTextStream in(&file);
   // while (!in.atEnd()) {
  //      QString line = in.readLine();
   //     process_line(line);
   // }


    // InitSystemParam();
    //QApplication a(argc, argv);
  //  pServer = new TCPServer(NULL,glbLocalPort);
  //  pWidget = new treeWidget;
  //  pWidget->show();
  //   return a.exec();
    glbfileLog.close();
}
