#include "experimentthread.h"
#include"businesssession.h"
/*
typedef struct experiment_struct
{
    int m_begin; //开始层数
    int m_end; //结束层数
    int m_type; // 周期类型 0,1 次数 天数
    int m_times; //周期数
}experimentData;
*/
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
		qDebug() << QStringLiteral("电梯[%1]-选层失败配置最大楼层[%2]-选择楼层[%3-%4]!")
			.arg(m_lifterID).arg(cof->m_lifterBmqJszNum).arg(m_data.m_begin).arg(m_data.m_end);
		return;
	}

	QMap<int, QString> retMap =
		Config::GetInstance()->GetDeviceIDListByLifterID(m_lifterID, device_bmq);
	int bmqNum = 1;//第一个编码器
	QString deviceID, strTemp;
	if (retMap.contains(bmqNum))
	{
		deviceID = retMap[bmqNum]; //获取指定电梯的 指定 bmqID的 ID
	}
	//获取最新编码器计数值
	int newBmqJsz = UserBuffer::GetInstance()->GetBmqNewestJsz(deviceID);

	

    //1.电梯回到起点
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
    if(0 == m_data.m_type) //次数
    {
        for(int index =0; index < m_data.m_times; ++index)
        {
            while(1)
            {

            }

        }
    }
    else //时间 天数
    {

    }
}
