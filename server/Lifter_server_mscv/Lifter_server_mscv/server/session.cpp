#include "session.h"
#include"buffer/usebuffer.h"
#include<QDateTime>
Mysession::Mysession(boost::asio::io_service& io_service,Connections& con)
    : socket_(io_service)
    ,m_connections(con)
    ,strand_(io_service)
    ,timer_(io_service)
    ,is_read_(false)
    ,m_sendFlag(true)
    ,m_sendCount(0)

{
    m_stop_flag = false;
    m_sendSessionThread = NULL;
    memset(data_,0,MAX_PACKET_LEN);
    memset(jdq_buff,0,MAX_PACKET_LEN);

}


Mysession::~Mysession()
    {
       //boost::mutex::scoped_lock lock(cout_mtx);
    if(m_sendSessionThread)
    {
		m_sendSessionThread->stop();
		m_sendSessionThread->wait();
       }

    qDebug() << "~Mysession";
    }
/*处理回话*/
void Mysession::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        if(m_stop_flag)
            return;
#ifndef SESSION_TO_DEVICECONTROL
         // 设备关联的 client 登入后才 写数据到 缓冲区
         /*一. 设备信息处理
          * 1.设备信息 解析存储
          * 2.解析组包 分发给客户端
          */
        if(m_connections.m_device_map.contains(getsessionIp()) &&
                m_connections.m_client_map.contains(Config::GetInstance()->GetClientIpByDeviceIp(getsessionIp())))
       {
         //处理数据  返回一个数据包  分发给client
          QString strData =  BusinessSession::GetInstance()->InterfaceFun(
                    getsessionIp(),data_,bytes_transferred);

          if(!strData.isEmpty())
          {
                //qDebug() << "read1";
                //qDebug() << strData;
              QMap<QString,shared_ptr<Mysession>>::iterator
                      begin = m_connections.m_client_map.begin()
                      ,end = m_connections.m_client_map.end();
              for(; begin != end;++begin)
              {
                QString strClientIP =begin.key();
                QByteArray ba = strData.toLatin1();
                char *dataTemp = ba.data();
                int     sizeTemp = strData.length();
                if(m_connections.m_client_map.contains(strClientIP))
                {
                    if(m_connections.m_client_map[strClientIP]->socket().is_open())
                    boost::asio::async_write(m_connections.m_client_map[strClientIP]->socket()
                            , boost::asio::buffer(dataTemp,sizeTemp),
                    strand_.wrap( boost::bind(&Mysession::handle_write, shared_from_this(), boost::asio::placeholders::error)));

                }
              }
           }
        }

        /*
         * 二.客户端信息传入的 处理
         * 1.传入控制命令 ,解析 并 分发给指定设备
         *
         */
        else if(m_connections.m_client_map.contains(getsessionIp()))
#else
        if(m_connections.m_client_map.contains(getsessionIp()))
#endif
        {

            //解析从客户端发来的 数据包
            int packgCount = 0;
            if(BusinessSession::GetInstance()->DealClientPackg(QString(data_),getsessionIp()))
            {

                qDebug() << "正确处理pc客户端数据包!";
            }
            else
            {
                qDebug() << "错误处理pc客户端数据包!";
            }
        }

        //开启下一次读取

                 start();
    }
    else
    {
        qDebug() << error.value();
        CloseSocket();
    }

}


 void Mysession::start() //此时 双方 已建立连接, 开始会话
{
    /*会话分类
     *不同设备的会话
     * 需要做不同的处理
     *1.485 编码器 需要不断 发送指令
     * 2.传感器 不需要发送任何 信息
     *
     */
#ifndef SESSION_TO_DEVICECONTROL
#ifdef _MSG_
      Msg  msgTemp;
      memset(msgTemp.buff,0,MAX_PACKET_LEN);
      msgTemp.len = 0;
#endif
      char* buff = NULL;


      switch(Config::GetInstance()->GetDeviceFlag(getsessionIp())) //获取该ip 用户属性
    {
    case device_cgq: //传感器 设备

        break;
    case device_bmq: //旋转编码器设备
        ::Sleep(30);
        //03 功能命令 04 功能命令
        /*03 返回方向 角度 转速
         *04 返回计数值
         */

        if(BusinessSession::GetInstance()->getBmpFirst()) //表示读取第一条指令
            buff = buffbmq_1;
        else
            buff = buffbmq_2;
#ifdef _MSG_
         memcpy(msgTemp.buff,buff,device_bmq_data_len);
         msgTemp.len = device_bmq_data_len;
         deliver(1,msgTemp); //发送数据


#else
       boost::asio::async_write(socket_, boost::asio::buffer(buff,device_bmq_data_len),
        boost::bind(&Mysession::handle_write, shared_from_this(), boost::asio::placeholders::error));
#endif
        break;
    case device_jdq: //继电器 CC DD C0 01 00 00 0D CE 9C
//        qDebug() << "read3";
//        buff = buffjdq;
//        SendData msg;
//        memcpy(msg.buff,buffjdq,device_jdq_data_len);
//        msg.len = device_jdq_data_len;
//        UserBuffer::GetInstance()->PushBackSendList(msg);
//         sendJdqData();
        //qDebug() << QThread::currentThreadId();

#ifdef _MSG_

          if(!m_sendJdqThread)
          {
                m_sendJdqThread   = new SendThread();
                m_sendJdqThread->m_session = this;
                m_sendJdqThread->start();
           }
                return;
#endif

        break;
      default:
        //  HANDLE threadID = QThread::currentThreadId();

          break;
    }




#endif
      if(NULL == m_sendSessionThread)
      {
          m_sendIp = getsessionIp();
		  m_sendSessionThread = new SendThread();
		  m_sendSessionThread->m_session = this;
		  m_sendSessionThread->start();
      }

      socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
          strand_.wrap(boost::bind(&Mysession::handle_read, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)));

    //当客户端连接上10秒钟以后检测是否进行了通信
//    if(m_connections.m_device_map.contains(getsessionIp()))
//    {
//              timer_.expires_from_now(boost::posix_time::seconds(10));
//              timer_.async_wait(boost::bind(&Mysession::time_out, shared_from_this()));
//     }
}



void Mysession::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {

    }
    else
    {

    }
}


bool Mysession::sendNewestData()
{

    QString strRet = UserBuffer::GetInstance()->PopResponseQueue(m_sendIp);
    if(!strRet.isEmpty())
    {
        boost::system::error_code ec;
        QByteArray ba = strRet.toLatin1();
        char *dataTemp = ba.data();
        int     sizeTemp = strRet.length();
       int retlen = socket_.write_some(boost::asio::buffer(dataTemp,sizeTemp),ec);
        if(retlen != sizeTemp && !ec)
        {
         return false;
        }
    }
    QString strBelongs = Config::GetInstance()->GetLifterIDByClientIp(m_sendIp);
    QStringList list;
    //sc双笼 对应 两套设备 后期 优化,暂时没想到好设计
    if(strBelongs.left(4) == "0003")
    {
       list = UserBuffer::GetInstance()->ReadDeviceToClientMap(strBelongs);
       strBelongs = QString("0003%1").arg(strBelongs.right(4).toInt()+1,4,10,QLatin1Char('0'));
       QStringList strTemp = UserBuffer::GetInstance()->ReadDeviceToClientMap(strBelongs);
       for(int index = 0; index <strTemp.size(); ++index)
           list.append(strTemp[index]);
    }
    else
    {
       list = UserBuffer::GetInstance()->ReadDeviceToClientMap(strBelongs);
    }


    for(int index = 0; index < list.size(); ++index)
    {
        QByteArray ba = list[index].toLatin1();
        char *dataTemp = ba.data();
        int     sizeTemp = list[index].length();


        boost::system::error_code ec;
        int retlen = -1;

       retlen = socket_.write_some(boost::asio::buffer(dataTemp,sizeTemp),ec);
       if(retlen != sizeTemp && !ec)
       {
        return false;
       }
    }
	//QString strTestTime = QString("cgq_end_%1").arg(QTime::currentTime().toString(QString("HH:mm:ss:ms")));

   return true;
}


//对于双笼的 还待优化
bool Mysession::sendNewestDhData()
{
    QString strSendData;

    strSendData = UserBuffer::GetInstance()->ReadDeviceToDhData(UserBuffer::GetInstance()->ReadDhxzBuffer());
    if(strSendData.isEmpty())
        return true;
    QByteArray ba = strSendData.toLatin1();
    char *dataTemp = ba.data();
    int     sizeTemp = strSendData.length();


    boost::system::error_code ec;
    int retlen = -1;

   retlen = socket_.write_some(boost::asio::buffer(dataTemp,sizeTemp),ec);
   if(retlen != sizeTemp && !ec)
   {
    return false;
   }
   return true;
}
