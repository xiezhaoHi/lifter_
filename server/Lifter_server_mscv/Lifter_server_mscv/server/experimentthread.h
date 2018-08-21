#ifndef EXPERIMENTTHREAD_H
#define EXPERIMENTTHREAD_H

#include"config/config.h"
#include<QThread>

typedef struct experiment_struct
{
	int m_start; //��ʼ����� 0 ���� 1��ʼ
	QString m_lifterID; //����ID
    int m_begin; //��ʼ���� �� 1 ��ʼ:���� 1-7
    int m_end; //��������
    int m_type; // �������� 0,1 ���� ����
    int m_times; //������
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
