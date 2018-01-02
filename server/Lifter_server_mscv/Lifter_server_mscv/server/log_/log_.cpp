#include "log_.h"
#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QApplication>
#include<QDebug>
#include<qlogging.h>
#include<QDir>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
Log_* Log_::m_singleton = NULL;
static QString g_strDir;
Log_::Log_()
{

}

void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("debug");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    //QString strTemp = QString(context.file);
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:ms ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);


    //当天的 日志文件目录 qApp->applicationDirPath()
    QString strLogDirNameForToday = g_strDir+QString("/logfiles/%1")
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd"));//
    QDir logDirForToday(strLogDirNameForToday);
    if (!logDirForToday.exists()) {
        int ret = logDirForToday.mkpath(strLogDirNameForToday);
        if (!ret) {
            return;
        }
    }

   QString strFile =strLogDirNameForToday + QString("/%1.txt").arg( QDateTime::currentDateTime().toString("yyyyMMddhh"));
    QFile file(strFile);

    //如果打开失败,文件不存在
    if(file.exists())
    {
          file.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    else
    {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
    }

    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}


bool Log_::Init_log(QString strDir)
{
    //注册MessageHandler
    g_strDir = strDir;
       qInstallMessageHandler(OutputMessage);
       return true;
}


 Log_* Log_::GetInstance(void)
 {
     if(NULL == m_singleton)
         m_singleton = new Log_();
     return m_singleton;
 }


 bool Log_::Write_log(QString strErr,int level )
 {
     switch (level) {
     case logerr_debug:
        qDebug() << strErr;
         break;
     case logerr_fatal:
         qDebug() << strErr;
         break;
     default:
         break;
     }
     return true;
 }
