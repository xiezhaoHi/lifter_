#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H
#include<QThread>
class MyQuitThread:public QThread
{
    Q_OBJECT
public:
    MyQuitThread(bool flag):m_flag(flag)
    {

    }

    void run();
    bool    m_flag;
};

class server_manager
{

public:
    static server_manager* GetInstance(void);
    ~server_manager()
    {
        if(m_quitThread)
            m_quitThread->wait();
    }

    bool start_server();
    void stop_server();
    bool    m_start_flag ;
    MyQuitThread* m_quitThread;
private:
    server_manager();

    static server_manager* m_singleton;
};

#endif // SERVER_MANAGER_H
