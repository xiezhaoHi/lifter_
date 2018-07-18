#include<QThread>
#include<QTime>
#include"usebuffer.h"
#include"enum.h"
#include"server/config/config.h"
#include"server/database_service/mydatabase.h"

UserBuffer* UserBuffer::m_singleton = NULL;


UserBuffer::UserBuffer()
{

m_database_queue_max = DATABASE_QUEUE_MAX;

}
//缓冲 清理
UserBuffer::~UserBuffer()
{

}

UserBuffer* UserBuffer::GetInstance(void)
{
    if(NULL == m_singleton)
     {
            m_singleton = new UserBuffer();
            m_singleton->m_strTableName[device_can] = QString("structure_stress_");
            m_singleton->m_strTableName[device_cgq] = QString("sensor_");
            m_singleton->m_strTableName[device_jdq] = QString("relay_");
            m_singleton->m_strTableName[device_bmq] = QString("encoder_");
			m_singleton->m_strTableName[device_dydl] = QString("power_");
            m_singleton->m_strDBName = QString("Lifter_");
            QDateTime current_date_time = QDateTime::currentDateTime();
        //    QString strDate = current_date_time.toString("yyyy/MM/dd");
        //    QString current_time = current_date_time.toString("hh:mm:ss.zzz");

            //CAN模块
            m_singleton->InitDBSqlBuff(device_can,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_can]+current_date_time.toString(QString("yyyyMMdd_hh")));

            //Cgq模块
            m_singleton->InitDBSqlBuff(device_cgq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_cgq]+current_date_time.toString(QString("yyyyMMdd")));
            //Jdq模块
            m_singleton->InitDBSqlBuff(device_jdq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_jdq]+current_date_time.toString(QString("yyyyMM")));
            //Bmq模块
            m_singleton->InitDBSqlBuff(device_bmq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_bmq]+current_date_time.toString(QString("yyyyMM")));
			//电源模块
			m_singleton->InitDBSqlBuff(device_dydl, m_singleton->m_strDBName + current_date_time.toString(QString("yyyyMM"))
				, m_singleton->m_strTableName[device_dydl] + current_date_time.toString(QString("yyyyMM")));

            QString strDBName = Config::GetInstance()->GetDBConfigData(database_dbName);
            strDBName = strDBName.isEmpty()?"data":strDBName;
            //有待优化 2017/7/6
            m_singleton->InitDBSqlBuffAll(config_device_config,strDBName,"device_config");
             m_singleton->InitDBSqlBuffAll(config_client_config,strDBName,"client_config");

     }
    return m_singleton;
}
void UserBuffer::ClearBuff()
{
    for(int index = 0; index <device_max; ++index)
    {
           if(m_iSqlCount[index]>0)
           {
               QString strSql = m_strSql[index];

               if(strSql.right(1) == ",")
                   strSql = strSql.mid(0,strSql.length()-1);
               m_database_queue.enqueue(strSql);
               m_cond_DB.notify_all();
           }
    }
}






/*
 * 初始化 m_strSql 缓存
 * 参数为 数据库名字
 * strDbname 与 strTableName 都是 时间后缀 例如201706  2017061616
 */
void    UserBuffer::InitDBSqlBuff(int flag,QString const& strDBName,QString const& strTableName)
{

    if(flag<0 || flag>=device_max)
        return;


    switch (flag) {
    case device_can:
        /*
         * 结构应力的 sql语句初始化
         *
         */
        m_strDefSql[device_can] = QString("INSERT INTO `%1`.`%2`\
                                              (`structure_stress_id`,\
                                               `structure_stress_date_id`,\
                                              `structure_stress_date`,\
                                              `structure_stress_time`,\
                                              `structure_stress_ms`,\
                                              `structure_stress_data`,\
                                              `save_date`,`save_time`)\
                                              VALUES\
                                              ").arg(strDBName)
                                          .arg(strTableName);

        break;

    case device_cgq:
                /*
                 * 传感器 获取 加速度 和水平度
                 */
                m_strDefSql[device_cgq] = QString("INSERT INTO `%1`.`%2`\
                                                  (`sensor_id`,\
                                                    `sensor_date`,\
                                                   `sensor_time`,\
                                                   `sensor_jd_x`,\
                                                  `sensor_jd_y`,\
                                                  `sensor_jd_z`,\
                                                  `sensor_jsd_x`,\
                                                  `sensor_jsd_y`,\
                                                  `sensor_jsd_z`)\
                                                  VALUES").arg(strDBName).arg(strTableName);

                break;
   case device_bmq:

                /*
                 * 编码器 设备 数据 sql
                 */
                 m_strDefSql[device_bmq] = QString("INSERT INTO `%1`.`%2`\
                                                   (`encoder_id`,\
                                                   `encoder_date`,\
                                                   `encoder_time`,\
                                                   `encoder_dir`,\
                                                   `encoder_jd`,\
                                                   `encoder_zs`,\
                                                   `encoder_jsz`)\
                                                   VALUES \
                                                   ").arg(strDBName).arg(strTableName);

                break;
    case device_jdq:
                /*
                 * 继电器 设备 数据 sql模板
                 */
                 m_strDefSql[device_jdq] = QString("INSERT INTO `%1`.`%2`\
                                                   (`relay_id`,\
                                                   `relay_date`,\
                                                   `relay_time`,\
                                                   `relay_state`)\
                                                   VALUES \
                                                   ").arg(strDBName).arg(strTableName);


                break;
	case device_dydl:
		/*
		* 电源 设备 数据 sql模板
		*/
		m_strDefSql[device_dydl] = QString("INSERT INTO `%1`.`%2`\
                                                  (`power_id`,\
													`power_date`,\
													`power_time`,\
													`power_data_a`,\
													`power_data_b`,\
													`power_data_c`)VALUES").arg(strDBName).arg(strTableName);


		break;
    default:
        break;
    }
    m_strSql[flag]  = m_strDefSql[flag];
     m_iSqlCount[flag] = 0;
     m_maxLen[flag] = 100;
}


/*
 * 初始化 m_strSql 缓存
 * 参数为
 * flag 表示数据类型 例如:device_jdq
 * strDBName 表示 数据库名字
 * strTableName 表名字
 */
void   UserBuffer::InitDBSqlBuffAll(int flag,QString const& strDBName,QString const& strTableName)
{
    switch (flag) {
    case config_device_config: //数据库 增加配置项 表device_config
         m_strSqlAll[flag] = QString(" REPLACE INTO %1.%2 (id,config) VALUES ").arg(strDBName).arg(strTableName);
        break;

    case config_client_config://数据库 增加配置项 表device_config client_config
         m_strSqlAll[flag] = QString(" REPLACE INTO %1.%2 (id,address,belongs) VALUES ").arg(strDBName).arg(strTableName);
        break;
    default:
        break;
    }

}

//写buffer
     bool UserBuffer::Write_buffer(int deviceID, char* buff, int len)
     {
         deviceID = deviceID-device_begin_ID;
         assert(deviceID>=0 && deviceID < device_max);
         boost::mutex::scoped_lock lock(m_deviceData_mutex[deviceID]);
        if( len != m_deviceData_buff[deviceID].write(buff,len)) //写入失败
        {
            return false;
        }

        m_deviceData_cond[deviceID].notify_all();
        return true;
     }


   //读buffer
     bool UserBuffer::Read_buffer(int deviceID, char*buff, int len)
     {
         deviceID = deviceID-device_begin_ID;
         assert(deviceID>=0 && deviceID < device_max);
         boost::mutex::scoped_lock lock( m_deviceData_mutex[deviceID]);

         switch (deviceID+device_begin_ID) {
         case device_cgq:
             if(m_deviceData_buff[deviceID].getUsedSize() < 11 || m_deviceData_buff[deviceID].isEmpty())
                  m_deviceData_cond[deviceID].wait(m_deviceData_mutex[deviceID]);
             break;
         default:
             if(m_deviceData_buff[deviceID].isEmpty())
                  m_deviceData_cond[deviceID].wait(m_deviceData_mutex[deviceID]);
             break;
         }

            if(len != m_deviceData_buff[deviceID].read(buff,len))
                return false;
            return true;
     }
     //读数据库任务队列
       QString UserBuffer::PopDatabaseQueue()
       {

              boost::mutex::scoped_lock lock( m_mutex_DB);
//             if( m_database_queue.isEmpty())
//                 m_cond_DB.wait(m_mutex_DB);
             QString strRet;
             if(!m_database_queue.isEmpty())
             {
                 int len = m_database_queue.size();
               strRet  =  m_database_queue.dequeue();
             }

             return strRet;
             //m_database_queue.pop_front();
       }
       //写数据库任务队列
 /*
  *
  *以不同设备为划分,存储不同设备的sql数据
  *
  *
  */
       void UserBuffer::PushDatabaseQueue(int flag,QString const& strSql)
       {
             boost::mutex::scoped_lock lock( m_mutex_DB);


                    if(++m_iSqlCount[flag]>=m_maxLen[flag])
                    {


                        m_strSql[flag] += strSql ;

                        //当队列大于 最大限度 则不继续存
                        if(m_database_queue.size() >= m_database_queue_max)
                         {
                            int len = m_database_queue.size();
                            int length = m_strSql[flag].length();
                            m_cond_DB.notify_all();
                            m_strSql[flag]  = m_strDefSql[flag];
                                 m_iSqlCount[flag] = 0;
                                return;
                         }

                        m_database_queue.enqueue(m_strSql[flag]);
#ifdef _TEST_
                        for(int index = 0; index <8; ++index)
                        m_database_queue.enqueue(m_strSql[flag]);
#endif
                        m_cond_DB.notify_all();
                        m_strSql[flag]  = m_strDefSql[flag];
                             m_iSqlCount[flag] = 0;
                    }
                    else
                    {
                        m_strSql[flag] += strSql + ",";

                    }
       }


       //写数据库队列
       /*默认  device_config表 配置
        *
        *
        */
       void UserBuffer::PushDatabaseQueueAll(QString const& strSql,int flag)
       {
           if(m_strSqlAll.contains(flag))
           {
               boost::mutex::scoped_lock lock( m_mutex_DB);
                QString strTemp = m_strSqlAll[flag] + strSql;
                m_database_queue.enqueue(strTemp);
                m_cond_DB.notify_all();
           }
       }

       //测试读取缓冲区
       bool UserBuffer::TestRead_buffer()
       {
             if(m_deviceData_buff[device_cgq-device_begin_ID].getUsedSize() > DATA_SIZE_CGQ )
                 return true;
             return false;

       }

int  UserBuffer::ReturnDatabaseQueueSize()
{
      boost::mutex::scoped_lock lock( m_mutex_DB);
    return m_database_queue.size();
}

  bool   UserBuffer::IsEmptyDatabaseQueue()
  {
        boost::mutex::scoped_lock lock( m_mutex_DB);
      return m_database_queue.isEmpty();
  }

void     UserBuffer::PushUserDLData(QStringList const& list)
{
     boost::mutex::scoped_lock lock( m_mutex_userDL);
     m_userDL_queue.enqueue(list);
     m_cond_userDL.notify_all();
}

QStringList UserBuffer::PopUserDLData()
{
    boost::mutex::scoped_lock lock( m_mutex_userDL);
   if( m_userDL_queue.isEmpty())
       m_cond_userDL.wait(m_mutex_userDL);
   return   m_userDL_queue.dequeue();
}
bool      UserBuffer::IsEmptyUserDLData()
{
    return  m_userDL_queue.isEmpty();
}

#include<qDebug>
SendData&    UserBuffer::FrontSendList(QString const& strID)
{

 boost::recursive_mutex::scoped_lock lock(m_sendList_mutex);

 if(m_sendList.contains(strID))
    return m_sendList[strID]->front();


}

void    UserBuffer::PushBackSendList(QString const& strID,SendData const& list)
{

boost::recursive_mutex::scoped_lock lock(m_sendList_mutex);
    if(!m_sendList.contains(strID))
    {
        boost::shared_ptr<QList<SendData>> p(new QList<SendData>);
        m_sendList[strID] = p;
    }

        m_sendList[strID]->push_back(list);


//    if((0xff&list.buff[2] )!= 0xc0)
//        qDebug() << list.len;

}

void    UserBuffer::PopFrontSendList(QString const& strID)
{

    boost::recursive_mutex::scoped_lock lock(m_sendList_mutex);
    if(m_sendList.contains(strID))
        m_sendList[strID]->pop_front();


}
bool    UserBuffer::IsEmptySendList(QString const& strID)
{

    boost::recursive_mutex::scoped_lock lock(m_sendList_mutex);
    if(m_sendList.contains(strID))
        return m_sendList[strID]->isEmpty();
    return true;

}
void    UserBuffer::ClearFrontSendList()
{

     boost::recursive_mutex::scoped_lock lock(m_sendList_mutex);
//    if(m_sendList.isEmpty())
//        return;
//    m_sendList.clear();

}
/*
 *写入数据   m_device_to_client_map
 */
void UserBuffer::WriteDeviceToClientMap(QString const& strIp,QString const& strData)
{
    boost::mutex::scoped_lock lock(m_device_to_client_map_mutex);

    m_device_to_data[strIp] = strData;


}

/*
 * 读出数据 m_device_to_client_map
 */
QStringList UserBuffer::ReadDeviceToClientMap(QString const& strLifterID)
{
     QStringList retList;
     if(m_device_to_data.isEmpty())
        return retList;

    boost::mutex::scoped_lock lock(m_device_to_client_map_mutex);
    QMap<QString,QString>::const_iterator begin = m_device_to_data.begin(),end=m_device_to_data.end();

    for(;begin != end; ++begin)
    {
        if(strLifterID == Config::GetInstance()->GetLifterIDByDeviceIp(begin.key()))
            retList.append(begin.value());
    }
    return retList;
}

/*
 *写入数据   m_device_to_dh_data
 * 设备ip 关联 数据
 */
void UserBuffer::WriteDeviceToDhData(QString const& strIp, QString const& strData)
{
    boost::mutex::scoped_lock lock(m_device_to_dh_data_map_mutex);

    m_device_to_dh_data[strIp] = strData;

}

/*
 * 读出指定 客户端 的动画 数据包
 */
QString  UserBuffer::ReadDeviceToDhData(QString const& strLifterID)
{
    QString retList;
    if(m_device_to_dh_data.isEmpty())
       return retList;

   boost::mutex::scoped_lock lock(m_device_to_dh_data_map_mutex);
   QMap<QString,QString>::const_iterator begin = m_device_to_dh_data.begin(),end=m_device_to_dh_data.end();

   for(;begin != end; ++begin)
   {
       if(strLifterID == Config::GetInstance()->GetLifterIDByDeviceIp(begin.key()))
           retList +=begin.value();
   }
   return retList;
}


/**************************************************/
/*
 * can 模块数据队列处理  m_can_device_queue
 *     QQueue<device_buf_T > m_can_device_queue;
    boost::mutex              m_mutex_can_device;
 */

void  UserBuffer::PushCanDeviceQueue(device_buf_T const& buffT)
{
    boost::mutex::scoped_lock lock(m_mutex_can_device);
    m_can_device_queue.push_back(buffT);
    int len = m_can_device_queue.length();
    qDebug() << len;
    return;
}

device_buf_T UserBuffer::PopCanDeviceQueue()
{
    boost::mutex::scoped_lock lock(m_mutex_can_device);
    device_buf_T ret;
    if(!m_can_device_queue.isEmpty())
    {
        ret = m_can_device_queue.front();
        m_can_device_queue.pop_front();
    }
   // int len = m_can_device_queue.length();
    return ret;
}



/**************************************************/

/*
 * 动画选择 记录 选择的 电梯ID
 */
void    UserBuffer::WriteDhxzBuffer(QString const& strLifterID)
{
    m_dhxz_id = strLifterID;
}

QString UserBuffer::ReadDhxzBuffer()
{
    return m_dhxz_id;
}

/**************************************************/
/*
 * 获取编码器相关计数值
 * strip 编码器ip
 * flag  上限位 下限位 每层的标识   BmqJsz
 * jsz 计数值
 */
void    UserBuffer::SetBmqJsz(QString const& strIp,int const& flag,int const& jsz)
{
     boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
      if(!m_bmq_begin_jsz.contains(strIp))
      {
        QStringList list;

        list[flag] = jsz;
        m_bmq_begin_jsz[strIp] =list;
      }
      else
      {
        m_bmq_begin_jsz[strIp][flag] = jsz;
      }
}


/*
 *获取 指定编码器 指定位置的 初始 计数值
 */

int     UserBuffer::GetBmqJsz(QString const& strIp,int const& flag)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    QString strRet = m_bmq_begin_jsz[strIp][flag];
    return strRet.toInt();
}


/*
 * 设置 当前 编码器 最新的计数值
 */
void    UserBuffer::SetBmqNewestJsz(QString const& strIp,int const& jsz)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    m_bmq_newest_jsz[strIp] = jsz;
}

/*
 * 获取 指定编码器 最新的计数值
 */
int     UserBuffer::GetBmqNewestJsz(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    if(m_bmq_newest_jsz.contains(strIp))
        return m_bmq_newest_jsz[strIp];
    return -1;
}


/*
 * 记录 制动 开始时 计数值
 */
void     UserBuffer::SetBmqBeginZdJsz(QString const& strIp,int const& jsz)
{
     boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    m_bmq_begin_zd_jsz[strIp] = jsz;
}

/*
 * 获取 制动 开始时 计数值
 */
int      UserBuffer::GetBmqBeginZdJsz(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    if(m_bmq_begin_zd_jsz.contains(strIp))
        return m_bmq_begin_zd_jsz[strIp];
    return -1;
}
/*
 * 获取 制动距离
 */
double  UserBuffer::GetBmqZdjl(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    if(m_bmq_zd_jl.contains(strIp))
        return m_bmq_zd_jl[strIp];
    return -1;
}

/**************************************************/
/*
 * 回复 客户端队列 读写
 * m_response_queue
 */
/*
 * 回复 队列
 */
//QQueue<response_data>   m_response_queue;
//boost::mutex            m_response_queue_mutex;
void    UserBuffer::PushResponseQueue(QString const& strIp,response_data const& data)
{
    boost::mutex::scoped_lock lock(m_response_queue_mutex);
    m_response_queue[strIp] = data;
}

/*
 * strIp  客户端ip
 * 返回 回复的 数据包
 */
QString UserBuffer::PopResponseQueue(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_response_queue_mutex);
    QString strRet;
    if(m_response_queue.contains(strIp) && !m_response_queue[strIp].strData.isEmpty())
    {
        strRet = m_response_queue[strIp].strData;
        m_response_queue[strIp].strData = "";
    }

    return strRet;
}
