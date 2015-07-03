#include "common.h"
#include <QTest>
#include <sstream>
#include <fstream>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <QSettings>
#include <QDir>
#include <QTime>

#include <iostream>
#include<QFile>
using namespace std;

const QString glbstrXMLParamFile = "Top_block_param.xml";
QString glbExeName = "top_block.py";
QString glbLocalAdress = "127.0.0.1";
int         glbLocalPort = 8010;
QString glbServerAdress = "127.0.0.1";
int         glbServerPort = 9090;

QFile glbfileLog;

void InitSystemParam()
{
    QDir dir = QDir::	current();
    QString sFile = "Option.ini";
    if (!dir.exists(sFile))
    {
        cout << "Info:Option.ini is not existed,the program will build a new Option.ini."<<endl;
        QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("DevOption");
        settings.setValue("glbLocalAdress", glbLocalAdress);
        settings.setValue("glbLocalPort", glbLocalPort);
        settings.setValue("glbServerAdress", glbServerAdress);
        settings.setValue("glbServerPort", glbServerPort);
        settings.setValue("glbExeName", glbExeName);
        settings.endGroup();
    }
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    glbLocalAdress = settings.value("DevOption/glbLocalAdress").toString();
    glbLocalPort = settings.value("DevOption/glbLocalPort").toInt();
    glbServerAdress = settings.value("DevOption/glbServerAdress").toString();
    glbServerPort = settings.value("DevOption/glbServerPort").toInt();
    glbExeName = settings.value("DevOption/glbExeName").toString();
}
bool KillProcess(QString sName)
{
    QString RE = "\\d\\d:\\d\\d:\\d\\d python " +  sName; //  python top_block.py";
    QRegExp rgeexp(RE);
    system("ps -ef | grep python > peinfo &");
    QTest::qWait(1000);
    QFile file("peinfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Error:Can't find peinfo file!!!" << endl;
        return false;
    }
    bool bFindTopblock = false;
    QString strLine;
    while (!file.atEnd()) {
        strLine = QString(file.readLine());
        qDebug()<< strLine;
        if (strLine.contains(rgeexp))
        {
            bFindTopblock = true;
            break;
        }
    }
    if (!bFindTopblock)
    {
        cout << "Error:Can't find Radio Thread!!!" << endl;
        file.close();
        return false;
    }
    else
    {
        cout << "Info:Find Radio Thread!!!"<<endl;
        QStringList list1 = strLine.split(QRegExp("\\s+"));
        QString KillCmd = "kill -s 9 " + list1[1];
        cout << "Info:" << KillCmd.toStdString().c_str() << endl;
        system (KillCmd.toStdString().c_str());
        QTest::qWait(100);
        return true;
    }
}


void LogOut(QString strLog)
{
    cout << QTime::currentTime().toString("hh:mm:ss").toStdString().c_str() << " "<<strLog.toStdString().c_str() << endl;
}

void LogFile(QFile &file, QString strLog)
{
    if (!file.exists())
    {
        LogOut("Log file is not exist.");
        return;
    }
    strLog = QTime::currentTime().toString("hh:mm:ss.zzz ") + strLog;
    file.write(strLog.toStdString().c_str(),strLog.length());
    cout << strLog.toStdString().c_str() << endl;

}

