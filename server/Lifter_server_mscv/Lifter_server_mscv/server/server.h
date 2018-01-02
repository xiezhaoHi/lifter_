#ifndef SERVER_H
#define SERVER_H


#include"buffer/usebuffer.h" //缓冲区
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

#include"session.h"
#include"connections.h"
#include"io_pool.h"
using namespace boost;

using boost::asio::ip::tcp;
//#pragma comment(lib,"E:\\boost库\\boost_1_63_0_msvc\\boost_1_63_0\\stage\\lib\\libboost_system-vc140-mt-gd-1_63.lib")

class Myserver
{
public:


    ~Myserver();

    void handle_accept(boost::shared_ptr<Mysession>& new_session, const boost::system::error_code& error);

    /*
     *开启服务
     */
    void run_server();

    /*
     * 停止服务
     */
    void stop_server();

    static Myserver* getInstance(short port = -1,  int thread_cnt =- 1);



    /*
     * 给设备发送数据
     * 暂时没用
     */
    bool     writeToDevice(QString strIp,char* buff, int len);

    /*
     * 登入信息记录
     * flag: 0 下线 1 上线
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
    bool            m_start_flag; //server 开启标志
};


#endif // SERVER_H
