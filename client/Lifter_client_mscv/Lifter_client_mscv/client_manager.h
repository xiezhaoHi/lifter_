#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H
#include<QThread>
#include"clientasio.h"
class CClientASIO;
class ClientServer:public QThread
{
   public:
    volatile  bool    m_stop;
    CClientASIO*    m_client_con; //客户端连接
    io_service*        m_io_service; //记录io_services
    ClientServer():m_stop(true)
    ,m_client_con(NULL)
     ,m_io_service(NULL)
    {}
    void run();
public:

    void stop()
     {
        if( m_io_service)
            m_io_service->stop();
        m_stop = false;
    }
};
class client_manager
{

public:
    static client_manager* GetInstance(void);

    void start_server();
    void stop_server();
    /*
     * 发送数据到 连接的服务端
     * strData 表示 需要发送的数据 为 <dev ID='1'>texxt</dev> 结构 ID表示设备ID  text 为 0 或者 1 (现阶段的定义)
     */
    bool  send_to_server(const QString &strType, const QStringList &strData);
private:
    client_manager();

    static client_manager* m_singleton;
    ClientServer*    m_client;
};

#endif // CLIENT_MANAGER_H
