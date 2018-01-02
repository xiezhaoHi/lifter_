#include"usebuffer.h"
#include"enum.h"




UserBuffer* UserBuffer::m_singleton = NULL;


UserBuffer::UserBuffer()
{

   for(int index = 0; index <detection_max; ++index)
   {
       m_strSql[index]  = QString("INSERT INTO `data_test`.`monitordate` (`MonitorCode`, `MonitorName`, `MonitorType`, \
                              `MonitorDesc`, `MonitorResult`, `MonitorFromElevator`) VALUES");
        m_iSqlCount[index] = 0;
        m_maxLen[index] = 100;
   }


}

UserBuffer* UserBuffer::GetInstance(void)
{
    if(NULL == m_singleton)
        m_singleton = new UserBuffer();
    return m_singleton;
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
             if( m_database_queue.isEmpty())
                 m_cond_DB.wait(m_mutex_DB);
             QString strRet;
             if(!m_database_queue.isEmpty())
               strRet  =  m_database_queue.dequeue();

             return strRet;
             //m_database_queue.pop_front();
       }
       //д���ݿ��������
 /*
  *
  *
  *
  *       detection_spd,//����/����ˮƽ��
  * //����/�������мӡ����ٶȼ�⹦��
  *  detection_jjsd,//����/��������мӼ��ٶ�
  *
  *
  */
       void UserBuffer::PushDatabaseQueue(int flag,QString const& strSql)
       {
             boost::mutex::scoped_lock lock( m_mutex_DB);
                    if(++m_iSqlCount[flag]>=m_maxLen[flag])
                    {
                        m_strSql[flag] += strSql ;


                        m_database_queue.enqueue(m_strSql[flag]);
                        m_cond_DB.notify_all();
                        m_strSql[flag]  = QString("INSERT INTO `data_test`.`monitordate` (`MonitorCode`, `MonitorName`, `MonitorType`, \
                                               `MonitorDesc`, `MonitorResult`, `MonitorFromElevator`) VALUES");
                             m_iSqlCount[flag] = 0;
                    }
                    else
                    {
                        m_strSql[flag] += strSql + ",";

                    }
       }
       //���Զ�ȡ������
       bool UserBuffer::TestRead_buffer()
       {
             if(m_deviceData_buff[device_cgq-device_begin_ID].getUsedSize() < 11 || m_deviceData_buff[device_cgq-device_begin_ID].isEmpty())
                 return false;
             return true;

       }

int  UserBuffer::ReturnDatabaseQueueSize()
{
    return m_database_queue.size();
}


/*
 * �ͻ��� ���ݶ��� ������
 */

void UserBuffer::PushClientServerQueue(QString const& strData)
{
    boost::mutex::scoped_lock lock( m_mutex_client);
               m_clientServer_queue.enqueue(strData);
               m_cond_client.notify_all();
}

QString UserBuffer::PopClientServerQueue()
{

           boost::mutex::scoped_lock lock( m_mutex_client);
          if( m_clientServer_queue.isEmpty())
              m_cond_client.wait(m_mutex_client);
           return  m_clientServer_queue.dequeue();
}

int UserBuffer::ReturnClientServerQueueSize()
{
    return m_clientServer_queue.size();
}

bool     UserBuffer::PushUserDLQueue(QStringList const& list)
{
    boost::mutex::scoped_lock lock( m_mutex_userDL);
    m_userDL_queue.enqueue(list);
	return true;
}

QStringList    UserBuffer::PopUserDLQueue()
{
        boost::mutex::scoped_lock lock( m_mutex_userDL);
        if(m_userDL_queue.isEmpty())
            m_cond_userDL.wait(m_mutex_userDL);
       return  m_userDL_queue.dequeue();
}
/*
 * ����˻ظ� Ӧ�� �������
 *
 */
/*
 * ����˻ظ� Ӧ�� �������
 */
//QQueue<int>     m_server_response;
//boost::mutex    m_server_response_mutex;

void    UserBuffer::PushServerResponse(int const& res)
{
    boost::mutex::scoped_lock lock(m_server_response_mutex);
    m_server_response.clear();
    m_server_response.push_back(res);
}
//0 ʧ�� 1 �ɹ�
int     UserBuffer::PopServerResponse()
{
    int ret =-2;
    boost::mutex::scoped_lock lock(m_server_response_mutex);
    if(m_server_response.isEmpty())
        ret = -1;
    else
    {
        ret = m_server_response.front();
        m_server_response.pop_front();
    }

    return ret;
}
