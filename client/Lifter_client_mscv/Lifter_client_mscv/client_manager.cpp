#include "client_manager.h"
#include<QMessageBox>
#include<QApplication>
#include"config/config.h"
#include"log_/log_.h"
#include"database_service/mydatabase.h"
#include<QSettings>
#include<QStringList>
 client_manager* client_manager::m_singleton = NULL;
client_manager::client_manager()
{
    m_client = new ClientServer();
}
 client_manager* client_manager::GetInstance(void)
 {
    if(NULL == m_singleton)
        m_singleton = new client_manager();
    return m_singleton;
 }

void client_manager::start_server()
{
      m_client->start();
        if(!Log_::GetInstance()->Init_log())
        {
           // QMessageBox m(QIcon(":/images/G:/测试图片/Entypo_26c8(0).ico"),QString("日志初始化错误"),QString("提示"));
            QMessageBox message(QMessageBox::Warning,"提示","日志初始化错误",QMessageBox::Yes|QMessageBox::No,NULL);

            message.exec();

            return ;
        }


//        //2.配置初始化
//        qDebug() << "开始初始化配置服务!";
//        if(!Config::GetInstance()->InitConfig())
//        {
//            Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
//            return;
//        }

//        //3.数据库服务
//         qDebug() << "数据库服务开启!";
//         MyDatabase::GetInstance()->Start();


//         //4.设备配置信息
//         qDebug() << "初始化设备配置信息";
//         if(!Config::GetInstance()->InitDeviceConfig())
//         {
//             Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
//             return ;
//         }

        //5.启动client server

}

void client_manager::stop_server()
{
    //1.断开连接

    m_client->stop();
    m_client->wait();
    delete m_client;


}

/*
 * 发送数据到 连接的服务端
 * strData 表示 需要发送的数据 为 <dev ID='1'>texxt</dev> 结构 ID表示设备ID  text 为 0 或者 1 (现阶段的定义)
 */
bool client_manager::send_to_server(QString const& strType,QStringList const& strData)
{
    QString strTemp,strSend ;

    for(int index = 0; index <strData.size(); ++index)
    {
        strTemp = QString("<?xml version='1.0' encoding ='utf8'?>")
                +QString("<body type='%1'>").arg(strType)
                +strData[index]
                +QString("</body>");
           strSend += QString("being#%1#%2#").arg(strType).arg(strTemp.length()) + strTemp + QString("#end#") ;

    }

	if (m_client->m_client_con)
	{
		m_client->m_client_con->PostSend(strSend.toStdString());
	}
	else
		return false;
    return true;
}

void ClientServer::run()
{
    QString strPath,strIp,strPort;
    while(m_stop)
    {
         strPath= qApp->applicationDirPath()+"\\config\\config.ini";
        QSettings iniRead(strPath,QSettings::IniFormat);
        strIp = iniRead.value("service/IP").toString();
        strPort = iniRead.value("service/PORT").toString();
      //  qDebug() <<"start" <<  QThread::currentThreadId();
        io_service*  io_service_ = new io_service;
         CClientASIO client(strIp.toStdString(), strPort.toUShort(),io_service_);
         m_client_con = &client;
         m_io_service = io_service_;
         io_service_->run();
         m_client_con = NULL;
         m_io_service = NULL;
         ::Sleep(1000);
    }
}
