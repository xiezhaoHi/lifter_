#ifndef SESSION_H
#define SESSION_H

#include<QMap>
#include<QDebug>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost\bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include<QThread>
#include<QFile>
#include<QTextStream>
#include<QQueue>
#include"server/businesssession.h" //处理会话 返回数据的 通用类
#include"server/config/config.h" //配置信息 包含 设备 ip 与设备标识的信息表
#include"connections.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include<QThread>

using boost::asio::ip::tcp;
using namespace boost;
using namespace boost::asio;
enum {MAX_PACKET_LEN = 4096};
#define _MSG_

typedef struct  Message
{
    char buff[MAX_PACKET_LEN];
    int len;
}Msg;





class SendThread;


//class Connections;
class Mysession:public boost::enable_shared_from_this<Mysession>
{
public:
    Mysession(boost::asio::io_service& io_service,Connections& con);


    virtual ~Mysession();

    /*
     * 发送指定数据
     */
    bool    sendData(char * buff,int strLen)
    {
       // HANDLE threadID = QThread::currentThreadId();
        boost::asio::async_write(socket_,
            boost::asio::buffer(buff,
              strLen),
           strand_.wrap( boost::bind(&Mysession::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));

        return true;
    }

    /*获取会话的ip地址*/
    /*
     * flag 0 表示 获取远程客户端信息 1表示获取本地服务端信息
     */
    QString getsessionIp(int flag = 0)
    {
        if(0 == flag)
         return QString::fromStdString(socket_.remote_endpoint().address().to_string());
        else
            return QString::fromStdString(socket_.local_endpoint().address().to_string());
    }
    /*获取会话的ip地址*/
    int getsessionPort(int flag = 0)
    {
         if(0 == flag)
            return socket_.remote_endpoint().port();
          else
              return socket_.local_endpoint().port();
    }
    tcp::socket& socket()
    {
        return socket_;
    }

     void start(); //此时 双方 已建立连接, 开始会话



    void time_out()
        {
            //判断没有从客户端读取到消息的情况下断开连接
            if(!is_read_) CloseSocket();
            else
            {//is_read_ == true。
            ////这段时间内有数据到来， 是活跃连接
            is_read_ = false;//重置这个。。
            timer_.expires_from_now(boost::posix_time::seconds(10));
            timer_.async_wait(boost::bind(&Mysession::time_out, shared_from_this()));
           }
        }

    /*处理回话*/
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);


    //关闭socket 连接
    void CloseSocket()
    {

        if(m_stop_flag)
            return;
        m_stop_flag = true;
        if(socket_.is_open())
        {

             strand_.post(bind(&Mysession::do_close, shared_from_this()));
            m_connections.Remove(shared_from_this());

       }

    }

    void do_close()
      {
          socket_.shutdown(tcp::socket::shutdown_both);
         socket_.close();

      }

    void deliver(int flag,const Msg& msg) //发送数据  flag 为0 消息放头 1 放尾
     {

        if(0 == msg.len)
            return;

       if(1 == flag)
            write_msgs_.push_back(msg); //把消息挂到write_msgs队尾
       else
            write_msgs_.push_front(msg); //把消息挂到write_msgs队头
    bool write_in_progress = write_msgs_.empty();
       if (!write_in_progress)
       {
         //::Sleep(50);
         boost::asio::async_write(socket_,
             boost::asio::buffer(write_msgs_.front().buff,
               write_msgs_.front().len),
            strand_.wrap( boost::bind(&Mysession::handle_write, shared_from_this(),
               boost::asio::placeholders::error)));

       }
     }

    //显示 控制客户端
    bool sendNewestData();
    //动画显示客户端
    bool sendNewestDhData();

    boost::mutex    mutex_msg;
    QList<Msg> write_msgs_; //发送队列
private:
    SendThread*  m_sendSessionThread; //客户端发送队列
    bool    m_stop_flag; //结束标志

    tcp::socket socket_;
      Connections&  m_connections;
       boost::asio::io_service::strand strand_;
         deadline_timer timer_;
          bool                    is_read_;

    char data_[MAX_PACKET_LEN];
    int recv_times;


  //会话 ip
    QString m_sendIp;
    char m_sendBuff[64] ;
    int     m_sendLen;
    bool    m_sendFlag;
    int     m_sendCount;//重发次数

    //继电器
        char jdq_buff[MAX_PACKET_LEN];

};
/*
 * 分发数据到客户端的线程
 */
class SendThread:public QThread
{
public:
    SendThread():m_session(NULL),m_stop(true)
    {


    }


    void run()
    {
		if(m_session != NULL)
			m_client_ID =  Config::GetInstance()->GetClientIDByIp(m_session->getsessionIp());
        while(m_stop)
        {
            if(m_session != NULL)
           {
                //控制显示 客户端
            if(m_client_ID.left(4) == QString("3001"))
            {
                if(!m_session->sendNewestData())
                {
                      break;
                }
            }
             //动画显示客户端
            if(m_client_ID.left(4) == QString("3002"))
            {
                if(!m_session->sendNewestDhData())
                {
                      break;
                }
            }

              Sleep(DISPENSE_TIMES);
            }

        }
         //m_session->CloseSocket();
    }

    void stop()
    {
        m_stop  = false;
    }
     Mysession*  m_session;

private:
    QString m_client_ID;
volatile  bool    m_stop;

};
#endif // SESSION_H
