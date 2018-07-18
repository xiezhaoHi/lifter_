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
//���� ����
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

            //CANģ��
            m_singleton->InitDBSqlBuff(device_can,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_can]+current_date_time.toString(QString("yyyyMMdd_hh")));

            //Cgqģ��
            m_singleton->InitDBSqlBuff(device_cgq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_cgq]+current_date_time.toString(QString("yyyyMMdd")));
            //Jdqģ��
            m_singleton->InitDBSqlBuff(device_jdq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_jdq]+current_date_time.toString(QString("yyyyMM")));
            //Bmqģ��
            m_singleton->InitDBSqlBuff(device_bmq,m_singleton->m_strDBName+current_date_time.toString(QString("yyyyMM"))
                          ,m_singleton->m_strTableName[device_bmq]+current_date_time.toString(QString("yyyyMM")));
			//��Դģ��
			m_singleton->InitDBSqlBuff(device_dydl, m_singleton->m_strDBName + current_date_time.toString(QString("yyyyMM"))
				, m_singleton->m_strTableName[device_dydl] + current_date_time.toString(QString("yyyyMM")));

            QString strDBName = Config::GetInstance()->GetDBConfigData(database_dbName);
            strDBName = strDBName.isEmpty()?"data":strDBName;
            //�д��Ż� 2017/7/6
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
 * ��ʼ�� m_strSql ����
 * ����Ϊ ���ݿ�����
 * strDbname �� strTableName ���� ʱ���׺ ����201706  2017061616
 */
void    UserBuffer::InitDBSqlBuff(int flag,QString const& strDBName,QString const& strTableName)
{

    if(flag<0 || flag>=device_max)
        return;


    switch (flag) {
    case device_can:
        /*
         * �ṹӦ���� sql����ʼ��
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
                 * ������ ��ȡ ���ٶ� ��ˮƽ��
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
                 * ������ �豸 ���� sql
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
                 * �̵��� �豸 ���� sqlģ��
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
		* ��Դ �豸 ���� sqlģ��
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
 * ��ʼ�� m_strSql ����
 * ����Ϊ
 * flag ��ʾ�������� ����:device_jdq
 * strDBName ��ʾ ���ݿ�����
 * strTableName ������
 */
void   UserBuffer::InitDBSqlBuffAll(int flag,QString const& strDBName,QString const& strTableName)
{
    switch (flag) {
    case config_device_config: //���ݿ� ���������� ��device_config
         m_strSqlAll[flag] = QString(" REPLACE INTO %1.%2 (id,config) VALUES ").arg(strDBName).arg(strTableName);
        break;

    case config_client_config://���ݿ� ���������� ��device_config client_config
         m_strSqlAll[flag] = QString(" REPLACE INTO %1.%2 (id,address,belongs) VALUES ").arg(strDBName).arg(strTableName);
        break;
    default:
        break;
    }

}

//дbuffer
     bool UserBuffer::Write_buffer(int deviceID, char* buff, int len)
     {
         deviceID = deviceID-device_begin_ID;
         assert(deviceID>=0 && deviceID < device_max);
         boost::mutex::scoped_lock lock(m_deviceData_mutex[deviceID]);
        if( len != m_deviceData_buff[deviceID].write(buff,len)) //д��ʧ��
        {
            return false;
        }

        m_deviceData_cond[deviceID].notify_all();
        return true;
     }


   //��buffer
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
     //�����ݿ��������
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
       //д���ݿ��������
 /*
  *
  *�Բ�ͬ�豸Ϊ����,�洢��ͬ�豸��sql����
  *
  *
  */
       void UserBuffer::PushDatabaseQueue(int flag,QString const& strSql)
       {
             boost::mutex::scoped_lock lock( m_mutex_DB);


                    if(++m_iSqlCount[flag]>=m_maxLen[flag])
                    {


                        m_strSql[flag] += strSql ;

                        //�����д��� ����޶� �򲻼�����
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


       //д���ݿ����
       /*Ĭ��  device_config�� ����
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

       //���Զ�ȡ������
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
 *д������   m_device_to_client_map
 */
void UserBuffer::WriteDeviceToClientMap(QString const& strIp,QString const& strData)
{
    boost::mutex::scoped_lock lock(m_device_to_client_map_mutex);

    m_device_to_data[strIp] = strData;


}

/*
 * �������� m_device_to_client_map
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
 *д������   m_device_to_dh_data
 * �豸ip ���� ����
 */
void UserBuffer::WriteDeviceToDhData(QString const& strIp, QString const& strData)
{
    boost::mutex::scoped_lock lock(m_device_to_dh_data_map_mutex);

    m_device_to_dh_data[strIp] = strData;

}

/*
 * ����ָ�� �ͻ��� �Ķ��� ���ݰ�
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
 * can ģ�����ݶ��д���  m_can_device_queue
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
 * ����ѡ�� ��¼ ѡ��� ����ID
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
 * ��ȡ��������ؼ���ֵ
 * strip ������ip
 * flag  ����λ ����λ ÿ��ı�ʶ   BmqJsz
 * jsz ����ֵ
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
 *��ȡ ָ�������� ָ��λ�õ� ��ʼ ����ֵ
 */

int     UserBuffer::GetBmqJsz(QString const& strIp,int const& flag)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    QString strRet = m_bmq_begin_jsz[strIp][flag];
    return strRet.toInt();
}


/*
 * ���� ��ǰ ������ ���µļ���ֵ
 */
void    UserBuffer::SetBmqNewestJsz(QString const& strIp,int const& jsz)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    m_bmq_newest_jsz[strIp] = jsz;
}

/*
 * ��ȡ ָ�������� ���µļ���ֵ
 */
int     UserBuffer::GetBmqNewestJsz(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    if(m_bmq_newest_jsz.contains(strIp))
        return m_bmq_newest_jsz[strIp];
    return -1;
}


/*
 * ��¼ �ƶ� ��ʼʱ ����ֵ
 */
void     UserBuffer::SetBmqBeginZdJsz(QString const& strIp,int const& jsz)
{
     boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    m_bmq_begin_zd_jsz[strIp] = jsz;
}

/*
 * ��ȡ �ƶ� ��ʼʱ ����ֵ
 */
int      UserBuffer::GetBmqBeginZdJsz(QString const& strIp)
{
    boost::mutex::scoped_lock lock(m_mutex_bmq_zd);
    if(m_bmq_begin_zd_jsz.contains(strIp))
        return m_bmq_begin_zd_jsz[strIp];
    return -1;
}
/*
 * ��ȡ �ƶ�����
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
 * �ظ� �ͻ��˶��� ��д
 * m_response_queue
 */
/*
 * �ظ� ����
 */
//QQueue<response_data>   m_response_queue;
//boost::mutex            m_response_queue_mutex;
void    UserBuffer::PushResponseQueue(QString const& strIp,response_data const& data)
{
    boost::mutex::scoped_lock lock(m_response_queue_mutex);
    m_response_queue[strIp] = data;
}

/*
 * strIp  �ͻ���ip
 * ���� �ظ��� ���ݰ�
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
