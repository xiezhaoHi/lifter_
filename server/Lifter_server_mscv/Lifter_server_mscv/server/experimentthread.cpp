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

	JDQ_DEAL_DATA dealData;
	dealData.m_strBelongs = m_lifterID;
	dealData.m_strID = lifterFload[m_data.m_begin];
	dealData.m_strRec = QString("1");
	dealData.m_openFlag = 0;

    BusinessSession::GetInstance()->GetJdqControlOrder(dealData,deviceIp,buff,len);
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
