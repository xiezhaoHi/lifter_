#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H
#include<QThread>
#include"clientasio.h"
class CClientASIO;
class ClientServer:public QThread
{
   public:
    volatile  bool    m_stop;
    CClientASIO*    m_client_con; //�ͻ�������
    io_service*        m_io_service; //��¼io_services
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
     * �������ݵ� ���ӵķ����
     * strData ��ʾ ��Ҫ���͵����� Ϊ <dev ID='1'>texxt</dev> �ṹ ID��ʾ�豸ID  text Ϊ 0 ���� 1 (�ֽ׶εĶ���)
     */
    bool  send_to_server(const QString &strType, const QStringList &strData);
private:
    client_manager();

    static client_manager* m_singleton;
    ClientServer*    m_client;
};

#endif // CLIENT_MANAGER_H
