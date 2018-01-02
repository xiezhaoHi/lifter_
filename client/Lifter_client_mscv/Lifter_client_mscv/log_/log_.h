#ifndef Log__H
#define Log__H

#include<QTextCodec>
#include"enum.h"
class Log_
{
public:

    static Log_* GetInstance(void);


     //void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

     bool  Init_log(void);

     bool Write_log(QString strErr,int level = logerr_debug);

private:
    Log_();
   static Log_* m_singleton;
};

#endif // Log__H
