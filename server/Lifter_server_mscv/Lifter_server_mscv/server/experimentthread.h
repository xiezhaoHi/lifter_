#ifndef EXPERIMENTTHREAD_H
#define EXPERIMENTTHREAD_H

#include"config/config.h"
#include<QThread>

typedef struct experiment_struct
{
	int m_start; //开始或结束 0 结束 1开始
	QString m_lifterID; //电梯ID
    int m_begin; //开始层数 从 1 开始:例如 1-7
    int m_end; //结束层数
    int m_type; // 周期类型 0,1 次数 天数
    int m_times; //周期数
}experimentData;

class ExperimentThread : public QThread
{
public:
    ExperimentThread(QString const& lifterID):m_lifterID(lifterID){}
    void SetData(experimentData const& struct_Data)
    {
        m_data = struct_Data;
    }


    void run();
    experimentData m_data;
    QString m_lifterID;
	
};

#endif // EXPERIMENTTHREAD_H
