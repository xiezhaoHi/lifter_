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
 *电梯以额定速度上升, 中间进行一次制动, 到达终点, 以额定速度 下降,中间 进行一次制动
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
    //1.电梯回到起点
    QString deviceIp;
    int len = 0;
    char buff[10] = {0};
    BusinessSession::GetInstance()->GetJdqControlOrder(m_strIp,lifterFload[m_data.m_begin],QString("1"),deviceIp,buff,len);
    SendData msg;
    memcpy(msg.buff,buff,len);
    msg.len = len;
    UserBuffer::GetInstance()->PushBackSendList(deviceIp,msg);
   // int beginJsz = UserBuffer::GetInstance()->GetBmqJsz()
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
