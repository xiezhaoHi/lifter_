#include "config.h"
#include <QSettings>
#include<QApplication>
#include"server/database_service/mydatabase.h"
#include<Qtxml/QDomDocument>
#include"enum.h"
Config* Config::m_singleton = NULL;
Config::Config()
{

}
Config::~Config()
{
    if(m_config_list)
    {
        for(int index = 0; index < config_max; ++index)
        {
            for(int num = 0; num < m_config_list_num[index]; ++num)
            {
                if(m_config_list[index][num])
                    delete m_config_list[index][num];
            }
            delete m_config_list[index];
        }
    }

    m_devcice_ipMap.clear(); //设备ip 映射 设备list
    m_clientMap_ipMap.clear();
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
bool Config::InitConfig(QString strPath)
{
   // QString strPath = qApp->applicationDirPath()+"\\config\\config.ini";

    QSettings iniRead(strPath,QSettings::IniFormat);
   // QString strHost = iniRead.value("SOCKET/host").toString();
   // int  port = iniRead.value("SOCKET/port").toInt();
      //程序数据库配置信息
    int maxNum = iniRead.value("database/database_max").toInt();
    QString strTemp ;
   for(int index = 0; index < maxNum; ++index)
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

   //钢丝绳数据库配置信息
    maxNum = iniRead.value("database_gss/database_max").toInt();
   for(int index = 0; index < maxNum; ++index)
    {
       strTemp = iniRead.value(QString("database_gss/database_%1").arg(index)).toString();
       if(strTemp.isEmpty())
       {
           m_error ="##" + QString("database_gss/database_%1").arg(index) + "##项数据为空";
           return false;
        }
        m_databaseMap_gss[index] = strTemp;
        //!配置
    }


   //服务器信息配置
   strTemp = iniRead.value(QString("service/servvice_port")).toString();
   if(strTemp.isEmpty())
   {
       m_error ="##" + QString("service/servvice_port") + "##项数据为空";
       return false;
    }
    m_service_port = strTemp.toInt();

    for(int index = device_cgq; index<device_max; ++index)
    {
        m_deviceID_map[QString("%1").arg(index)] = index;
    }

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
     QString strDBName = m_databaseMap[database_dbName];
     strSqlList.append( QString("SELECT `observe_control_device`.`id`,\
                             `observe_control_device`.`name`,\
                             `observe_control_device`.`describe`,\
                             `observe_control_device`.`ip_address`,\
                             `observe_control_device`.`port`,\
                             `observe_control_device`.`belongs`\
                         FROM `%1`.`observe_control_device`;").arg(strDBName));
    strSqlList.append(QString("SELECT `be_observe_control_device`.`id`,\
                            `be_observe_control_device`.`name`,\
                            `be_observe_control_device`.`describe`,\
                            `be_observe_control_device`.`belongs`,\
                            `be_observe_control_device`.`binddevice`\
                        FROM `%1`.`be_observe_control_device`;").arg(strDBName));
    strSqlList.append( QString("SELECT `device_config`.`id`,\
                                               `device_config`.`config`\
                                           FROM `%1`.`device_config`;").arg(strDBName));
    strSqlList.append(QString("SELECT `lifter_config`.`id`,\
                                               `lifter_config`.`name`,\
                                               `lifter_config`.`describe`\
                                           FROM `%1`.`lifter_config`;").arg(strDBName));
     strSqlList.append(QString("SELECT `lifter_id_describe`.`id`,\
                                                    `lifter_id_describe`.`type`\
                                                FROM `%1`.`lifter_id_describe`;").arg(strDBName));
     strSqlList.append( QString("SELECT `client_config`.`id`,\
                                                `client_config`.`address`,\
                                                `client_config`.`belongs`\
                                            FROM `%1`.`client_config`;").arg(strDBName));



     for(int index = 0; index < config_max; ++index)
     {
        if(!MyDatabase::GetInstance()->SelectData(strSqlList[index],m_config_list[index],m_config_list_num[index]))
        {
              m_error = "执行sql语句错误!";
            return false;
         }
        if(0 == m_config_list_num[index] && index != config_client_config)
        {
            m_error = "配置错误!###"+strSqlList[index];
            return false;
        }
     }

      //填充设备map
      for(int index = 0; index < m_config_list_num[config_observe_control_device];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_observe_control_device][index]))));
              QString strIp = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
              QString strID = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_id];
              QString strPort = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_port];
                          //映射 ip 到整个设备的信息list
              m_devcice_ipMap[strIp]=p ;

                          //映射 ID 到 设备ip
              m_deviceID_to_ip_map[strID]= strIp;

       /*
        *填充继电器类型设备表 m_jdq_device_to_ip_port
        */
        if(device_jdq == m_deviceID_map[strID.left(4)])
        {
            ip_port ipPort;
            ipPort.m_strIp = strIp;
            ipPort.m_strPort = strPort.toInt();
            m_jdq_device_to_ip_port[strIp] =ipPort;
        }
      /*
       *填充传感器类型设备表 m_cgq_device_to_ip_port
       */
          if(device_cgq == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_cgq_device_to_ip_port[strIp] =ipPort;
          }
      /*
       *填充编码器类型设备表 m_bmq_device_to_ip_port
       */
          if(device_bmq == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_bmq_device_to_ip_port[strIp] =ipPort;
          }
      /*
       *填充电压电流 m_dydl_to_ip_port
       */
          if(device_dydl == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_dydl_to_ip_port[strIp] =ipPort;
          }

       }


      //填充client map
      for(int index = 0; index < m_config_list_num[config_client_config];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_client_config][index]))));
              QString strIp = (*(m_config_list[config_client_config][index]))[client_config_address];
              m_clientMap_ipMap[strIp]=p ;

        }





     //填充继电器 ip 与 关联设备
     //CAN模块相关配置
     QString strFirstCol,strID;
     for(int index = 0; index <m_config_list_num[config_device_config]; ++index)
     {
         strFirstCol = (*m_config_list[config_device_config][index])[0];
         strID = strFirstCol.left(4);
        //继电器相关配置

         if(m_deviceID_map.contains(strID)
                 && device_jdq == m_deviceID_map[strID])
         {
             QDomDocument doc;
             QString error;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //保存根节点
             QString strRootID = firstRoot.attribute(QString("ID"));
             child = firstRoot.firstChildElement(QString("DO_DATA")); //DO_DATA 节点开始
             child = child.firstChildElement(); //DO_DATA 节点的第一个子节点开始

             while(!child.isNull())  //DO 节点
             {
                 QString strID = child.attribute(QString("ID"));
                 QString strLink = child.attribute(QString("LINK"));
                 m_deviceIp_byID_map[strRootID+strLink] = m_deviceID_to_ip_map[strRootID];
                 m_deviceID_to_do_map[strRootID+strLink] = strID;
                 child = child.nextSiblingElement();
              }

             child = firstRoot.firstChildElement(QString("DI_DATA")); //DI_DATA 节点开始
             child = child.firstChildElement(); //DI_DATA 节点的第一个子节点开始

             while(!child.isNull())  //DI 节点
             {
                 QString strID = child.attribute(QString("ID"));
                 QString strLink = child.attribute(QString("LINK"));
                 m_deviceIp_byID_map[strRootID+strLink] = m_deviceID_to_ip_map[strRootID];
                 m_deviceID_to_di_map[strRootID+strLink] = strID;
                 child = child.nextSiblingElement();
              }
            }

         //CAN模块相关配置
         if(m_deviceID_map.contains(strID)
                 && device_can == m_deviceID_map[strID])
        {
             CANDATA data;
             QDomDocument doc;
             QString error;
              int row = 0, column = 0;
              QString strTemp = (*m_config_list[config_device_config][index])[1];
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
             {

                 return false;
             }
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //保存根节点
             QString strRootID = firstRoot.attribute(QString("ID"));
             child = firstRoot.firstChildElement("cannum");
             if(!child.isNull())
             data.cannum = child.text().toInt();

             child = firstRoot.firstChildElement("connect");
             if(!child.isNull())
             data.connect = child.text().toInt();

             child = firstRoot.firstChildElement("devind");
             if(!child.isNull())
             data.devind = child.text().toInt();

             child = firstRoot.firstChildElement("port");
             if(!child.isNull())
             data.port = child.text().toInt();

             data.devtype = VCI_CANETTCP;
             data.strIp = m_deviceID_to_ip_map[strRootID];
             m_CANIp_to_config[data.strIp] = data;
        }
         //编码器
         if(m_deviceID_map.contains(strID)
                 && device_bmq == m_deviceID_map[strID])
         {
             QDomDocument doc;
             QString error;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //保存根节点
             QString strRootID = firstRoot.attribute(QString("ID"));
             BMQCOf data;
             child = firstRoot.firstChildElement("d");
             if(!child.isNull())
                   data.m_d = child.text().toDouble();
             child = firstRoot.firstChildElement("p");
             if(!child.isNull())
                   data.m_p = child.text().toDouble();
            m_bmqID_to_config[strRootID] = data;
         }

         //设备 外的配置项
         if(!m_deviceID_map.contains(strID))
         {
             QDomDocument doc;
             QString error,strData;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //保存根节点
             QString strRootID = firstRoot.attribute(QString("ID"));
             if(CONFIG_DHXZ == strRootID)
             {
                 child = firstRoot.firstChildElement("DhxzID");
                 if(!child.isNull())
                      strData = child.text();
                 UserBuffer::GetInstance()->WriteDhxzBuffer(strData);
             }

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
   if(0 == flag)
       return m_devcice_ipMap;

  // if(1 == flag)
        return m_clientMap_ipMap;


}


/*
 *返回 指定设备的 所属电梯ID
 */
QString     Config::GetLifterIDByDeviceIp(const QString &strIp)
{
    if(m_devcice_ipMap.contains(strIp))
    {
       return m_devcice_ipMap[strIp]->at(observe_control_device_belongs); //所属电梯

//        QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_clientMap_ipMap.constBegin();
//        for(; it != m_clientMap_ipMap.constEnd(); ++it)
//        {
//            if(it.value()->at(client_config_belongs) == strBelongs)  //设备所属电梯  相应的 管理client
//            {
//                return it.value()->at(client_config_address);
//            }
//        }
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

     if( map_database == mapType)
        return m_databaseMap;

     if(map_database_gss == mapType)
         m_databaseMap_gss;

     if(map_device== mapType)
        return m_deviceMap;


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
 QString Config::GetDeviceID(const QString &strIp)
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
 int Config::GetDeviceFlag(const QString &strIp)
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
 QString Config::GetDeviceConfig(QString const& strID)
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
  * strID 为继电器ID加 DI DO 关联的设备ID
  */
 QString Config::GetDeviceIpByID(QString const& strClientIp,const QString &strID)
 {
     if(!m_clientMap_ipMap.contains(strClientIp))
         return "";
     QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //所属电梯
     QString strDeviceId;
      QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
      for(; it != m_devcice_ipMap.constEnd(); ++it)
      {
          if(it.value()->at(observe_control_device_belongs) == strBelongs)  //设备所属电梯  相应的 管理client
          {
              strDeviceId = it.value()->at(observe_control_device_id);
              if(m_deviceIp_byID_map.contains(strDeviceId+strID))
                  return m_deviceIp_byID_map[strDeviceId+strID];
          }
      }
//     if(m_deviceIp_byID_map.contains(strID))
//        return m_deviceIp_byID_map[strID];
     return "";
 }

 /*
  * 指定关联设备id 返回 继电器的DI 口
  * trID 为继电器ID加 DI DO 关联的设备ID
  */
 QString Config::GetDIByDeviceID(QString const& strClientIp,QString const& strID)
 {
     if(!m_clientMap_ipMap.contains(strClientIp))
         return "";
     QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //所属电梯
     QString strDeviceId;
      QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
      for(; it != m_devcice_ipMap.constEnd(); ++it)
      {
          if(it.value()->at(observe_control_device_belongs) == strBelongs)  //设备所属电梯  相应的 管理client
          {
              strDeviceId = it.value()->at(observe_control_device_id);
              if(m_deviceID_to_di_map.contains(strDeviceId+strID))
                  return m_deviceID_to_di_map[strDeviceId+strID];
          }
      }
//    if(m_deviceID_to_di_map.contains(strID))
//        return m_deviceID_to_di_map[strID];
    return "";
 }

 /*
  * 指定关联设备的id 返回 继电器的DO口号
  * strID 为继电器ID+ DI DO 关联的设备ID
  * strClientIp 客户端ip 获取到设备ID
  *
  */
// QMap<QString,boost::shared_ptr<QStringList>> m_devcice_ipMap; //设备ip 映射 设备list
//         QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip 映射list
 QString Config::GetDOByDeviceID(QString const& strClientIp,QString const& strID)
 {
    if(!m_clientMap_ipMap.contains(strClientIp))
        return "";
    QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //所属电梯
    QString strDeviceId;
     QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
     for(; it != m_devcice_ipMap.constEnd(); ++it)
     {
         if(it.value()->at(observe_control_device_belongs) == strBelongs)  //设备所属电梯  相应的 管理client
         {
             strDeviceId = it.value()->at(observe_control_device_id);
             if(m_deviceID_to_do_map.contains(strDeviceId+strID))
                 return m_deviceID_to_do_map[strDeviceId+strID];
         }
     }
    return "";

 }


 /*
  * 获取CAN模块相关配置MAP
  */
 QMap<QString,CANDATA>& Config::GetCANConfigMap()
 {
    return m_CANIp_to_config;
 }

 /*
  * 继电器类型设备表  m_jdq_device_to_ip_port
  */
 QMap<QString,ip_port>const& Config::GetTypeDeviceMap(int flag)
 {
     if(device_jdq == flag)  //继电器设备
     return m_jdq_device_to_ip_port;

     if(device_cgq == flag) //水平传感器设备
      return m_cgq_device_to_ip_port;

     if(device_bmq == flag) //编码器
         return m_bmq_device_to_ip_port;

     if(device_dydl == flag) //电压电流 传感器设备
     return m_dydl_to_ip_port;
 }

// /*
//  * 传感器类型设备表  m_cgq_device_to_ip_port
//  */
// QMap<QString,ip_port>const& Config::GetCgqTypeDeviceMap()
// {
//     return m_cgq_device_to_ip_port;
// }
// /*
//  *编码器类型设备表 m_bmq_device_to_ip_port
//  */
// QMap<QString,ip_port>const& Config::GetBmqTypeDeviceMap()
// {
//    return m_bmq_device_to_ip_port;
// }

 /*
  * 返回客户端的ID
  * 参数 客户端ip
  */
QString  Config::GetClientIDByIp(QString const& strIp)
{
    if(!m_clientMap_ipMap.contains(strIp))
        return "";
    return m_clientMap_ipMap[strIp]->at(client_config_id);
}


/*
 * 返回客户端绑定的电梯ID
 * 参数 客户端ip
 * 返回参数为 电梯ID   双笼的 对应两台电梯, 两套设备
 */
QString Config::GetLifterIDByClientIp(QString const& strIp)
{
    if(!m_clientMap_ipMap.contains(strIp))
        return "";

    return m_clientMap_ipMap[strIp]->at(client_config_belongs);
}
/*
 * 制动
 * 返回 传感器 对应的 编码器ip
 */
QString Config::GetCgqToBmqIp(QString const& strIp)
{
    return "";
}
/*
 * 获取指定ip 编码器的配置项
 *
 */
BMQCOf   Config::GetBmqConfig(QString const& strIp)
{
    BMQCOf data;
    data.m_d = -1;
    data.m_p = -1;
    QString strID = GetDeviceID(strIp);
    if(m_bmqID_to_config.contains(strID))
    {
       return  m_bmqID_to_config[strID];
    }
    return data;
}
/*
 * 返回数据库 配置信息
 */
QString Config::GetDBConfigData(int flag)
{
    //database_dbName
    if(m_databaseMap.contains(flag))
        return m_databaseMap[flag];
    return "";
}

/*
 * 返回指定电梯ID 指定设备类型的 设备ID list
 */
 QMap<int,QString>   Config::GetDeviceIDListByLifterID(QString const& strLifterID,int flag)
{
     QMap<int,QString>  retMap;
    QString strID ,strBelongs;
    for(int index = 0; index < m_config_list_num[config_observe_control_device];++index)
      {
             strID = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_id];
             strBelongs = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_belongs];
             if(strLifterID == strBelongs && strID.left(4).toInt() == flag)
             {
                retMap[strID.right(2).toInt()] = strID;
             }
      }
    return retMap;
}
 /*
  * 获取客户端编码器 与 关联电梯的 配置数据包  更改 内存配置
  */
 void   Config::SetLifterConfig(MemoryConfig const& config)
 {
    if(m_bmqID_to_config.contains(config.m_deviceID))
         m_bmqID_to_config[config.m_deviceID] = config.m_bmqcof;
    if(m_clientMap_ipMap.contains(config.m_clientIp))//[config.m_clientIp]
    {
        //boost::shared_ptr<QStringList>& list = m_clientMap_ipMap[config.m_clientIp];//[config.m_clientIp]= config.m_strBelongs;
        (*(m_clientMap_ipMap[config.m_clientIp]))[client_config_belongs]= config.m_strBelongs;
    }

 }
