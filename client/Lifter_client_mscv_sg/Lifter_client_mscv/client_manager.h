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
		
		if (m_io_service)
		{
			m_io_service->stop();
		}
            
		m_stop = false;
    }
};


//上线 和 心跳
class clientMsgInform :public QThread
{
public:
	clientMsgInform::clientMsgInform(QString lifterID,QString clientID)
		:m_stop(true),m_lifterID(lifterID), m_online(false), m_clientID(clientID){};
	volatile  bool m_stop;
	bool m_online; //上线标志
	QString m_lifterID;//关联电梯ID
	QString m_clientID; //客户端ID

	void run();
	void stop();

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

	/*获取客户端是否连接上服务端*/
	bool  get_online_status();

private:
    client_manager();

    static client_manager* m_singleton;
    ClientServer*    m_client;
	clientMsgInform* m_clientMsg;//客户端 上线 下线 心跳消息
};

#endif // CLIENT_MANAGER_H
