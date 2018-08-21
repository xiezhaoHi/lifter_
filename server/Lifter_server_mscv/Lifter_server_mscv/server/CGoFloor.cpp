#include "CGoFloor.h"
#include "businesssession.h"
#include <qdebug.h>


CGoFloor::CGoFloor(QString const& strLifterID, int const& floorID)
	:m_strLifterID(strLifterID), m_floorID(floorID)
{
}

CGoFloor::~CGoFloor()
{
}
void CGoFloor::run()
{
	lifterBmqJszCof* cof = UserBuffer::GetInstance()->ReadLifterBmqBuffer(m_lifterID);
	if (cof == nullptr)
	{
		qDebug() << QStringLiteral("����[%1]-��ȡ������ÿ�����ֵʧ��!").arg(m_lifterID);
		return;
	}
	//��ʼ¥����Ч
	if (cof->m_lifterBmqJszNum <= m_data.m_begin && cof->m_lifterBmqJszNum <= m_data.m_end)
	{
		qDebug() << QStringLiteral("����[%1]-ѡ��ʧ���������¥��[%2]-ѡ��¥��[%3-%4]!").arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_strLifterID, device_bmq);
	int bmqNum = 1;//���ݹ����ı�������ţ�Ĭ��ֻ��һ��������
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //��ȡָ�����ݵ� ָ�� bmqID�� ID
	}
	
	//��ȡ���±���������ֵ
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

}
