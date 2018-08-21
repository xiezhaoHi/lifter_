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
		qDebug() << QStringLiteral("电梯[%1]-获取编码器每层计数值失败!").arg(m_lifterID);
		return;
	}
	//开始楼层无效
	if (cof->m_lifterBmqJszNum <= m_data.m_begin && cof->m_lifterBmqJszNum <= m_data.m_end)
	{
		qDebug() << QStringLiteral("电梯[%1]-选层失败配置最大楼层[%2]-选择楼层[%3-%4]!").arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_strLifterID, device_bmq);
	int bmqNum = 1;//电梯关联的编码器编号，默认只有一个编码器
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //获取指定电梯的 指定 bmqID的 ID
	}
	
	//获取最新编码器计数值
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

}
