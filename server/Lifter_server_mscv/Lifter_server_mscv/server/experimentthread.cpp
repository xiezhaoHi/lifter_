#include "experimentthread.h"
#include"businesssession.h"
#include <qdebug.h>
#include "CGoFloor.h"
#include <QDateTime>
//��������ʼ������ı�־
enum RenwuStart
{
	RenwuStart_end,
	RenwuStart_begin
};

//��������
enum RenwuType
{
	RenwuType_time, //����
	RenwuType_day //����
};


//����������ֵ ������Χ
static const int gBmqJszFudong = 150;

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
		qDebug() << QStringLiteral("����[%1]-ѡ��ʧ���������¥��[%2]-ѡ��¥��[%3-%4]!").arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_lifterID, device_bmq);
	int bmqNum = 1;//���ݹ����ı�������ţ�Ĭ��ֻ��һ��������
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //��ȡָ�����ݵ� ָ�� bmqID�� ID
	}

	//1.�����������
	if (m_data.m_start == RenwuStart_end)
	{
		qDebug() << QStringLiteral("�������񣬴�����û�п�ʼ��ֱ�ӽ���.[������ID:%1]").arg(deviceID);
		return;
	}

	//2.��������ʼ 


	//��ȡ���±���������ֵ
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

    //1.���ݻص����
	//1.1 ��ȡ�������õ� ��� ����ֵ
	int bmqCofBeginJsz = cof->m_lifterBmqJsz[m_data.m_begin-1];
	if (true != (bmqCofBeginJsz-gBmqJszFudong <= newBmqJsz 
		&& newBmqJsz <= bmqCofBeginJsz+gBmqJszFudong)) 
		//������������ֵ �ڵ�ǰ ����ֵ�ķ�Χ��,����Ϊ�������
	{
		//1.2 �����˶���ָ��λ��
		CGoFloor goFloor(m_lifterID, m_data.m_begin);
		goFloor.start();
		goFloor.wait();
	}

	if (RenwuType_time == m_data.m_type) //����
	{
		CGoFloor goFloorEnd(m_lifterID, m_data.m_end);
		CGoFloor goFloorBegin(m_lifterID, m_data.m_begin);
		for (int index = 0; index < m_data.m_times; ++index)
		{
			//�ӿ�ʼ-����
			
			goFloorEnd.start();
			goFloorEnd.wait();

			//�ӽ���-��ʼ
			
			goFloorBegin.start();
			goFloorBegin.wait();

			Sleep(10);
		}
	}
	else //ʱ�� ����
	{
		QDateTime beginTime = QDateTime::currentDateTime();
		QDateTime endTime = QDateTime::currentDateTime().addDays(m_data.m_times);
		CGoFloor goFloorEnd(m_lifterID, m_data.m_end);
		CGoFloor goFloorBegin(m_lifterID, m_data.m_begin);
		while (beginTime < endTime)
		{
			//�ӿ�ʼ-����

			goFloorEnd.start();
			goFloorEnd.wait();

			//�ӽ���-��ʼ

			goFloorBegin.start();
			goFloorBegin.wait();

			Sleep(10);
			beginTime = QDateTime::currentDateTime();
		}
	}

//     //QString deviceID;
//     int len = 0;
//     char buff[10] = {0};
// 
// 	JDQ_DEAL_DATA dealData;
// 	dealData.m_strBelongs = m_lifterID;
// 	dealData.m_strID = "20020007";
// 	dealData.m_strRec = QString("1");
// 	dealData.m_openFlag = 0;
// 
//     BusinessSession::GetInstance()->GetJdqControlOrder(dealData, deviceID,buff,len);
//     SendData msg;
//     memcpy(msg.buff,buff,len);
//     msg.len = len;
//     UserBuffer::GetInstance()->PushBackSendList(deviceID,msg);
// 	

}
