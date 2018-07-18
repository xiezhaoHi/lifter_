#include "server_manager.h"


#include"server/config/config.h"
#include"server/log_/log_.h"
#include"server/database_service/mydatabase.h"
#include"server/server.h" //优化server
#include<QMessageBox>
#include<QApplication>
#include<QEventLoop>
 server_manager* server_manager::m_singleton =NULL;
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
server_manager::server_manager()
{
    m_start_flag = false;
    m_quitThread = NULL;
}
 server_manager* server_manager::GetInstance(void)
 {
     if(NULL == m_singleton)
         m_singleton = new server_manager();
     return m_singleton;
 }

bool server_manager::start_server()
{

    /*初始化*/
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);	//初始化WS2_32.DLL



    //1.初始化日志
    if(!Log_::GetInstance()->Init_log(qApp->applicationDirPath()))
    {
       // QMessageBox m(QIcon(":/images/G:/测试图片/Entypo_26c8(0).ico"),QString("日志初始化错误"),QString("提示"));
        QMessageBox message(QMessageBox::Warning,"提示","日志初始化错误",QMessageBox::Ok,NULL);

        message.exec();

        return false;
    }


    //2.配置初始化
    qDebug() << "开始初始化配置服务!";
    if(!Config::GetInstance()->InitConfig(qApp->applicationDirPath()+"\\config\\config.ini"))
    {
        Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
        QMessageBox message(QMessageBox::Warning,"提示","配置服务初始化错误",QMessageBox::Ok,NULL);
        message.exec();
        return false;
    }

    //3.数据库服务
     qDebug() << "数据库服务开启!";
     MyDatabase::GetInstance()->Start();

     //4.设备配置信息
     qDebug() << "初始化设备配置信息";
     if(!Config::GetInstance()->InitDeviceConfig())
     {
         Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
         QMessageBox message(QMessageBox::Warning,"提示","设备配置信息服务初始化错误"
                             ,QMessageBox::Ok,NULL);
         message.exec();
         return false;
     }


     //4.采集服务
     qDebug() << "采集服务开启!";
	 //4.1 专门处理用户消息
	 Myserver::getInstance(Config::GetInstance()->GetServicePort(), THREAD_POOL_NUM);
	 //采集设备服务
	 Myserver::getInstance()->run_server();
     m_start_flag = true;
     return true;
}
void MyQuitThread::run()
{

    /*
     * 采集服务结束
     */
    if(m_flag)
		Myserver::getInstance()->stop_server();
    /*
     *数据库服务结束
     */
    MyDatabase::GetInstance()->Destroy();
}

void server_manager::stop_server()
{
//    m_quitThread = new MyQuitThread(m_start_flag);
//    m_quitThread->start();

    /*
     * 采集服务结束
     */
    if(m_start_flag)
		Myserver::getInstance()->stop_server();
    /*
     *数据库服务结束
     */
    MyDatabase::GetInstance()->Destroy();
	qDebug() << QString("结束服务!");

	//卸载网络库
	WSACleanup();
}



