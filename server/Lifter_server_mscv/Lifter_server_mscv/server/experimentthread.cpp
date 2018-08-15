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
 * ����
 * ����
 * ���Ὺ��
 */
static QStringList lifterFload = {"20020007"
                                  ,"20020008" 
								 ,"20020015"
};
/*
����:
������������:
1.���С�����(�������Ὺ��)
2. ѡ�� ͨ�������� ���ؼ���ֵȷ��.
*/

void ExperimentThread::run()
{

	//0.��ȡ�������õ�ÿ�����ֵ
	lifterBmqJszCof* cof =UserBuffer::GetInstance()->ReadLifterBmqBuffer(m_lifterID);
	if (cof == nullptr)
	{
		qDebug() << QStringLiteral("����[%1]-��ȡ������ÿ�����ֵʧ��!").arg(m_lifterID);
		return;
	}
	//��ʼ¥����Ч
	if (cof->m_lifterBmqJszNum <= m_data.m_begin && cof->m_lifterBmqJszNum <= m_data.m_end )
	{
		qDebug() << QStringLiteral("����[%1]-ѡ��ʧ���������¥��[%2]-ѡ��¥��[%3-%4]!")
			.arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_lifterID, device_bmq);
	int bmqNum = 1;//��һ��������
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //��ȡָ�����ݵ� ָ�� bmqID�� ID
	}
	//��ȡ���±���������ֵ
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

	

    //1.���ݻص����
    QString deviceID;
    int len = 0;
    char buff[10] = {0};

	JDQ_DEAL_DATA dealData;
	dealData.m_strBelongs = m_lifterID;
	dealData.m_strID = "20020007";
	dealData.m_strRec = QString("1");
	dealData.m_openFlag = 0;

    BusinessSession::GetInstance()->GetJdqControlOrder(dealData, deviceID,buff,len);
    SendData msg;
    memcpy(msg.buff,buff,len);
    msg.len = len;
    UserBuffer::GetInstance()->PushBackSendList(deviceID,msg);
	int beginJsz = UserBuffer::GetInstance()->GetBmqJsz();
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
