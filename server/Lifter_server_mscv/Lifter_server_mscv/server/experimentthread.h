#ifndef EXPERIMENTTHREAD_H
#define EXPERIMENTTHREAD_H

#include"config/config.h"
#include<QThread>
typedef struct experiment_struct
{
    int m_begin; //开始层数
    int m_end; //结束层数
    int m_type; // 周期类型 0,1 次数 天数
    int m_times; //周期数
}experimentData;

class ExperimentThread : public QThread
{
public:
    ExperimentThread(QString const& strIp):m_strIp(strIp){}
    void SetData(experimentData const& struct_Data)
    {
        m_data = struct_Data;
    }


    void run();
    experimentData m_data;
    QString m_strIp;

};

#endif // EXPERIMENTTHREAD_H
