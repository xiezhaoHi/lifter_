#include "experimentthread.h"
#include"businesssession.h"
/*
typedef struct experiment_struct
{
    int m_begin; //��ʼ����
    int m_end; //��������
    int m_type; // �������� 0,1 ���� ����
    int m_times; //������
}experimentData;
*/
/*
 *�����Զ�ٶ�����, �м����һ���ƶ�, �����յ�, �Զ�ٶ� �½�,�м� ����һ���ƶ�
 */
static QStringList lifterFload = {""
                                  ,"20020001"
                                  ,"20020002"
                                  ,"20020003"
                                  ,"20020004"
                                  ,"20020005"
                                  ,"20020006"
};

void ExperimentThread::run()
{
    //1.���ݻص����
    QString deviceIp;
    int len = 0;
    char buff[10] = {0};
    BusinessSession::GetInstance()->GetJdqControlOrder(m_strIp,lifterFload[m_data.m_begin],QString("1"),deviceIp,buff,len);
    SendData msg;
    memcpy(msg.buff,buff,len);
    msg.len = len;
    UserBuffer::GetInstance()->PushBackSendList(deviceIp,msg);
   // int beginJsz = UserBuffer::GetInstance()->GetBmqJsz()
    if(0 == m_data.m_type) //����
    {
        for(int index =0; index < m_data.m_times; ++index)
        {
            while(1)
            {

            }

        }
    }
    else //ʱ�� ����
    {

    }
}
