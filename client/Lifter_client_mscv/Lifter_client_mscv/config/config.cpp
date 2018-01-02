#include "config.h"
#include <QSettings>
#include<QApplication>
#include"database_service/mydatabase.h"
#include<QDomDocument>
Config* Config::m_singleton = NULL;
Config::Config()
{

}



/*获取单实例*/
Config*  Config::GetInstance(void)
{
    if(NULL == m_singleton)
    {
        m_singleton = new Config();
    }
      return m_singleton;
}

/*配置初始化
*填充3个 信息map
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
   for(int index = 0; index < maxNum; ++index)  //数据库配置信息
    {
       strTemp = iniRead.value(QString("database/database_%1").arg(index)).toString();
       if(strTemp.isEmpty())
       {
           m_error ="##" + QString("database/database_%1").arg(index) + "##项数据为空";
           return false;
        }
        m_databaseMap[index] = strTemp;
        //!配置
    }
   strTemp = iniRead.value(QString("service/servvice_port")).toString();
   if(strTemp.isEmpty())
   {
       m_error ="##" + QString("service/servvice_port") + "##项数据为空";
       return false;
    }
    m_service_port = strTemp.toInt();


    m_deviceID_map[QString("1001")] = device_cgq;
    m_deviceID_map[QString("1002")] = device_bmq;
    m_deviceID_map[QString("1003")] = device_jdq;


//   maxNum = iniRead.value("device/device_max").toInt();
//   for(int index = 0; index < maxNum; ++index)  //设备
//    {
//       strTemp = iniRead.value(QString("device/device_%1").arg(index)).toString();
//       if(strTemp.isEmpty())
//       {
//           m_error ="##" + QString("device/device_%1").arg(index) + "##项数据为空";
//           return false;
//        }
//        m_devcice_ipMap[strTemp] = index+device_begin_ID;
//        m_deviceMap[index] = strTemp;

//        //!配置
//    }

//   maxNum = iniRead.value("client/client_max").toInt();
//   for(int index = 0; index < maxNum; ++index)  //客户端
//    {
//       strTemp = iniRead.value(QString("client/client_%1").arg(index)).toString();
//       if(strTemp.isEmpty())
//       {
//           m_error ="##" + QString("client/client_%1").arg(index) + "##项数据为空";
//           return false;
//        }
//        m_clientMap_ipMap[strTemp] = index;
//        m_clientMap[index] = strTemp;
//        //!配置
//    }




   return true;
}


/*
 * 初始化 设备配置信息
 */
bool Config::InitDeviceConfig()
{


    /*
     *
     * 从数据库 读取 设备详细的配置信息
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
              m_error = "执行sql语句错误!";
            return false;
         }
        if(0 == m_config_list_num[index])
        {
            m_error = "配置错误!###"+strSqlList[index];
            return false;
        }
     }

      //填充设备map
      for(int index = 0; index < m_config_list_num[config_observe_control_device];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_observe_control_device][index]))));
              QString str = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
                          //映射 ip 到整个设备的信息list
              m_devcice_ipMap[(*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address]]=p ;

                          //映射 ID 到 设备ip
              m_deviceID_to_ip_map[(*(m_config_list[config_observe_control_device][index]))[observe_control_device_id]]
                          = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
    }

      //填充client map
      for(int index = 0; index < m_config_list_num[config_client_config];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_client_config][index]))));
              QString str = (*(m_config_list[config_client_config][index]))[client_config_address];
              m_clientMap_ipMap[(*(m_config_list[config_client_config][index]))[client_config_address]]=p ;
    }





     //填充继电器 ip 与 关联设备
     for(int index = 0; index <m_config_list_num[config_device_config]; ++index)
     {
         QDomDocument doc;
         QString error;
          int row = 0, column = 0;
         if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
             return false;
         QDomElement root = doc.firstChildElement();
         QString strRootID = root.attribute(QString("ID"));
         root = root.firstChildElement(QString("DO_DATA")); //DO_DATA 节点开始
         root = root.firstChildElement(); //DO_DATA 节点的第一个子节点开始
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
 * 获取 设备 客户端 注册信息
 * flag  0 表示 设备  1表示 client 用户
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
 *返回 指定设备的 client ip
 */
QString     Config::GetDeviceIp(QString strIp)
{
    if(m_devcice_ipMap.contains(strIp))
    {
       QString strBelongs = m_devcice_ipMap[strIp]->at(observe_control_device_belongs); //所属电梯

        QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_clientMap_ipMap.constBegin();
        for(; it != m_clientMap_ipMap.constEnd(); ++it)
        {
            if(it.value()->at(client_config_belongs) == strBelongs)  //设备所属电梯  相应的 管理client
            {
                return it.value()->at(client_config_address);
            }
        }
    }
    return ""; //没有相应的 返回空
}

/*
 *
 * 获取错误信息
 */
 QString Config::GetErrInfo(void)
 {
     return m_error;
 }
 /*
  *
  * 获取设备信息map
  * 参数0 1 2 分别表示
  * 设备信息
  * 客户端信息
  * 数据库信息
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
  * 获取service port
  */
 int Config::GetServicePort(void)
 {
    return m_service_port;
 }

 /*
  * 获取设备 ID
  * 设备ip作为参数
  */
 QString Config::GetDeviceID(QString strIp)
 {
     if(m_devcice_ipMap.contains(strIp) ) //设备中包含该设备
     {
         return m_devcice_ipMap[strIp]->at(observe_control_device_id);
      }
     return "";
 }

 /*
  * 获取设备 标识  这是个什么设备
  * 遍历设备表 和客户端表 获取 连接用户的身份 并 保存m_deviceMap
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
  *返回 指定设备ID的 配置信息
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
  *指定 继电器关联设备ID 返回 继电器IP
  */
 QString Config::GetDeviceIpByID(QString strID)
 {
        return m_deviceIp_byID_map[strID];
 }
