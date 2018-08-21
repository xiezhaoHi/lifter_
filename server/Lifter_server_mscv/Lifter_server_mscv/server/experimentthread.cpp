#include "experimentthread.h"
#include"businesssession.h"
#include <qdebug.h>
#include "CGoFloor.h"
#include <QDateTime>
//周期任务开始或结束的标志
enum RenwuStart
{
	RenwuStart_end,
	RenwuStart_begin
};

//周期类型
enum RenwuType
{
	RenwuType_time, //次数
	RenwuType_day //天数
};


//编码器计数值 浮动范围
static const int gBmqJszFudong = 150;

/*
 * 上行
 * 下行
 * 手提开关
 */
static QStringList lifterFload = {"20020007"
                                  ,"20020008" 
								 ,"20020015"
};
/*
分析:
电梯周期运行:
1.上行、下行(包括手提开关)
2. 选择 通过编码器 返回计数值确定.
*/

void ExperimentThread::run()
{

	//0.获取电梯配置的每层计数值
	lifterBmqJszCof* cof =UserBuffer::GetInstance()->ReadLifterBmqBuffer(m_lifterID);
	if (cof == nullptr)
	{
		qDebug() << QStringLiteral("电梯[%1]-获取编码器每层计数值失败!").arg(m_lifterID);
		return;
	}
	//开始楼层无效
	if (cof->m_lifterBmqJszNum <= m_data.m_begin && cof->m_lifterBmqJszNum <= m_data.m_end )
	{
		qDebug() << QStringLiteral("电梯[%1]-选层失败配置最大楼层[%2]-选择楼层[%3-%4]!").arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_lifterID, device_bmq);
	int bmqNum = 1;//电梯关联的编码器编号，默认只有一个编码器
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //获取指定电梯的 指定 bmqID的 ID
	}

	//1.周期任务结束
	if (m_data.m_start == RenwuStart_end)
	{
		qDebug() << QStringLiteral("结束任务，此任务还没有开始，直接结束.[编码器ID:%1]").arg(deviceID);
		return;
	}

	//2.周期任务开始 


	//获取最新编码器计数值
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

    //1.电梯回到起点
	//1.1 获取电梯配置的 起点 计数值
	int bmqCofBeginJsz = cof->m_lifterBmqJsz[m_data.m_begin-1];
	if (true != (bmqCofBeginJsz-gBmqJszFudong <= newBmqJsz 
		&& newBmqJsz <= bmqCofBeginJsz+gBmqJszFudong)) 
		//起点编码器计数值 在当前 计数值的范围中,则认为是在起点
	{
		//1.2 电梯运动到指定位置
		CGoFloor goFloor(m_lifterID, m_data.m_begin);
		goFloor.start();
		goFloor.wait();
	}

	if (RenwuType_time == m_data.m_type) //次数
	{
		CGoFloor goFloorEnd(m_lifterID, m_data.m_end);
		CGoFloor goFloorBegin(m_lifterID, m_data.m_begin);
		for (int index = 0; index < m_data.m_times; ++index)
		{
			//从开始-结束
			
			goFloorEnd.start();
			goFloorEnd.wait();

			//从结束-开始
			
			goFloorBegin.start();
			goFloorBegin.wait();

			Sleep(10);
		}
	}
	else //时间 天数
	{
		QDateTime beginTime = QDateTime::currentDateTime();
		QDateTime endTime = QDateTime::currentDateTime().addDays(m_data.m_times);
		CGoFloor goFloorEnd(m_lifterID, m_data.m_end);
		CGoFloor goFloorBegin(m_lifterID, m_data.m_begin);
		while (beginTime < endTime)
		{
			//从开始-结束

			goFloorEnd.start();
			goFloorEnd.wait();

			//从结束-开始

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
