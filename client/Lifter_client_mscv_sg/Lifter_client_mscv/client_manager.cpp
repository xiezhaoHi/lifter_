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
	m_clientMsg = nullptr;
}
 client_manager* client_manager::GetInstance(void)
 {
    if(NULL == m_singleton)
        m_singleton = new client_manager();
    return m_singleton;
 }

void client_manager::start_server()
{
	//1.�����������
		m_client->start();

	//2.�ͻ�����Ϣ�߳�
		QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
		QSettings iniRead(strPath, QSettings::IniFormat);
		QString strLifterID = iniRead.value("lifter/lifter_choose").toString();
		QString strClientID = iniRead.value("client/client_id").toString();
		m_clientMsg = new clientMsgInform(strLifterID, strClientID);
		m_clientMsg->start();


	//3.��־��������
        if(!Log_::GetInstance()->Init_log())
        {
           // QMessageBox m(QIcon(":/images/G:/����ͼƬ/Entypo_26c8(0).ico"),QString("��־��ʼ������"),QString("��ʾ"));
            QMessageBox message(QMessageBox::Warning,"��ʾ","��־��ʼ������",QMessageBox::Yes|QMessageBox::No,NULL);

            message.exec();

            return ;
        }


//        //2.���ó�ʼ��
//        qDebug() << "��ʼ��ʼ�����÷���!";
//        if(!Config::GetInstance()->InitConfig())
//        {
//            Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
//            return;
//        }

//        //3.���ݿ����
//         qDebug() << "���ݿ������!";
//         MyDatabase::GetInstance()->Start();


//         //4.�豸������Ϣ
//         qDebug() << "��ʼ���豸������Ϣ";
//         if(!Config::GetInstance()->InitDeviceConfig())
//         {
//             Log_::GetInstance()->Write_log(Config::GetInstance()->GetErrInfo(),logerr_fatal);
//             return ;
//         }

        //5.����client server

}

void client_manager::stop_server()
{
	//1.�ͻ��˶Ͽ���Ϣ ����
	m_clientMsg->stop();
	m_clientMsg->wait();

    //2.�Ͽ�����
    m_client->stop();
    m_client->wait();
    delete m_client;
}

/*
 * �������ݵ� ���ӵķ����
 * strData ��ʾ ��Ҫ���͵����� Ϊ <dev ID='1'>texxt</dev> �ṹ ID��ʾ�豸ID  text Ϊ 0 ���� 1 (�ֽ׶εĶ���)
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

/*��ȡ�ͻ����Ƿ������Ϸ����*/
bool  client_manager::get_online_status()
{
	if (m_clientMsg)
	{
		return m_clientMsg->m_online;
	}
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


 void clientMsgInform::run()
{
	 QString strSend;
	 QStringList sendList;
	 while (m_stop)
	 {
		 //����
		
		 if (!m_online) //����
		 {
			 strSend = QString("<lifterID>%1</lifterID><clientID>%2</clientID><msg>%3</msg>")
				 .arg(m_lifterID).arg(m_clientID).arg(client_online);
			 sendList.clear();
			 sendList.append(strSend);
			 if (client_manager::GetInstance()->send_to_server(CLIENTMSG, sendList))
			 {
				 m_online = true;
			 }
		 }
		 else //���� ��������
		 {
			 strSend = QString("<lifterID>%1</lifterID><clientID>%2</clientID><msg>%3</msg>")
				 .arg(m_lifterID).arg(m_clientID).arg(client_heartbeat);
			 sendList.clear();
			 sendList.append(strSend);
			 if (!client_manager::GetInstance()->send_to_server(CLIENTMSG, sendList)) //����ʧ�� ����
			 {
				 m_online = false;
			 }
		 }
		 Sleep(3000);
	 }
	 strSend = QString("<lifterID>%1</lifterID><clientID>%2</clientID><msg>%3</msg>")
		 .arg(m_lifterID).arg(m_clientID).arg(client_offline);
	 sendList.clear();
	 sendList.append(strSend);
	 client_manager::GetInstance()->send_to_server(CLIENTMSG, sendList);
}

 void clientMsgInform::stop()
 {
	 m_stop = false;
 }
