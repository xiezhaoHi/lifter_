#include "config.h"
#include <QSettings>
#include<QApplication>
#include"database_service/mydatabase.h"
#include<QDomDocument>
Config* Config::m_singleton = NULL;
Config::Config()
{

}



/*��ȡ��ʵ��*/
Config*  Config::GetInstance(void)
{
    if(NULL == m_singleton)
    {
        m_singleton = new Config();
    }
      return m_singleton;
}

/*���ó�ʼ��
*���3�� ��Ϣmap
*m_deviceMap     m_clientMap m_databaseMap
*/
bool Config::InitConfig(void)
{
    QString strPath = qApp->applicationDirPath()+"\\config\\config.ini";

    QSettings iniRead(strPath,QSettings::IniFormat);
   // QString strHost = iniRead.value("SOCKET/host").toString();
   // int  port = iniRead.value("SOCKET/port").toInt();
    int maxNum = iniRead.value("database/database_max").toInt();
    QString strTemp ;
   for(int index = 0; index < maxNum; ++index)  //���ݿ�������Ϣ
    {
       strTemp = iniRead.value(QString("database/database_%1").arg(index)).toString();
       if(strTemp.isEmpty())
       {
           m_error ="##" + QString("database/database_%1").arg(index) + "##������Ϊ��";
           return false;
        }
        m_databaseMap[index] = strTemp;
        //!����
    }
   strTemp = iniRead.value(QString("service/servvice_port")).toString();
   if(strTemp.isEmpty())
   {
       m_error ="##" + QString("service/servvice_port") + "##������Ϊ��";
       return false;
    }
    m_service_port = strTemp.toInt();


    m_deviceID_map[QString("1001")] = device_cgq;
    m_deviceID_map[QString("1002")] = device_bmq;
    m_deviceID_map[QString("1003")] = device_jdq;


//   maxNum = iniRead.value("device/device_max").toInt();
//   for(int index = 0; index < maxNum; ++index)  //�豸
//    {
//       strTemp = iniRead.value(QString("device/device_%1").arg(index)).toString();
//       if(strTemp.isEmpty())
//       {
//           m_error ="##" + QString("device/device_%1").arg(index) + "##������Ϊ��";
//           return false;
//        }
//        m_devcice_ipMap[strTemp] = index+device_begin_ID;
//        m_deviceMap[index] = strTemp;

//        //!����
//    }

//   maxNum = iniRead.value("client/client_max").toInt();
//   for(int index = 0; index < maxNum; ++index)  //�ͻ���
//    {
//       strTemp = iniRead.value(QString("client/client_%1").arg(index)).toString();
//       if(strTemp.isEmpty())
//       {
//           m_error ="##" + QString("client/client_%1").arg(index) + "##������Ϊ��";
//           return false;
//        }
//        m_clientMap_ipMap[strTemp] = index;
//        m_clientMap[index] = strTemp;
//        //!����
//    }




   return true;
}


/*
 * ��ʼ�� �豸������Ϣ
 */
bool Config::InitDeviceConfig()
{


    /*
     *
     * �����ݿ� ��ȡ �豸��ϸ��������Ϣ
     *  QStringList*    m_config_list[config_max];
     *    int                     m_config_list_num[config_max];
     */

     QStringList strSqlList;

     strSqlList.append( QString("SELECT `observe_control_device`.`id`,\
                             `observe_control_device`.`name`,\
                             `observe_control_device`.`describe`,\
                             `observe_control_device`.`ip_address`,\
                             `observe_control_device`.`belongs`\
                         FROM `data_test`.`observe_control_device`;"));
    strSqlList.append(QString("SELECT `be_observe_control_device`.`id`,\
                            `be_observe_control_device`.`name`,\
                            `be_observe_control_device`.`describe`,\
                            `be_observe_control_device`.`belongs`\
                        FROM `data_test`.`be_observe_control_device`;"));
    strSqlList.append( QString("SELECT `device_config`.`id`,\
                                               `device_config`.`config`\
                                           FROM `data_test`.`device_config`;"));
    strSqlList.append(QString("SELECT `lifter_config`.`id`,\
                                               `lifter_config`.`name`,\
                                               `lifter_config`.`describe`\
                                           FROM `data_test`.`lifter_config`;"));
     strSqlList.append(QString("SELECT `lifter_id_describe`.`id`,\
                                                    `lifter_id_describe`.`type`\
                                                FROM `data_test`.`lifter_id_describe`;"));
     strSqlList.append( QString("SELECT `client_config`.`id`,\
                                                `client_config`.`address`,\
                                                `client_config`.`belongs`\
                                            FROM `data_test`.`client_config`;"));

     for(int index = 0; index < config_max; ++index)
     {
        if(!MyDatabase::GetInstance()->SelectData(strSqlList[index],m_config_list[index],m_config_list_num[index]))
        {
              m_error = "ִ��sql������!";
            return false;
         }
        if(0 == m_config_list_num[index])
        {
            m_error = "���ô���!###"+strSqlList[index];
            return false;
        }
     }

      //����豸map
      for(int index = 0; index < m_config_list_num[config_observe_control_device];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_observe_control_device][index]))));
              QString str = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
                          //ӳ�� ip �������豸����Ϣlist
              m_devcice_ipMap[(*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address]]=p ;

                          //ӳ�� ID �� �豸ip
              m_deviceID_to_ip_map[(*(m_config_list[config_observe_control_device][index]))[observe_control_device_id]]
                          = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
    }

      //���client map
      for(int index = 0; index < m_config_list_num[config_client_config];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_client_config][index]))));
              QString str = (*(m_config_list[config_client_config][index]))[client_config_address];
              m_clientMap_ipMap[(*(m_config_list[config_client_config][index]))[client_config_address]]=p ;
    }





     //���̵��� ip �� �����豸
     for(int index = 0; index <m_config_list_num[config_device_config]; ++index)
     {
         QDomDocument doc;
         QString error;
          int row = 0, column = 0;
         if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
             return false;
         QDomElement root = doc.firstChildElement();
         QString strRootID = root.attribute(QString("ID"));
         root = root.firstChildElement(QString("DO_DATA")); //DO_DATA �ڵ㿪ʼ
         root = root.firstChildElement(); //DO_DATA �ڵ�ĵ�һ���ӽڵ㿪ʼ
         QMap<int,QString> device_config_map;
         while(!root.isNull())
         {
             QString strID = root.attribute(QString("ID"));
             QString strLink = root.attribute(QString("LINK"));
             m_deviceIp_byID_map[strLink] = m_deviceID_to_ip_map[strRootID];
             root = root.nextSiblingElement();
          }
     }


return true;
    /***********************************************************/
}

/*
 * ��ȡ �豸 �ͻ��� ע����Ϣ
 * flag  0 ��ʾ �豸  1��ʾ client �û�
 */
QMap<QString,boost::shared_ptr<QStringList>>& Config::GetUserInfo(int flag )
{
    switch (flag) {
    case 0:
       return m_devcice_ipMap;
        break;
    case 1:
        return m_clientMap_ipMap;
        break;
    }
}


/*
 *���� ָ���豸�� client ip
 */
QString     Config::GetDeviceIp(QString strIp)
{
    if(m_devcice_ipMap.contains(strIp))
    {
       QString strBelongs = m_devcice_ipMap[strIp]->at(observe_control_device_belongs); //��������

        QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_clientMap_ipMap.constBegin();
        for(; it != m_clientMap_ipMap.constEnd(); ++it)
        {
            if(it.value()->at(client_config_belongs) == strBelongs)  //�豸��������  ��Ӧ�� ����client
            {
                return it.value()->at(client_config_address);
            }
        }
    }
    return ""; //û����Ӧ�� ���ؿ�
}

/*
 *
 * ��ȡ������Ϣ
 */
 QString Config::GetErrInfo(void)
 {
     return m_error;
 }
 /*
  *
  * ��ȡ�豸��Ϣmap
  * ����0 1 2 �ֱ��ʾ
  * �豸��Ϣ
  * �ͻ�����Ϣ
  * ���ݿ���Ϣ
  */
 QMap<int,QString> & Config::GetMap(int mapType)
 {
     switch (mapType) {
     case map_database:
        return m_databaseMap;
         break;
     case map_device:
        return m_deviceMap;
         break;
     default:
         break;
     }

 }
 /*
  *
  * ��ȡservice port
  */
 int Config::GetServicePort(void)
 {
    return m_service_port;
 }

 /*
  * ��ȡ�豸 ID
  * �豸ip��Ϊ����
  */
 QString Config::GetDeviceID(QString strIp)
 {
     if(m_devcice_ipMap.contains(strIp) ) //�豸�а������豸
     {
         return m_devcice_ipMap[strIp]->at(observe_control_device_id);
      }
     return "";
 }

 /*
  * ��ȡ�豸 ��ʶ  ���Ǹ�ʲô�豸
  * �����豸�� �Ϳͻ��˱� ��ȡ �����û������ �� ����m_deviceMap
  */
 int Config::GetDeviceFlag(QString strIp)
 {
        //return m_devcice_ipMap[strIp];
     QString str = GetDeviceID(strIp);
     if(!str.isEmpty())
     {
        return m_deviceID_map[str.left(USER_ID_COUNT)];
     }
    return -1;
 }

 /*
  *���� ָ���豸ID�� ������Ϣ
  */
 QString Config::GetDeviceConfig(QString strID)
 {
     if(!m_config_list[config_device_config])
         return "";
    for(int index = 0; index < m_config_list_num[config_device_config]; ++index)
    {
        if(m_config_list[config_device_config][index]->at(0) == strID)
            return m_config_list[config_device_config][index]->at(1);
    }
    return "";
 }
 /*
  *ָ�� �̵��������豸ID ���� �̵���IP
  */
 QString Config::GetDeviceIpByID(QString strID)
 {
        return m_deviceIp_byID_map[strID];
 }
