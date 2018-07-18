#include "devicethreadcontrol.h"
#include<QDateTime>

#include"server/ccanmoduleserver.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define  USEUDP //使用UDP模式

//获取编码器 角度 转速
static char buffbmq_1[] ={(char)0x01 ,(char)0x03 ,(char)0x00  ,(char)0x78  ,(char)0x00  ,(char)0x04  ,(char)0xC4  ,(char)0x10};

//获取编码器 计数值
static char buffbmq_2[] ={(char)0x01 ,(char)0x04 ,(char)0x00  ,(char)0x40  ,(char)0x00  ,(char)0x02  ,(char)0x70  ,(char)0x1f};


//继电器 获取当前 开关量
static char buffjdq[] ={(char)0xCC ,(char)0xDD ,(char)0xC0  ,(char)0x01  ,(char)0x00  ,(char)0x00 ,(char)0x0D  ,(char)0xCE  ,(char)0x9c};

/////电源
//电压 ABC 3项的值
static char buffDyDy[] = { (char)0x01 ,(char)0x03 ,(char)0x03 ,(char)0xEB ,(char)0x00 ,(char)0x03 ,(char)0x75 ,(char)0xBB };

//电流 ABC 3项的值
static char buffDyDl[] = { (char)0x01,(char)0x03 ,(char)0x03 ,(char)0xEE ,(char)0x00 ,(char)0x06 ,(char)0xA5 ,(char)0xB9 };

//功率 ABC 3项的值
static char buffDyGl[] = { (char)0x01 ,(char)0x03 ,(char)0x03 ,(char)0xF4 ,(char)0x00 ,(char)0x06 ,(char)0x84 ,(char)0x7E };

DeviceThreadControl* DeviceThreadControl::m_singleton = NULL;
DeviceThreadControl::DeviceThreadControl()
{

}
DeviceThreadControl*& DeviceThreadControl::GetInstance()
{
    if(NULL == m_singleton)
        m_singleton = new DeviceThreadControl;
    return m_singleton;
}
/*
 * 传感器模块
 */
#define CGQ_THREAD
    /*
     * 继电器模块
     */
#define JDQ_THREAD
/*
 *bmq
 */
#define BMQ_THREAD

//电流 电压 传感器设备
#define DLDY_THREAD
void    DeviceThreadControl::StartThreads()
{
    /*
     * 继电器模块 DI DO 
     */
#ifdef JDQ_THREAD
    QMap<QString,ip_port>const & ipPortMap = Config::GetInstance()->GetTypeDeviceMap(device_jdq);
    if(!ipPortMap.isEmpty())
    {
        QMap<QString,ip_port>::const_iterator  begin = ipPortMap.begin(),end = ipPortMap.end();
        for(; begin != end;++begin)
        {
            boost::shared_ptr<JdqControlThread> p( new JdqControlThread(begin.value().m_strIp,begin.value().m_strPort));
            m_jdq_threads_map[begin.key()] = p;
            p->start();
        }
    }
#endif
    /*
     * 传感器模块  采集 角度 加速度
     */
#ifdef CGQ_THREAD
    QMap<QString,ip_port>const & ipPortMap_cgq = Config::GetInstance()->GetTypeDeviceMap(device_cgq);
    if(!ipPortMap_cgq.isEmpty())
    {
        QMap<QString,ip_port>::const_iterator  begin = ipPortMap_cgq.begin(),end = ipPortMap_cgq.end();
        for(; begin != end;++begin)
        {
            boost::shared_ptr<CgqControlThread> p( new CgqControlThread(begin.value().m_strIp,begin.value().m_strPort));
            m_cgq_threads_map[begin.key()] = p;
            p->start();
        }
    }
#endif

    /*
     *bmq  采集 方向 计数值
     */
#ifdef BMQ_THREAD
    QMap<QString,ip_port>const & ipPortMap_bmq = Config::GetInstance()->GetTypeDeviceMap(device_bmq);
    if(!ipPortMap_bmq.isEmpty())
    {
        QMap<QString,ip_port>::const_iterator  begin = ipPortMap_bmq.begin(),end = ipPortMap_bmq.end();
        for(; begin != end;++begin)
        {
            boost::shared_ptr<BmqControlThread> p( new BmqControlThread(begin.value().m_strIp,begin.value().m_strPort));
            m_bmq_threads_map[begin.key()] = p;
            p->start();
        }
    }
#endif

//电流 电压 传感器设备
#ifdef DLDY_THREAD
    QMap<QString,ip_port>const & ipPortMap_dydl = Config::GetInstance()->GetTypeDeviceMap(device_dydl);
    if(!ipPortMap_dydl.isEmpty())
    {
        QMap<QString,ip_port>::const_iterator  begin = ipPortMap_dydl.begin(),end = ipPortMap_dydl.end();
        for(; begin != end;++begin)
        {
            boost::shared_ptr<DydlControlThread> p( new DydlControlThread(begin.value().m_strIp,begin.value().m_strPort));
            m_dydl_threads_map[begin.key()] = p;
            p->start();
        }
    }
#endif
    /*
     * CAN 应力模块  采集应力
     */
    startCANMod(1);

}


/*
 * 停止所有设备的读写
 */
void   DeviceThreadControl::StopThreads()
{

    QMap<QString,boost::shared_ptr<JdqControlThread>>::iterator begin = m_jdq_threads_map.begin()
            ,end = m_jdq_threads_map.end();
    for(; begin != end; ++begin)
    {
        begin.value()->stop();
        begin.value()->quit();
        begin.value()->wait();
    }
    m_jdq_threads_map.clear();


    QMap<QString,boost::shared_ptr<CgqControlThread>>::iterator begin_cgq = m_cgq_threads_map.begin()
            ,end_cgq = m_cgq_threads_map.end();
    for(; begin_cgq != end_cgq; ++begin_cgq)
    {
        begin_cgq.value()->stop();
        begin_cgq.value()->quit();
        begin_cgq.value()->wait();
    }
    m_cgq_threads_map.clear();

    QMap<QString,boost::shared_ptr<BmqControlThread>>::iterator begin_bmq = m_bmq_threads_map.begin()
            ,end_bmq = m_bmq_threads_map.end();
    for(; begin_bmq != end_bmq; ++begin_bmq)
    {
        begin_bmq.value()->stop();
        begin_bmq.value()->quit();
        begin_bmq.value()->wait();
    }
    m_bmq_threads_map.clear();

    QMap<QString,boost::shared_ptr<DydlControlThread>>::iterator begin_dydl = m_dydl_threads_map.begin()
            ,end_dydl = m_dydl_threads_map.end();
    for(; begin_dydl != end_dydl; ++begin_dydl)
    {
        begin_dydl.value()->stop();
        begin_dydl.value()->quit();
        begin_dydl.value()->wait();
    }
    m_dydl_threads_map.clear();


    startCANMod(0);
}

DeviceThreadControl::~DeviceThreadControl()
{


}


/*
 * 应力CAN数据采集模块
 * flag 1 开启 采集
 * 0 停止采集
 */
bool     DeviceThreadControl::startCANMod(bool   flag)
{
    if(flag)
    {
        QMap<QString,CANDATA>&  configMap = Config::GetInstance()->GetCANConfigMap();
        QMap<QString,CANDATA>::iterator begin = configMap.begin(), end = configMap.end();
        for(int index = 0; begin != end; ++index,++begin)
        {
              if( !CCanModuleServer::GetInstance()->StartCANModule(index,begin.value()))
              {
                  qDebug() << "初始化CAN模块失败!" << begin.value().strIp;
                  return false;
              }
         }

        qDebug() << "初始化CAN模块设备成功!";
        return true;
   }
    else
    {
        CCanModuleServer::GetInstance()->StopCANModuel();
    }
    return true;
}





JdqControlThread::JdqControlThread(QString strIp, int iPort):m_stop(true)
{
    m_strIp = strIp;
    m_iPort = iPort;
    m_strID = Config::GetInstance()->GetDeviceID(m_strIp);
}


void JdqControlThread::run()
{
#ifdef TCP_ASIO
    // 所有asio类都需要io_service对象
     boost::asio::io_service iosev;
    // socket对象
     boost::asio::ip::tcp::socket socket(iosev);
    // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
     boost::asio::ip::tcp::endpoint ep(ip::address_v4::from_string(m_strIp.toStdString()),m_iPort);
    // 连接服务器
     boost::system::error_code ec;
    while(m_stop)
    {

         socket.connect(ep,ec);
        // 如果出错，打印出错信息
        if(ec)
         {
             qDebug() << QString("链接继电器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                     <<ec.value();
             Sleep(1000); //连接失败,则等待1秒钟
            continue;
         }
        while(m_stop)
        {
          if(!sendJdqData(socket))
          {
                socket.close();
                break;
          }
        }
       socket.shutdown(tcp::socket::shutdown_both,ec);
       if(ec)
        {
            qDebug() << QString("socket.shutdown链接继电器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                     << boost::system::system_error(ec).what() ;
        }
       socket.close();
    }
#else
    SOCKET clientsocket;
    SOCKADDR_IN serveraddr;
    SOCKADDR_IN clientaddr;
    char buf[1024];


    while(m_stop)
    {

        while(m_stop)
        {
            //创建套接字
            if((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
            {
                qDebug() <<QString("套接字socket创建失败!");
                msleep(300);
                continue;
            }
            break;
         }

        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(m_iPort);
        serveraddr.sin_addr.S_un.S_addr = inet_addr(m_strIp.toStdString().c_str());
        int error = -1;
        int len = sizeof(int);
        timeval tm;
        fd_set set;
        unsigned long ul = 1;
        ioctlsocket(clientsocket, FIONBIO, &ul); //设置为非阻塞模式
        bool ret = false;
        if( ::connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        {
            tm.tv_sec  = 3;
            tm.tv_usec = 0;
            FD_ZERO(&set);
            FD_SET(clientsocket, &set);
            if( select(clientsocket+1, NULL, &set, NULL, &tm) > 0)
            {
                getsockopt(clientsocket, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
                if(error == 0)
                    ret = true;
                else
                    ret = false;
            }
            else
                ret = false;
        }
        else
            ret = true;
        ul = 0;
        ioctlsocket(clientsocket, FIONBIO, &ul); //设置为阻塞模式
        if(!ret)
        {
            closesocket( clientsocket );
            qDebug() <<QString("继电器_ip=%1连接失败!_%2").arg(m_strIp).arg(GetLastError());
            msleep(3000);
            continue;

        }

        int nNetTimeout = SOCKET_WAIT_TIME; //1秒
        //发送时限
        setsockopt( clientsocket, SOL_SOCKET, SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
        //接收时限
        setsockopt( clientsocket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
        while(m_stop)
        {
          if(!sendJdqData(clientsocket))
          {
                closesocket(clientsocket);
                break;
          }

        }

     }
    closesocket(clientsocket);
    
#endif
}

void JdqControlThread::stop()
{
    m_stop  = false;
}
#ifdef TCP_ASIO
bool    JdqControlThread::sendJdqData(tcp::socket &socket_)
{

     int retlen = -1;
     SendData msg;
     const int buffReadLen = 4096;
     char buffRead[buffReadLen] = {0};
     QString str;
     boost::system::error_code ec;
   //  qDebug() << QThread::currentThreadId();


     /*指令 队列中有 指令 优先处理
      *
      * 处理后 再 发送状态 指令获取当期的状态
      *
      */
            if(!UserBuffer::GetInstance()->IsEmptySendList(m_strID) && !m_strID.isEmpty())
            {
                SendData& data = UserBuffer::GetInstance()->FrontSendList(m_strID);

                 retlen = socket_.write_some(boost::asio::buffer(data.buff,data.len),ec);
                if(retlen == data.len && !ec)
                    UserBuffer::GetInstance()->PopFrontSendList(m_strID);
                else
                {
                    qDebug() << QString("继电器采集write错误") << ec.value() << boost::system::system_error(ec).what();
                   return false;
                }

                retlen = socket_.read_some(boost::asio::buffer(buffRead,buffReadLen),ec);
                if(retlen >0  && !ec)
                {
//                    for(int index = 0;index < retlen; ++index)
//                        str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));
//                  //  qDebug() << "read" <<str;
//                    str = "";

                    //处理 返回值
                     BusinessSession::GetInstance()->InterfaceFun(
                              m_strIp,buffRead,retlen);
                }
                else
                {
                    qDebug() << QString("继电器采集read错误") << ec.value();
                    return false;
                }

             }

             ::Sleep(50);
                memcpy(msg.buff,buffjdq,device_jdq_data_len);
                msg.len = device_jdq_data_len;
                retlen = socket_.write_some(boost::asio::buffer(msg.buff,msg.len),ec);
               if(retlen != msg.len || ec)
               {
                   qDebug() << QString("继电器采集write错误") << ec.value() << boost::system::system_error(ec).what();
                  return false;
               }
               retlen = socket_.read_some(boost::asio::buffer(buffRead,buffReadLen),ec);
               if(retlen >0  && !ec)
               {
                   for(int index = 0;index < retlen; ++index)
                       str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));
                 //  qDebug() << "read" <<str;
                   str = "";

                   QDateTime current_date_time = QDateTime::currentDateTime();
                   QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
                   //qDebug() << current_time;

                   //处理 返回值
                    BusinessSession::GetInstance()->InterfaceFun(
                             m_strIp,buffRead,retlen);

//                   if(!strData.isEmpty())
//                   {
//                       QMap<QString,shared_ptr<Mysession>>::iterator
//                               begin = m_connections.m_client_map.begin()
//                               ,end = m_connections.m_client_map.end();
//                       for(; begin != end;++begin)
//                       {
//                         QString strClientIP =begin.key();
//                         QByteArray ba = strData.toLatin1();
//                         char *dataTemp = ba.data();
//                         int     sizeTemp = strData.length();
//                         if(m_connections.m_client_map.contains(strClientIP))
//                         {
//                             if(m_connections.m_client_map[strClientIP]->socket().is_open())
//                            retlen = m_connections.m_client_map[strClientIP]->socket().write_some(
//                                        boost::asio::buffer(dataTemp,sizeTemp),ec);
//                            if(retlen != sizeTemp)
//                            {
//                                qDebug() << QString("继电器采集write错误") << ec.value() << QString::fromLocal8Bit(boost::system::system_error(ec).what());
//                               return false;
//                            }
//                         }
//                       }
//                   }
               }
               else
               {
                   qDebug() << QString("继电器采集read错误") << ec.value();
                   return false;
               }

            return true;

    return true;
}

#else
bool   JdqControlThread::sendJdqData(SOCKET & clientsocket)
{
    int retlen = -1;
    SendData msg;
    const int buffReadLen = 4096;
    char buffRead[buffReadLen] = {0};
    QString str;
    boost::system::error_code ec;
  //  qDebug() << QThread::currentThreadId();


    /*指令 队列中有 指令 优先处理
     *
     * 处理后 再 发送状态 指令获取当期的状态
     *
     */
           if(!UserBuffer::GetInstance()->IsEmptySendList(m_strID) && !m_strID.isEmpty())
           {
               SendData& data = UserBuffer::GetInstance()->FrontSendList(m_strID);

               retlen=  send(clientsocket, data.buff, data.len, 0);

               if(retlen == data.len )
                   UserBuffer::GetInstance()->PopFrontSendList(m_strID);
               else
               {
                   qDebug() << QString("继电器采集write错误") ;
                  return false;
               }
			  // QTime t1;
			  // t1.start();
                retlen = recv(clientsocket, buffRead, buffReadLen, 0);
				//qDebug() << QString("jdq1_%1").arg(t1.elapsed());
               if(retlen >0)
               {

                    BusinessSession::GetInstance()->InterfaceFun(
                             m_strIp,buffRead,retlen);
               }
               else
               {
                   qDebug() << QString("继电器采集read错误");
                   return false;
               }

            }

            ::Sleep(50);

               memcpy(msg.buff,buffjdq,device_jdq_data_len);
               msg.len = device_jdq_data_len;
               retlen=  send(clientsocket, msg.buff, msg.len, 0);

              if(retlen != msg.len )
              {
                  qDebug() << QString("继电器采集write错误") ;
                 return false;
              }
			  //QTime t1;
			  //t1.start();
                retlen = recv(clientsocket, buffRead, buffReadLen, 0);
				//qDebug() << QString("jdq2_%1").arg(t1.elapsed());
              if(retlen >0 )
              {
                  for(int index = 0;index < retlen; ++index)
                      str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));

                  str = "";

                  QDateTime current_date_time = QDateTime::currentDateTime();
                  QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
                  //qDebug() << current_time;

                  //处理 返回值
                   BusinessSession::GetInstance()->InterfaceFun(
                            m_strIp,buffRead,retlen);


              }
              else
              {
                  qDebug() << QString("继电器采集read错误") ;
                  return false;
              }

           return true;

   return true;
}

#endif


CgqControlThread::CgqControlThread(QString strIp, int iPort):m_stop(true)
{
    m_strIp = strIp;
    m_iPort = iPort;

}

#ifdef TCP_ASIO
/*
 * 给传感器 接受数据
 */
bool   CgqControlThread::sendCgqData(tcp::socket& socket_)
{

    int retlen = -1;

    const int buffReadLen = 4096;
    char buffRead[buffReadLen] = {0};

    boost::system::error_code ec;

      retlen = socket_.read_some(boost::asio::buffer(buffRead,buffReadLen),ec);
      if(retlen >0  && !ec)
      {
          //    QString str;
//          for(int index = 0;index < retlen; ++index)
//              str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));
//        //  qDebug() << "read" <<str;
//          str = "";

         // QDateTime current_date_time = QDateTime::currentDateTime();
          //QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
          //qDebug() << current_time;

          //处理 返回值
          BusinessSession::GetInstance()->InterfaceFun(
                    m_strIp,buffRead,retlen);

      }
      else
      {
          qDebug() << QString("传感器采集read错误") << ec.value();
          return false;
      }

   return true;
}
#else
 bool   CgqControlThread::sendCgqData(SOCKET & clientsocket)
 {
     int retlen = -1;

     const int buffReadLen = 4096;
     char buffRead[buffReadLen] = {0};
	// QTime t1;
	// t1.start();

     retlen = recv(clientsocket, buffRead, buffReadLen, 0);

	 // setsockopt(clientsocket, IPPROTO_TCP, TCP_QUICKACK, (int[]) { 1 }, sizeof(int));
	// qDebug() << QString("cgq2_%1").arg(t1.elapsed());
       if(retlen >0 )
       {
           //    QString str;
 //          for(int index = 0;index < retlen; ++index)
 //              str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));
 //        //  qDebug() << "read" <<str;
 //          str = "";

          // QDateTime current_date_time = QDateTime::currentDateTime();
           //QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
           //qDebug() << current_time;

		   //QTime t2;
		   //t2.start();
           //处理 返回值
           BusinessSession::GetInstance()->InterfaceFun(
                     m_strIp,buffRead,retlen);
		   //qDebug() << QString("cgq4_%1").arg(t2.elapsed());
       }
       else
       {
           qDebug() << QString("传感器采集read错误") ;
           return false;
       }

    return true;
 }

#endif

/*
 * 传感器 异步接受数据
 */

void CgqControlThread::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        return;
    }
    int retlen = bytes_transferred;
    if(retlen >0 )
    {

//        for(int index = 0;index < retlen; ++index)
//            str += QString("%1").arg(buffRead[index]&0xFF,2,16,QLatin1Char('0'));
//      //  qDebug() << "read" <<str;
//        str = "";

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_time = current_date_time.toString("hh:mm:ss.zzz ");


        //处理 返回值
        BusinessSession::GetInstance()->InterfaceFun(
                  m_strIp,m_cgq_buff,retlen);


        boost::asio::async_read(*m_psocket,  boost::asio::buffer(m_cgq_buff,CGQ_BUFFSIZE),
                    boost::bind(&CgqControlThread::handle_read,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
}

bool   CgqControlThread::ansy_sendCgqData(tcp::socket& socket_)
{

    boost::asio::async_read(socket_,  boost::asio::buffer(m_cgq_buff,CGQ_BUFFSIZE),
                boost::bind(&CgqControlThread::handle_read,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    m_psocket = &socket_;
    return true;
}

void CgqControlThread::run()
{



#ifdef TCP_ASIO
    // 所有asio类都需要io_service对象
     boost::asio::io_service iosev;
    // socket对象
     boost::asio::ip::tcp::socket socket(iosev);
    // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
     boost::asio::ip::tcp::endpoint ep(ip::address_v4::from_string(m_strIp.toStdString()),m_iPort);
    // 连接服务器
    boost::system::error_code ec;

    while(m_stop)
    {
         socket.connect(ep,ec);
        // 如果出错，打印出错信息
        if(ec)
         {
            //boost::system::system_error(ec).what()
             qDebug() << QString("链接传感器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                       <<ec.value();
             Sleep(1000); //连接失败,则等待1秒钟
            continue;
         }
        while(m_stop)
        {
          if(!sendCgqData(socket))
          {
                socket.close();
                break;
          }

        }
        socket.shutdown(tcp::socket::shutdown_both,ec);
        if(ec)
         {
             qDebug() << QString("socket.shutdown链接继传感器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                      << boost::system::system_error(ec).what() ;
         }
        socket.close();

    }

#else
    SOCKET clientsocket;
    SOCKADDR_IN serveraddr;
    SOCKADDR_IN clientaddr;
    char buf[1024];

 

	const int buffReadLen = 4096;
	char buffRead[buffReadLen] = { 0 };

    while(m_stop)
    {

        while(m_stop)
        {
#ifdef USEUDP
			//创建套接字
			if ((clientsocket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
			{
				qDebug() << QString("套接字socket创建失败!");
				msleep(300);
				continue;
			}
			break;
#else

            //创建套接字
            if((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
            {
                qDebug() <<QString("套接字socket创建失败!");
                msleep(300);
                continue;
            }
            break;

#endif
         }
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(m_iPort);
		serveraddr.sin_addr.S_un.S_addr = inet_addr(m_strIp.toStdString().c_str());
		int nNetTimeout = SOCKET_WAIT_TIME; //1秒
											//发送时限
		setsockopt(clientsocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
		//接收时限
		setsockopt(clientsocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));


#ifdef USEUDP

		memset(buffRead, 0, buffReadLen);
		int len = sizeof(serveraddr);
		if (sendto(clientsocket, buffRead, sizeof(buffRead), 0
			, (struct sockaddr*)&serveraddr, len) != SOCKET_ERROR)
		{

			while (m_stop)
			{
				//QTime t;
				//t.start();
				len = recvfrom(clientsocket, buffRead, sizeof(buffRead), 0
					, (struct sockaddr*)&serveraddr, &len);
				if (len == SOCKET_ERROR)
				{
					closesocket(clientsocket);
					break;
				}
				BusinessSession::GetInstance()->InterfaceFun(
					m_strIp, buffRead, len);
				//qDebug() << QString("cgq_%1").arg(t.elapsed());
			}
		}
		
#else

	

		int error = -1;
        int len = sizeof(int);
        timeval tm;
        fd_set set;
        unsigned long ul = 1;
        ioctlsocket(clientsocket, FIONBIO, &ul); //设置为非阻塞模式
        bool ret = false;
        if( ::connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        {
            tm.tv_sec  = 3;
            tm.tv_usec = 0;
            FD_ZERO(&set);
            FD_SET(clientsocket, &set);
            if( select(clientsocket+1, NULL, &set, NULL, &tm) > 0)
            {
                getsockopt(clientsocket, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
                if(error == 0)
                    ret = true;
                else
                    ret = false;
            }
            else
                ret = false;
        }
        else
            ret = true;
        ul = 0;
        ioctlsocket(clientsocket, FIONBIO, &ul); //设置为阻塞模式
        if(!ret)
        {
            closesocket( clientsocket );
            qDebug() <<QString("传感器_ip=%2连接失败!_%1").arg(GetLastError()).arg(m_strIp);
            msleep(3000);
            continue;

        }

        int nNetTimeout = SOCKET_WAIT_TIME; //1秒
        //发送时限
        setsockopt( clientsocket, SOL_SOCKET, SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
        //接收时限
        setsockopt( clientsocket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
      
		int enable = 1;
		setsockopt(clientsocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));

		while(m_stop)
        {
			QTime t;
			t.start();
          if(!sendCgqData(clientsocket))
          {
                closesocket(clientsocket);
                break;
          }
		  qDebug() << QString("cgq_%1").arg(t.elapsed());
        }

    
#endif
	}
    closesocket(clientsocket);
    
#endif
}

void CgqControlThread::stop()
{
     m_stop  = false;
}

BmqControlThread::BmqControlThread(QString strIp,int iPort)
{
    m_strIp = strIp;
    m_iPort = iPort;
}


void BmqControlThread::run()
{
#ifdef TCP_ASIO
    // 所有asio类都需要io_service对象
     boost::asio::io_service iosev;
    // socket对象
     boost::asio::ip::tcp::socket socket(iosev);
    // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
     boost::asio::ip::tcp::endpoint ep(ip::address_v4::from_string(m_strIp.toStdString()),m_iPort);
    // 连接服务器
    boost::system::error_code ec;
    while(m_stop)
    {
         socket.connect(ep,ec);
        // 如果出错，打印出错信息
        if(ec)
         {

             qDebug() << QString("链接编码器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                    <<ec.value();
             Sleep(1000); //连接失败,则等待1秒钟
            continue;
         }
        while(m_stop)
        {
          if(!sendBmqData(socket))
          {
                socket.close();
                break;
          }
        }
        socket.shutdown(tcp::socket::shutdown_both,ec);
        if(ec)
         {
             qDebug() << QString("socket.shutdown链接编码器失败Ip=%1,port=%2").arg(m_strIp).arg(m_iPort)
                      << boost::system::system_error(ec).what() ;
         }
        socket.close();

    }
#else
    SOCKET clientsocket;
    SOCKADDR_IN serveraddr;



    while(m_stop)
    {

        while(m_stop)
        {
            //创建套接字
            if((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
            {
                qDebug() <<QString("套接字socket创建失败!");
                msleep(300);
                continue;
            }
            break;
         }

        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(m_iPort);
        serveraddr.sin_addr.S_un.S_addr = inet_addr(m_strIp.toStdString().c_str());
            int error = -1;
            int len = sizeof(int);
            timeval tm;
            fd_set set;
            unsigned long ul = 1;
            ioctlsocket(clientsocket, FIONBIO, &ul); //设置为非阻塞模式
            bool ret = false;
            if( ::connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
            {
                tm.tv_sec  = 3;
                tm.tv_usec = 0;
                FD_ZERO(&set);
                FD_SET(clientsocket, &set);
                if( select(clientsocket+1, NULL, &set, NULL, &tm) > 0)
                {
                    getsockopt(clientsocket, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
                    if(error == 0)
                        ret = true;
                    else
                        ret = false;
                }
                else
                    ret = false;
            }
            else
                ret = true;
            ul = 0;
            ioctlsocket(clientsocket, FIONBIO, &ul); //设置为阻塞模式
            if(!ret)
            {
                closesocket( clientsocket );
                qDebug() <<QString("编码器_ip=%2连接失败!_%1").arg(GetLastError()).arg(m_strIp);
                msleep(3000);
                continue;

            }

        int nNetTimeout = SOCKET_WAIT_TIME; //1秒
        //发送时限
        setsockopt( clientsocket, SOL_SOCKET, SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
        //接收时限
        setsockopt( clientsocket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
       
	
		while(m_stop)
        {
          if(!sendBmqData(clientsocket))
          {
                closesocket(clientsocket);
				Sleep(3000); //读写错误 等待3秒
                break;
          }

        }

     }
    closesocket(clientsocket);
    
#endif
}
void BmqControlThread::stop()
{
    m_stop  = false;
}

#ifdef TCP_ASIO
/*
 * 给编码器发送指令
 */
bool   BmqControlThread::sendBmqData(tcp::socket& socket_)
{

    /*03 返回方向 角度 转速
     *04 返回计数值
     */
    char* buff = NULL;
    int  dataLen = 0;
    if(BusinessSession::GetInstance()->getBmqFirst(m_strIp)) //表示读取第一条指令
    {
        buff = buffbmq_1;
        dataLen = BMQPACKAGE_ONESIZE;
    }
    else
    {
        buff = buffbmq_2;
        dataLen = BMQPACKAGE_TWOSIZE;
    }

    const int buffReadLen = 256;
    int retlen = -1;
    char buffRead[buffReadLen] = {0};
    QString str;
    boost::system::error_code ec;



    retlen = socket_.write_some(boost::asio::buffer(buff,BMQPACKAGE_SENDSIZE),ec);
   if(retlen == BMQPACKAGE_SENDSIZE && !ec)
   {
       retlen = socket_.read_some(boost::asio::buffer(buffRead,buffReadLen),ec);
       if(retlen == dataLen  && !ec)
       {
          QString strRet = BusinessSession::GetInstance()->InterfaceFun(
                     m_strIp,buffRead,retlen);
          return true;
       }
   }
   else
   {
       qDebug() << QString("继电器采集write错误") << ec.value() << boost::system::system_error(ec).what();
      return false;
   }
   return false;
}

#else
bool   BmqControlThread::sendBmqData(SOCKET &clientsocket )
{

    /*03 返回方向 角度 转速
     *04 返回计数值
     */
    char* buff = NULL;
    int  dataLen = 0;
    if(BusinessSession::GetInstance()->getBmqFirst(m_strIp)) //表示读取第一条指令
    {
        buff = buffbmq_1;
        dataLen = BMQPACKAGE_ONESIZE;
    }
    else
    {
        buff = buffbmq_2;
        dataLen = BMQPACKAGE_TWOSIZE;
    }

    const int buffReadLen = 256;
    int retlen = -1;
    char buffRead[buffReadLen] = {0};
    QString str;

   retlen=  send(clientsocket, buff, BMQPACKAGE_SENDSIZE, 0);

   if(retlen == BMQPACKAGE_SENDSIZE)
   {
	  // QTime t1;
	   //t1.start();
       retlen = recv(clientsocket, buffRead, buffReadLen, 0);
	   //qDebug() << QString("bmq_%1").arg(t1.elapsed());
       if(retlen == dataLen )
       {
          QString strRet = BusinessSession::GetInstance()->InterfaceFun(
                     m_strIp,buffRead,retlen);
          return true;
       }
   }
   else
   {
       qDebug() << QString("继电器采集write错误_err:%1").arg(GetLastError()) ;
      return false;
   }
   return false;
}

#endif

DydlControlThread::DydlControlThread(QString strIp,int iPort)
{
	m_strIp = strIp;
	m_iPort = iPort;
	m_strID = Config::GetInstance()->GetDeviceID(m_strIp);
}


void DydlControlThread::run()
{

	SOCKET clientsocket;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	char buf[1024];


	while (m_stop)
	{

		while (m_stop)
		{
			//创建套接字
			if ((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
			{
				qDebug() << QString("套接字socket创建失败!");
				msleep(300);
				continue;
			}
			break;
		}

		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(m_iPort);
		serveraddr.sin_addr.S_un.S_addr = inet_addr(m_strIp.toStdString().c_str());
		int error = -1;
		int len = sizeof(int);
		timeval tm;
		fd_set set;
		unsigned long ul = 1;
		ioctlsocket(clientsocket, FIONBIO, &ul); //设置为非阻塞模式
		bool ret = false;
		if (::connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
		{
			tm.tv_sec = 3;
			tm.tv_usec = 0;
			FD_ZERO(&set);
			FD_SET(clientsocket, &set);
			if (select(clientsocket + 1, NULL, &set, NULL, &tm) > 0)
			{
				getsockopt(clientsocket, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
				if (error == 0)
					ret = true;
				else
					ret = false;
			}
			else
				ret = false;
		}
		else
			ret = true;
		ul = 0;
		ioctlsocket(clientsocket, FIONBIO, &ul); //设置为阻塞模式
		if (!ret)
		{
			closesocket(clientsocket);
			qDebug() << QString("电压电流_ip=%1连接失败!_%2").arg(m_strIp).arg(GetLastError());
			msleep(3000);
			continue;

		}

		int nNetTimeout = SOCKET_WAIT_TIME; //1秒
											//发送时限
		setsockopt(clientsocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
		//接收时限
		setsockopt(clientsocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		while (m_stop)
		{
			if (!sendDydlData(clientsocket))
			{
				closesocket(clientsocket);
				break;
			}

		}

	}
	closesocket(clientsocket);

}

void DydlControlThread::stop()
{
	m_stop = false;
}

bool   DydlControlThread::sendDydlData(SOCKET & clientsocket)
{
	int retlen = -1;
	SendData msg;
	const int buffReadLen = 4096;
	char buffRead[buffReadLen] = { 0 };
	QString str;
	boost::system::error_code ec;
	//  qDebug() << QThread::currentThreadId();


	/*指令 队列中有 指令 优先处理
	*
	* 处理后 再 发送状态 指令获取当期的状态
	*
	*/
	if (!UserBuffer::GetInstance()->IsEmptySendList(m_strID) && !m_strID.isEmpty())
	{
		SendData& data = UserBuffer::GetInstance()->FrontSendList(m_strID);

		retlen = send(clientsocket, data.buff, data.len, 0);

		if (retlen == data.len)
			UserBuffer::GetInstance()->PopFrontSendList(m_strID);
		else
		{
			qDebug() << QString("电源设备write错误");
			return false;
		}
		retlen = recv(clientsocket, buffRead, buffReadLen, 0);
	
		if (retlen >0)
		{

			BusinessSession::GetInstance()->InterfaceFun(
				m_strIp, buffRead, retlen, DY_DATA_FLAG_RET);
		}
		else
		{
			qDebug() << QString("电源设备采集read错误");
			return false;
		}

	}

	::Sleep(50);

	//电源 电压 3项
	{

		memcpy(msg.buff, buffDyDy, device_dydl_data_len);
		msg.len = device_dydl_data_len;
		retlen = send(clientsocket, msg.buff, msg.len, 0);

		if (retlen != msg.len)
		{
			qDebug() << QString("电源采集write错误");
			return false;
		}

		retlen = recv(clientsocket, buffRead, buffReadLen, 0);

		if (retlen > 0)
		{
			for (int index = 0; index < retlen; ++index)
				str += QString("%1").arg(buffRead[index] & 0xFF, 2, 16, QLatin1Char('0'));

			str = "";

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
			//qDebug() << current_time;

			//处理 返回值
			BusinessSession::GetInstance()->InterfaceFun(
				m_strIp, buffRead, retlen, DY_DATA_FLAG_DY);
		}
		else
		{
			qDebug() << QString("继电器采集read错误");
			return false;
		}

	}
	
	//电源 电流 3项
	{
		memcpy(msg.buff, buffDyDl, device_dydl_data_len);
		msg.len = device_dydl_data_len;
		retlen = send(clientsocket, msg.buff, msg.len, 0);

		if (retlen != msg.len)
		{
			qDebug() << QString("电源采集write错误");
			return false;
		}

		retlen = recv(clientsocket, buffRead, buffReadLen, 0);

		if (retlen > 0)
		{
			for (int index = 0; index < retlen; ++index)
				str += QString("%1").arg(buffRead[index] & 0xFF, 2, 16, QLatin1Char('0'));

			str = "";

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
			//qDebug() << current_time;

			//处理 返回值
			BusinessSession::GetInstance()->InterfaceFun(
				m_strIp, buffRead, retlen, DY_DATA_FLAG_DL);
		}
		else
		{
			qDebug() << QString("继电器采集read错误");
			return false;
		}
	}
	
	//电源  功率 3项
	{
		memcpy(msg.buff, buffDyGl, device_dydl_data_len);
		msg.len = device_dydl_data_len;
		retlen = send(clientsocket, msg.buff, msg.len, 0);

		if (retlen != msg.len)
		{
			qDebug() << QString("电源采集write错误");
			return false;
		}

		retlen = recv(clientsocket, buffRead, buffReadLen, 0);

		if (retlen > 0)
		{
			for (int index = 0; index < retlen; ++index)
				str += QString("%1").arg(buffRead[index] & 0xFF, 2, 16, QLatin1Char('0'));

			str = "";

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
			//qDebug() << current_time;

			//处理 返回值
			BusinessSession::GetInstance()->InterfaceFun(
				m_strIp, buffRead, retlen,DY_DATA_FLAG_GL);
		}
		else
		{
			qDebug() << QString("电源采集read错误");
			return false;
		}
	}


	return true;

	
}
