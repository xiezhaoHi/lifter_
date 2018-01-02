#include "connections.h"
#include <boost/bind.hpp>
#include"session.h"
#include"config/config.h"
#include"server/server.h"
bool Connections::Add(shared_ptr<Mysession> &con) {
    boost::mutex::scoped_lock lock(mtx);

//    values_.insert(con);
//   return true;


    QString strIp = con->getsessionIp();

    //model test
  //  m_client_map[QString("192.168.1.8")] = con;
/************************************************/

    if(Config::GetInstance()->GetUserInfo(device_or_client_device).contains(strIp)) //设备登入
    {
        m_device_map[strIp] = con;

        return true;
    }
    if(Config::GetInstance()->GetUserInfo(device_or_client_client).contains(strIp)) //用户登入
    {
        m_client_map[strIp] = con;

        return true;
    }
    return false;

}

void Connections::Remove(shared_ptr<Mysession> con) {
    boost::mutex::scoped_lock lock(mtx);

    QString strIP = con->getsessionIp();

	Myserver::getInstance()->recordUserDL(strIP,QString("%1").arg(con->getsessionPort()),0);

    if(m_device_map.contains(strIP))
         m_device_map.remove(strIP);
    if(m_client_map.contains(strIP))
        m_client_map.remove(strIP);
}

void Connections::CloseAll() {

#ifndef SESSION_TO_DEVICECONTROL
    if(!m_device_map.isEmpty())
    {
        QList<QString> listKeys = m_device_map.keys();
        for(int index = 0; index < listKeys.size(); ++index)
        {
            long c = m_device_map[listKeys[index]].use_count();
            if(m_device_map[listKeys[index]]->socket().is_open())
            {
                m_device_map[listKeys[index]]->socket().shutdown(tcp::socket::shutdown_both);
                m_device_map[listKeys[index]]->socket().close();
            }
    //        Remove(m_device_map[listKeys[index]]);
           //m_device_map.remove(listKeys[index]);
          //  m_device_map[listKeys[index]]->CloseSocket();

        }
    }
    m_client_map.clear();
#endif
    if(m_client_map.isEmpty())
        return;
    QList<QString> listKeys_client = m_client_map.keys();
    for(int index = 0; index < listKeys_client.size(); ++index)
    {
        Remove(m_client_map[listKeys_client[index]]);
    }

}
