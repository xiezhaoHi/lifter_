#include "server.h"
#include"ccanmoduleserver.h"
#include"device_control/devicethreadcontrol.h"
Myserver* Myserver::m_singleton;



Myserver::~Myserver()
{
    if(m_singleton)
        delete m_singleton;
}

void Myserver::handle_accept(boost::shared_ptr<Mysession>& new_session, const boost::system::error_code& error)
{

    if (!acceptor_.is_open()) {
              return;
          }

    if (!error)
    {
        qDebug() << QString("server:ip = ") << new_session->getsessionIp(1) << QString("server:port =") << new_session->getsessionPort(1);
        QString strIp =new_session->getsessionIp() ;
        int intPort = new_session->getsessionPort();
         qDebug() << QString("client:IP =") << strIp<< QString("client:port =") << intPort;
        m_connections.Add(new_session);
        //记录用户登录信息
        recordUserDL(strIp,QString("%1").arg(intPort));
//                      if(!m_connections.Add(new_session))  /*用户登录,记录信息*/
//                      {
//                          //ip 未注册 过滤
//                          new_session->socket().shutdown(tcp::socket::shutdown_both);
//                          new_session->socket().close();
//                          qDebug() << "该用户未注册,断开连接,过滤!";
//                          return;
//                       }


        new_session->start();
    }

   shared_ptr<Mysession> pnew_session(new Mysession(io_service_pool_.get_io_service(),m_connections));
   //m_connections.Add(pnew_session);
    acceptor_.async_accept(pnew_session->socket(),
        boost::bind(&Myserver::handle_accept, this, pnew_session, boost::asio::placeholders::error));
}

void Myserver::run_server()
{
    io_service_pool_.start();
    io_service_pool_.join();
    /*
     * 开启设备读写服务
     */
    DeviceThreadControl::GetInstance()->StartThreads();
   // startCANMod();// CAN数据收集模块

}

/*
 * 停止服务
 */
void Myserver::stop_server()
{

        m_connections.CloseAll();
       // startCANMod(0);
        io_service_pool_.stop();
        DeviceThreadControl::GetInstance()->StopThreads();

}
Myserver* Myserver::getInstance(short port ,  int thread_cnt )
{
    if(NULL == m_singleton && port != -1 && thread_cnt != -1)
    {
        m_singleton = new Myserver(port,thread_cnt);
    }
    return m_singleton;
}




bool     Myserver::writeToDevice(QString strIp,char* buff, int len)
{
    if(m_connections.m_device_map.contains(strIp))
    {
        m_connections.m_device_map[strIp]->sendData(buff,len);
    }
    return false;
}




/*
 * 登入信息记录
 */
void    Myserver::recordUserDL(QString const& strIp, QString const& strPort,int flag)
{
        QStringList list;
        if(flag)
            list.append(QString("上线"));
        else
            list.append(QString("下线"));
        if(Config::GetInstance()->GetUserInfo().contains(strIp)) //设备
            list.append(QString("设备"));
        if(Config::GetInstance()->GetUserInfo(1).contains(strIp)) //client 端
            list.append(QString("client"));
        list.append(strIp);
        list.append(strPort);

        UserBuffer::GetInstance()->PushUserDLData(list);

        if(0 == flag)
        {
                QMap<QString,shared_ptr<Mysession>>::iterator
                        begin =m_connections.m_client_map.begin()
                        ,end = m_connections.m_client_map.end();

                for(; begin != end;++begin)
                {

                    //比较设备 绑定的client端 ip是否相等
                    //if(begin.key() == Config::GetInstance()->GetClientIpByDeviceIp(strIp))
                    {
                        QString strData = BusinessSession::GetInstance()->GetExitDevicePackg(strIp);
                        if(strData.isEmpty())
                            return ;
                        QString strClientIP;
                        QByteArray ba;
                        char *dataTemp ;
                        int     sizeTemp ;
                         ba = strData.toLatin1();
                        dataTemp = ba.data();
                         sizeTemp = strData.length();
                        strClientIP = begin.key();
                        if(m_connections.m_client_map[strClientIP]->socket().is_open())
                            begin.value()->sendData(dataTemp,sizeTemp);
                    }
        }
    }
}
