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

    m_devcice_ipMap.clear(); //�豸ip ӳ�� �豸list
    m_clientMap_ipMap.clear();
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
bool Config::InitConfig(QString strPath)
{
   // QString strPath = qApp->applicationDirPath()+"\\config\\config.ini";

    QSettings iniRead(strPath,QSettings::IniFormat);
   // QString strHost = iniRead.value("SOCKET/host").toString();
   // int  port = iniRead.value("SOCKET/port").toInt();
      //�������ݿ�������Ϣ
    int maxNum = iniRead.value("database/database_max").toInt();
    QString strTemp ;
   for(int index = 0; index < maxNum; ++index)
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

   //��˿�����ݿ�������Ϣ
    maxNum = iniRead.value("database_gss/database_max").toInt();
   for(int index = 0; index < maxNum; ++index)
    {
       strTemp = iniRead.value(QString("database_gss/database_%1").arg(index)).toString();
       if(strTemp.isEmpty())
       {
           m_error ="##" + QString("database_gss/database_%1").arg(index) + "##������Ϊ��";
           return false;
        }
        m_databaseMap_gss[index] = strTemp;
        //!����
    }


   //��������Ϣ����
   strTemp = iniRead.value(QString("service/servvice_port")).toString();
   if(strTemp.isEmpty())
   {
       m_error ="##" + QString("service/servvice_port") + "##������Ϊ��";
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
              m_error = "ִ��sql������!";
            return false;
         }
        if(0 == m_config_list_num[index] && index != config_client_config)
        {
            m_error = "���ô���!###"+strSqlList[index];
            return false;
        }
     }

      //����豸map
      for(int index = 0; index < m_config_list_num[config_observe_control_device];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_observe_control_device][index]))));
              QString strIp = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_ip_address];
              QString strID = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_id];
              QString strPort = (*(m_config_list[config_observe_control_device][index]))[observe_control_device_port];
                          //ӳ�� ip �������豸����Ϣlist
              m_devcice_ipMap[strIp]=p ;

                          //ӳ�� ID �� �豸ip
              m_deviceID_to_ip_map[strID]= strIp;

       /*
        *���̵��������豸�� m_jdq_device_to_ip_port
        */
        if(device_jdq == m_deviceID_map[strID.left(4)])
        {
            ip_port ipPort;
            ipPort.m_strIp = strIp;
            ipPort.m_strPort = strPort.toInt();
            m_jdq_device_to_ip_port[strIp] =ipPort;
        }
      /*
       *��䴫���������豸�� m_cgq_device_to_ip_port
       */
          if(device_cgq == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_cgq_device_to_ip_port[strIp] =ipPort;
          }
      /*
       *�������������豸�� m_bmq_device_to_ip_port
       */
          if(device_bmq == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_bmq_device_to_ip_port[strIp] =ipPort;
          }
      /*
       *����ѹ���� m_dydl_to_ip_port
       */
          if(device_dydl == m_deviceID_map[strID.left(4)])
          {
              ip_port ipPort;
              ipPort.m_strIp = strIp;
              ipPort.m_strPort = strPort.toInt();
              m_dydl_to_ip_port[strIp] =ipPort;
          }

       }


      //���client map
      for(int index = 0; index < m_config_list_num[config_client_config];++index)
        {
             boost::shared_ptr<QStringList> p(new QStringList((*(m_config_list[config_client_config][index]))));
              QString strIp = (*(m_config_list[config_client_config][index]))[client_config_address];
              m_clientMap_ipMap[strIp]=p ;

        }





     //���̵��� ip �� �����豸
     //CANģ���������
     QString strFirstCol,strID;
     for(int index = 0; index <m_config_list_num[config_device_config]; ++index)
     {
         strFirstCol = (*m_config_list[config_device_config][index])[0];
         strID = strFirstCol.left(4);
        //�̵����������

         if(m_deviceID_map.contains(strID)
                 && device_jdq == m_deviceID_map[strID])
         {
             QDomDocument doc;
             QString error;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //������ڵ�
             QString strRootID = firstRoot.attribute(QString("ID"));
             child = firstRoot.firstChildElement(QString("DO_DATA")); //DO_DATA �ڵ㿪ʼ
             child = child.firstChildElement(); //DO_DATA �ڵ�ĵ�һ���ӽڵ㿪ʼ

             while(!child.isNull())  //DO �ڵ�
             {
                 QString strID = child.attribute(QString("ID"));
                 QString strLink = child.attribute(QString("LINK"));
                 m_deviceIp_byID_map[strRootID+strLink] = m_deviceID_to_ip_map[strRootID];
                 m_deviceID_to_do_map[strRootID+strLink] = strID;
                 child = child.nextSiblingElement();
              }

             child = firstRoot.firstChildElement(QString("DI_DATA")); //DI_DATA �ڵ㿪ʼ
             child = child.firstChildElement(); //DI_DATA �ڵ�ĵ�һ���ӽڵ㿪ʼ

             while(!child.isNull())  //DI �ڵ�
             {
                 QString strID = child.attribute(QString("ID"));
                 QString strLink = child.attribute(QString("LINK"));
                 m_deviceIp_byID_map[strRootID+strLink] = m_deviceID_to_ip_map[strRootID];
                 m_deviceID_to_di_map[strRootID+strLink] = strID;
                 child = child.nextSiblingElement();
              }
            }

         //CANģ���������
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
             QDomElement child = firstRoot; //������ڵ�
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
         //������
         if(m_deviceID_map.contains(strID)
                 && device_bmq == m_deviceID_map[strID])
         {
             QDomDocument doc;
             QString error;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //������ڵ�
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

         //�豸 ���������
         if(!m_deviceID_map.contains(strID))
         {
             QDomDocument doc;
             QString error,strData;
              int row = 0, column = 0;
             if(!doc.setContent((*m_config_list[config_device_config][index])[1], false, &error, &row, &column))
                 return false;
             QDomElement  firstRoot= doc.firstChildElement();
             QDomElement child = firstRoot; //������ڵ�
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
 * ��ȡ �豸 �ͻ��� ע����Ϣ
 * flag  0 ��ʾ �豸  1��ʾ client �û�
 */
QMap<QString,boost::shared_ptr<QStringList>>& Config::GetUserInfo(int flag )
{
   if(0 == flag)
       return m_devcice_ipMap;

  // if(1 == flag)
        return m_clientMap_ipMap;


}


/*
 *���� ָ���豸�� ��������ID
 */
QString     Config::GetLifterIDByDeviceIp(const QString &strIp)
{
    if(m_devcice_ipMap.contains(strIp))
    {
       return m_devcice_ipMap[strIp]->at(observe_control_device_belongs); //��������

//        QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_clientMap_ipMap.constBegin();
//        for(; it != m_clientMap_ipMap.constEnd(); ++it)
//        {
//            if(it.value()->at(client_config_belongs) == strBelongs)  //�豸��������  ��Ӧ�� ����client
//            {
//                return it.value()->at(client_config_address);
//            }
//        }
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

     if( map_database == mapType)
        return m_databaseMap;

     if(map_database_gss == mapType)
         m_databaseMap_gss;

     if(map_device== mapType)
        return m_deviceMap;


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
 QString Config::GetDeviceID(const QString &strIp)
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
  *���� ָ���豸ID�� ������Ϣ
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
  *ָ�� �̵��������豸ID ���� �̵���IP
  * strID Ϊ�̵���ID�� DI DO �������豸ID
  */
 QString Config::GetDeviceIpByID(QString const& strClientIp,const QString &strID)
 {
     if(!m_clientMap_ipMap.contains(strClientIp))
         return "";
     QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //��������
     QString strDeviceId;
      QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
      for(; it != m_devcice_ipMap.constEnd(); ++it)
      {
          if(it.value()->at(observe_control_device_belongs) == strBelongs)  //�豸��������  ��Ӧ�� ����client
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
  * ָ�������豸id ���� �̵�����DI ��
  * trID Ϊ�̵���ID�� DI DO �������豸ID
  */
 QString Config::GetDIByDeviceID(QString const& strClientIp,QString const& strID)
 {
     if(!m_clientMap_ipMap.contains(strClientIp))
         return "";
     QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //��������
     QString strDeviceId;
      QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
      for(; it != m_devcice_ipMap.constEnd(); ++it)
      {
          if(it.value()->at(observe_control_device_belongs) == strBelongs)  //�豸��������  ��Ӧ�� ����client
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
  * ָ�������豸��id ���� �̵�����DO�ں�
  * strID Ϊ�̵���ID+ DI DO �������豸ID
  * strClientIp �ͻ���ip ��ȡ���豸ID
  *
  */
// QMap<QString,boost::shared_ptr<QStringList>> m_devcice_ipMap; //�豸ip ӳ�� �豸list
//         QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip ӳ��list
 QString Config::GetDOByDeviceID(QString const& strClientIp,QString const& strID)
 {
    if(!m_clientMap_ipMap.contains(strClientIp))
        return "";
    QString strBelongs = m_clientMap_ipMap[strClientIp]->at(client_config_belongs); //��������
    QString strDeviceId;
     QMap<QString,boost::shared_ptr<QStringList>>::const_iterator it = m_devcice_ipMap.constBegin();
     for(; it != m_devcice_ipMap.constEnd(); ++it)
     {
         if(it.value()->at(observe_control_device_belongs) == strBelongs)  //�豸��������  ��Ӧ�� ����client
         {
             strDeviceId = it.value()->at(observe_control_device_id);
             if(m_deviceID_to_do_map.contains(strDeviceId+strID))
                 return m_deviceID_to_do_map[strDeviceId+strID];
         }
     }
    return "";

 }


 /*
  * ��ȡCANģ���������MAP
  */
 QMap<QString,CANDATA>& Config::GetCANConfigMap()
 {
    return m_CANIp_to_config;
 }

 /*
  * �̵��������豸��  m_jdq_device_to_ip_port
  */
 QMap<QString,ip_port>const& Config::GetTypeDeviceMap(int flag)
 {
     if(device_jdq == flag)  //�̵����豸
     return m_jdq_device_to_ip_port;

     if(device_cgq == flag) //ˮƽ�������豸
      return m_cgq_device_to_ip_port;

     if(device_bmq == flag) //������
         return m_bmq_device_to_ip_port;

     if(device_dydl == flag) //��ѹ���� �������豸
     return m_dydl_to_ip_port;
 }

// /*
//  * �����������豸��  m_cgq_device_to_ip_port
//  */
// QMap<QString,ip_port>const& Config::GetCgqTypeDeviceMap()
// {
//     return m_cgq_device_to_ip_port;
// }
// /*
//  *�����������豸�� m_bmq_device_to_ip_port
//  */
// QMap<QString,ip_port>const& Config::GetBmqTypeDeviceMap()
// {
//    return m_bmq_device_to_ip_port;
// }

 /*
  * ���ؿͻ��˵�ID
  * ���� �ͻ���ip
  */
QString  Config::GetClientIDByIp(QString const& strIp)
{
    if(!m_clientMap_ipMap.contains(strIp))
        return "";
    return m_clientMap_ipMap[strIp]->at(client_config_id);
}


/*
 * ���ؿͻ��˰󶨵ĵ���ID
 * ���� �ͻ���ip
 * ���ز���Ϊ ����ID   ˫���� ��Ӧ��̨����, �����豸
 */
QString Config::GetLifterIDByClientIp(QString const& strIp)
{
    if(!m_clientMap_ipMap.contains(strIp))
        return "";

    return m_clientMap_ipMap[strIp]->at(client_config_belongs);
}
/*
 * �ƶ�
 * ���� ������ ��Ӧ�� ������ip
 */
QString Config::GetCgqToBmqIp(QString const& strIp)
{
    return "";
}
/*
 * ��ȡָ��ip ��������������
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
 * �������ݿ� ������Ϣ
 */
QString Config::GetDBConfigData(int flag)
{
    //database_dbName
    if(m_databaseMap.contains(flag))
        return m_databaseMap[flag];
    return "";
}

/*
 * ����ָ������ID ָ���豸���͵� �豸ID list
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
  * ��ȡ�ͻ��˱����� �� �������ݵ� �������ݰ�  ���� �ڴ�����
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
