#ifndef ANSYCSERVICE_H
#define ANSYCSERVICE_H

#include"usebuffer.h" //������
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
#include"server/businesssession.h" //����Ự �������ݵ� ͨ����
#include"server/config/config.h" //������Ϣ ���� �豸 ip ���豸��ʶ����Ϣ��
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

    inline void start() //��ʱ ˫�� �ѽ�������, ��ʼ�Ự
    {
        /*�Ự����
         *��ͬ�豸�ĻỰ
         * ��Ҫ����ͬ�Ĵ���
         *1.485 ������ ��Ҫ���� ����ָ��
         * 2.������ ����Ҫ�����κ� ��Ϣ
         *
         */
          char* buff = NULL;
          //��ȡ������ �Ƕ� ת��
          char buff03[] ={(char)0x01 ,(char)0x03 ,(char)0x00  ,(char)0x78  ,(char)0x00  ,(char)0x04  ,(char)0xC4  ,(char)0x10};

          //��ȡ������ ����ֵ
          char buff04[] ={(char)0x01 ,(char)0x04 ,(char)0x00  ,(char)0x40  ,(char)0x00  ,(char)0x02  ,(char)0x70  ,(char)0x1f};


          //�̵��� ��ȡ��ǰ ������
          char buff01[] ={(char)0xCC ,(char)0xDD ,(char)0xC0  ,(char)0x01  ,(char)0x00  ,(char)0x00 ,(char)0x0D  ,(char)0xCE  ,(char)0x9c};

        switch(Config::GetInstance()->GetDeviceID(getsessionIp()))
        {
        case device_cgq: //������ �豸
            break;
        case device_bmq: //��ת�������豸
            ::Sleep(30);
            //03 �������� 04 ��������
            /*03 ���ط��� �Ƕ� ת��
             *04 ���ؼ���ֵ
             */

            if(BusinessSession::GetInstance()->getBmpFirst()) //��ʾ��ȡ��һ��ָ��
                buff = buff03;
            else
                buff = buff04;
           boost::asio::async_write(socket_, boost::asio::buffer(buff,device_bmq_data_len),
            boost::bind(&session::handle_write, shared_from_this(), boost::asio::placeholders::error));
            break;
        case device_jdq: //�̵��� CC DD C0 01 00 00 0D CE 9C
            ::Sleep(50) ; //��ȡ��ǰ �������� ״̬
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




    /*����ػ�*/
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
            //����
//                 QString str = QString("INSERT INTO `data_test`.`monitordate` (`MonitorCode`, `MonitorName`, `MonitorType`, `MonitorDesc`, `MonitorResult`, `MonitorFromElevator`) VALUES ('1', 'jsd', '08:58:39', 'sm', '5551aaffc4ffec07f61f1a', '1');");
//                 UserBuffer::GetInstance()->PushDatabaseQueue(str);;

        //������һ�ζ�ȡ
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
            /*�û���¼,��¼��Ϣ*/
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


    //��ȡ�Ự���е� �ض��Ự
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
    QMap<QString,session*>    m_sessionGroup; //�Ự����
   // QMap<QString,session*>    m_client_sessionGroup; //�ͻ��˻Ự
    io_service_pool io_service_pool_;
    tcp::acceptor acceptor_;
};
#endif // ANSYCSERVICE_H
server* server::m_singleton;
