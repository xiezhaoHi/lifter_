#ifndef CLIENTASIO_H
#define CLIENTASIO_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include<boost/thread.hpp>
#include<QString>
using namespace boost::asio;
using namespace boost::asio::ip;

#define _RECV_BUF_SIZE_ 4096

class CClientASIO
{
public:
    CClientASIO (const std::string & hostname, unsigned short port,io_service  * io_service_);
    virtual ~CClientASIO();

    // methods
    virtual void PostSend (const std::string & message);
    virtual void start(void);

    //event call
    virtual bool OnRecv (const boost::system::error_code& error,size_t bytes_transferred);
    virtual bool OnSend(const boost::system::error_code& error,size_t bytes_transferred);
    virtual void OnClose (const boost::system::error_code& error);

    //解析收到的 数据
    void  DealData(QString const& strData);
   // static CClientASIO*    GetInstance(const std::string & hostname, unsigned short port);
private:

    io_service*   m_io_service;
    //boost::shared_ptr<boost::thread>&  m_thread;
    tcp::socket*  m_socket;

    char          m_data[_RECV_BUF_SIZE_];

    static CClientASIO*     m_singleton;

};

#endif // CLIENTASIO_H
