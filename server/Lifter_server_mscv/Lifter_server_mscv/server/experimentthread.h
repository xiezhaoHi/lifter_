#ifndef EXPERIMENTTHREAD_H
#define EXPERIMENTTHREAD_H

#include"config/config.h"
#include<QThread>
typedef struct experiment_struct
{
    int m_begin; //��ʼ����
    int m_end; //��������
    int m_type; // �������� 0,1 ���� ����
    int m_times; //������
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