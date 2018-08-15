#ifndef ANSYCSERVICE_H
#define ANSYCSERVICE_H

#include"usebuffer.h" //缓冲区
#include"dataexchange.h"


#include<QMap>
#include<QDebug>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include<QThread>
#include<QFile>
#include<QTextStream>
#include<QQueue>
#include"server/businesssession.h" //处理会话 返回数据的 通用类
#include"server/config/config.h" //配置信息 包含 设备 ip 与设备标识的信息表
using namespace std;
using boost::asio::ip::tcp;

class io_service_pool
    : public boost::noncopyable
{
public:

    explicit io_service_pool(std::size_t pool_size)
        : next_io_service_(0)
    {
        for (std::size_t i = 0; i < pool_size; ++ i)
        {
            io_service_sptr io_service(new boost::asio::io_service);
            work_sptr work(new boost::asio::io_service::work(*io_service));
            io_services_.push_back(io_service);
            work_.push_back(work);
        }
    }

    void start()
    {
        for (std::size_t i = 0; i < io_services_.size(); ++ i)
        {
            boost::shared_ptr<boost::thread> thread(new boost::thread(
                boost::bind(&boost::asio::io_service::run, io_services_[i])));
            threads_.push_back(thread);
        }
    }

    void join()
    {
        for (std::size_t i = 0; i < threads_.size(); ++ i)
        {
          //  threads_[i]->join();
             threads_[i]->detach();
        }
    }

    void stop()
    {
        for (std::size_t i = 0; i < io_services_.size(); ++ i)
        {
            io_services_[i]->stop();

        }
    }

    boost::asio::io_service& get_io_service()
    {
        boost::mutex::scoped_lock lock(mtx);
        boost::asio::io_service& io_service = *io_services_[next_io_service_];
        ++ next_io_service_;
        if (next_io_service_ == io_services_.size())
        {
            next_io_service_ = 0;
        }
        return io_service;
    }

private:
    typedef boost::shared_ptr<boost::asio::io_service> io_service_sptr;
    typedef boost::shared_ptr<boost::asio::io_service::work> work_sptr;
    typedef boost::shared_ptr<boost::thread> thread_sptr;

    boost::mutex mtx;

    std::vector<io_service_sptr> io_services_;
    std::vector<work_sptr> work_;
    std::vector<thread_sptr> threads_;
    std::size_t next_io_service_;
};

boost::mutex cout_mtx;
int packet_size = 0;
enum {MAX_PACKET_LEN = 4096};

class session:public boost::enable_shared_from_this<session>
{
public:
    session(boost::asio::io_service& io_service)
        : socket_(io_service)
        , recv_times(0)
    {
        memset(data_,0,MAX_PACKET_LEN);
    }

    virtual ~session()
    {
       boost::mutex::scoped_lock lock(cout_mtx);
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
    short getsessionPort(int flag = 0)
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

    inline void start() //此时 双方 已建立连接, 开始会话
    {
        /*会话分类
         *不同设备的会话
         * 需要做不同的处理
         *1.485 编码器 需要不断 发送指令
         * 2.传感器 不需要发送任何 信息
         *
         */
          char* buff = NULL;
          //获取编码器 角度 转速
          char buff03[] ={(char)0x01 ,(char)0x03 ,(char)0x00  ,(char)0x78  ,(char)0x00  ,(char)0x04  ,(char)0xC4  ,(char)0x10};

          //获取编码器 计数值
          char buff04[] ={(char)0x01 ,(char)0x04 ,(char)0x00  ,(char)0x40  ,(char)0x00  ,(char)0x02  ,(char)0x70  ,(char)0x1f};


          //继电器 获取当前 开关量
          char buff01[] ={(char)0xCC ,(char)0xDD ,(char)0xC0  ,(char)0x01  ,(char)0x00  ,(char)0x00 ,(char)0x0D  ,(char)0xCE  ,(char)0x9c};

        switch(Config::GetInstance()->GetDeviceID(getsessionIp()))
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
                buff = buff03;
            else
                buff = buff04;
           boost::asio::async_write(socket_, boost::asio::buffer(buff,device_bmq_data_len),
            boost::bind(&session::handle_write, shared_from_this(), boost::asio::placeholders::error));
            break;
        case device_jdq: //继电器 CC DD C0 01 00 00 0D CE 9C
            ::Sleep(50) ; //读取当前 开关量的 状态
            buff = buff01;
            boost::asio::async_write(socket_, boost::asio::buffer(buff, device_jdq_data_len),
             boost::bind(&session::handle_write, shared_from_this(), boost::asio::placeholders::error));
            break;
        }



        socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&session::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }




    /*处理回话*/
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
#if 0
//    QFile f("C:\\Boost\\test.txt");
//    if(!f.open(QIODevice::WriteOnly | QIODevice::Append))
//    {
//    }
//    QTextStream txtOutput(&f);
   // txtOutput << data_ << "\n";
//    f.close();
//          //     boost::asio::async_write(socket_, boost::asio::buffer(data_, bytes_transferred),
//          //      boost::bind(&session::handle_write, this, boost::asio::placeholders::error));

                     QString str;
                     for(int index = 0; index < bytes_transferred; ++ index)
                         {
                        str += QString("%1").arg(data_[index]&0xFF,2,16,QLatin1Char('0'));
                       //qDebug() << str;
                        }
             qDebug() << str << "\n";
#endif
            BusinessSession::GetInstance()->InterfaceFun(
				getsessionID(),data_,bytes_transferred);
            //测试
//                 QString str = QString("INSERT INTO `data_test`.`monitordate` (`MonitorCode`, `MonitorName`, `MonitorType`, `MonitorDesc`, `MonitorResult`, `MonitorFromElevator`) VALUES ('1', 'jsd', '08:58:39', 'sm', '5551aaffc4ffec07f61f1a', '1');");
//                 UserBuffer::GetInstance()->PushDatabaseQueue(str);;

        //开启下一次读取
                 start();
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
          //  start();
        }
        else
        {
            delete this;
        }
    }

private:
    tcp::socket socket_;
    char data_[MAX_PACKET_LEN];
    int recv_times;
};

class server
{
public:


    ~server()
    {

        stop();

    }

    void handle_accept(session* new_session, const boost::system::error_code& error)
    {
        if (!error)
        {
              qDebug() << QString("server:ip = ") << new_session->getsessionIp(1) << QString("server:port =") << new_session->getsessionPort(1);
            QString strIp =new_session->getsessionIp() ;
            qDebug() << QString("client:IP =") << strIp<< QString("client:port =") << new_session->getsessionPort();
            m_sessionGroup[strIp] = new_session;
            new_session->start();
            /*用户登录,记录信息*/
        }
        else
        {
            delete new_session;
        }

        new_session = new session(io_service_pool_.get_io_service());
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
    }

    void run()
    {
        io_service_pool_.start();
        io_service_pool_.join();
    }

    inline void stop()
    {
        io_service_pool_.stop();
        QMap<QString,session*>::iterator beginIt = m_sessionGroup.begin(),endIt = m_sessionGroup.end();
        for(;beginIt != endIt; ++beginIt)
        {

                        if((beginIt.value())->socket().is_open())
                        {
                            beginIt.value()->socket().close();
                        }
                        delete beginIt.value();
                        m_sessionGroup.remove(beginIt.key());
        }
    }

    static server* getInstance(short port = -1,  int thread_cnt =- 1)
    {
        if(NULL == m_singleton && port != -1 && thread_cnt != -1)
        {
            m_singleton = new server(port,thread_cnt);
        }
        return m_singleton;
    }


    //获取会话组中的 特定会话
    session* getMapSession(QString strIp)
    {
       return m_sessionGroup[strIp];
    }



private:
     server(short port,  int thread_cnt)
        :io_service_pool_(thread_cnt)
        ,acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port))
     {


         session* new_session = new session(io_service_pool_.get_io_service());

         acceptor_.async_accept(new_session->socket(),
             boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
     }
    static server* m_singleton;
    QMap<QString,session*>    m_sessionGroup; //会话管理
   // QMap<QString,session*>    m_client_sessionGroup; //客户端会话
    io_service_pool io_service_pool_;
    tcp::acceptor acceptor_;
};
#endif // ANSYCSERVICE_H
server* server::m_singleton;
