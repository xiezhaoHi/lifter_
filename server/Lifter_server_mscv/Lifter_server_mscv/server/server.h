#ifndef SERVER_H
#define SERVER_H


#include"buffer/usebuffer.h" //������
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

#include"session.h"
#include"connections.h"
#include"io_pool.h"
using namespace boost;

using boost::asio::ip::tcp;
//#pragma comment(lib,"E:\\boost��\\boost_1_63_0_msvc\\boost_1_63_0\\stage\\lib\\libboost_system-vc140-mt-gd-1_63.lib")

class Myserver
{
public:


    ~Myserver();

    void handle_accept(boost::shared_ptr<Mysession>& new_session, const boost::system::error_code& error);

    /*
     *��������
     */
    void run_server();

    /*
     * ֹͣ����
     */
    void stop_server();

    static Myserver* getInstance(short port = -1,  int thread_cnt =- 1);



    /*
     * ���豸��������
     * ��ʱû��
     */
    bool     writeToDevice(QString strIp,char* buff, int len);

    /*
     * ������Ϣ��¼
     * flag: 0 ���� 1 ����
     */
    void    recordUserDL(QString const& strIp, QString const& strPort,int flag=1);
private:
	Myserver(short port,  int thread_cnt)
        :io_service_pool_(thread_cnt)
        ,acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port))
     {

         shared_ptr<Mysession> new_session(new Mysession(io_service_pool_.get_io_service(),m_connections));

         acceptor_.async_accept(new_session->socket(),
             boost::bind(&Myserver::handle_accept, this, new_session, boost::asio::placeholders::error));
     }
    static Myserver* m_singleton;
    io_service_pool io_service_pool_;
    tcp::acceptor acceptor_;

    Connections     m_connections;
    bool            m_start_flag; //server ������־
};


#endif // SERVER_H
