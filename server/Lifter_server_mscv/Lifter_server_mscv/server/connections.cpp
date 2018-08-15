#include "connections.h"
#include <boost/bind.hpp>
#include"session.h"
#include"config/config.h"
#include"server/server.h"
bool Connections::Add(shared_ptr<Mysession> &con) {
    boost::mutex::scoped_lock lock(mtx);

//    values_.insert(con);
//   return true;

	QString strID= con->getsessionID();
   

    //model test
  //  m_client_map[QString("192.168.1.8")] = con;
/************************************************/

    if(Config::GetInstance()->GetUserInfo(device_or_client_device).contains(strID)) //设备登入
    {
        m_device_map[strID] = con;

        return true;
    }
    if(Config::GetInstance()->GetUserInfo(device_or_client_client).contains(strID)) //用户登入
    {
        m_client_map[strID] = con;

        return true;
    }
    return false;

}

void Connections::Remove(shared_ptr<Mysession> con) {
    boost::mutex::scoped_lock lock(mtx);

    QString strID = con->getsessionID();

	Myserver::getInstance()->recordUserDL(con->getsessionIp(),QString("%1").arg(con->getsessionPort()),0);

    if(m_device_map.contains(strID))
         m_device_map.remove(strID);
    if(m_client_map.contains(strID))
        m_client_map.remove(strID);
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
