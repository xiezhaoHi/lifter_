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
#include"server/businesssession.h" //����Ự �������ݵ� ͨ����
#include"server/config/config.h" //������Ϣ ���� �豸 ip ���豸��ʶ����Ϣ��
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
     * ����ָ������
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

    /*��ȡ�Ự��ip��ַ*/
    /*
     * flag 0 ��ʾ ��ȡԶ�̿ͻ�����Ϣ 1��ʾ��ȡ���ط������Ϣ
     */
    QString getsessionIp(int flag = 0)
    {
        if(0 == flag)
         return QString::fromStdString(socket_.remote_endpoint().address().to_string());
        else
            return QString::fromStdString(socket_.local_endpoint().address().to_string());
    }
    /*��ȡ�Ự��ip��ַ*/
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

     void start(); //��ʱ ˫�� �ѽ�������, ��ʼ�Ự



    void time_out()
        {
            //�ж�û�дӿͻ��˶�ȡ����Ϣ������¶Ͽ�����
            if(!is_read_) CloseSocket();
            else
            {//is_read_ == true��
            ////���ʱ���������ݵ����� �ǻ�Ծ����
            is_read_ = false;//�����������
            timer_.expires_from_now(boost::posix_time::seconds(10));
            timer_.async_wait(boost::bind(&Mysession::time_out, shared_from_this()));
           }
        }

    /*����ػ�*/
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);


    //�ر�socket ����
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

    void deliver(int flag,const Msg& msg) //��������  flag Ϊ0 ��Ϣ��ͷ 1 ��β
     {

        if(0 == msg.len)
            return;

       if(1 == flag)
            write_msgs_.push_back(msg); //����Ϣ�ҵ�write_msgs��β
       else
            write_msgs_.push_front(msg); //����Ϣ�ҵ�write_msgs��ͷ
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

    //��ʾ ���ƿͻ���
    bool sendNewestData();
    //������ʾ�ͻ���
    bool sendNewestDhData();

    boost::mutex    mutex_msg;
    QList<Msg> write_msgs_; //���Ͷ���
private:
    SendThread*  m_sendSessionThread; //�ͻ��˷��Ͷ���
    bool    m_stop_flag; //������־

    tcp::socket socket_;
      Connections&  m_connections;
       boost::asio::io_service::strand strand_;
         deadline_timer timer_;
          bool                    is_read_;

    char data_[MAX_PACKET_LEN];
    int recv_times;


  //�Ự ip
    QString m_sendIp;
    char m_sendBuff[64] ;
    int     m_sendLen;
    bool    m_sendFlag;
    int     m_sendCount;//�ط�����

    //�̵���
        char jdq_buff[MAX_PACKET_LEN];

};
/*
 * �ַ����ݵ��ͻ��˵��߳�
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
                //������ʾ �ͻ���
            if(m_client_ID.left(4) == QString("3001"))
            {
                if(!m_session->sendNewestData())
                {
                      break;
                }
            }
             //������ʾ�ͻ���
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
