#ifndef CONFIG_H
#define CONFIG_H
#include<QMap>
#include<enum.h>
#include<boost\shared_ptr.hpp>
#include"server/ccanmoduleserver.h"
/*配置类
 * 实现配置文件的读写
 * 初始化 用户登录信息表 与 数据库登录信息表的 初始化工作
 */

typedef struct struct_ip_port
{
    QString m_strIp;
    int  m_strPort;
}ip_port;

//编码器 配置项
typedef struct struct_bmq_config
{
	//旧编码器
    double m_d; //半径
    double m_p; //刻度

	//新编码器
	double m_pLen; //每个刻度的长度 cm
}BMQCOf;


//设置内存配置
typedef struct
{
    QString m_clientIp;
    QString m_strBelongs;
    QString m_deviceID;
    BMQCOf m_bmqcof;
}MemoryConfig;

class Config
{
public:
    static Config* m_singleton;

    /*
     * 获取客户端编码器 与 关联电梯的 配置数据包  更改 内存配置
     */
    void   SetLifterConfig(MemoryConfig const& config);



    /*
     * 返回指定电梯ID的设备ID map
	 * strLifterID:电梯ID
	 * flag:设备类型
     */
    QMap<int,QString>  GetDeviceIDListByLifterID(QString const& strLifterID, int flag);

/*********************************************************************************/
    /*
     * 返回数据库 配置信息
	 * flag:标识
     */
    QString GetDBConfigData(int flag);


    /*
     * 获取指定ip 编码器的配置项
     *
     */
    BMQCOf GetBmqConfig(QString const& );

    /*
     * 制动
     * 返回 传感器id 对应的 编码器id
     */
    QString GetCgqToBmqID(QString const& strId);


    /*
     * 返回客户端的ID
     * 参数 客户端ip
     */
   QString  GetClientIDByIp(QString const& strIp);

   /*
    * 返回客户端绑定的电梯ID
    * 参数 客户端ip
    */
  QString GetLifterIDByClientIp(QString const& strIp);

   /*********************************************************************************/
    /*获取单实例*/
    static Config* GetInstance(void);

    /*配置初始化
   *填充3个 信息map
   *m_deviceMap     m_clientMap m_databaseMap
    */
    bool InitConfig(QString strPath);

    /*
     * 初始化 设备配置信息
     */
    bool InitDeviceConfig();

    /*
     * 获取 设备 客户端 注册信息
     * flag  0 表示 设备  1表示 client 用户
     */
    QMap<QString,boost::shared_ptr<QStringList>>& GetUserInfo(int flag=0);

    /*
     *返回 指定设备的 所属电梯ID
     * 参数为设备IP
     *
     */
    QString     GetLifterIDByDeviceId(QString const& strIp);


    /*
     * 获取设备 ID
     * 设备ip作为参数
     */
    QString GetDeviceID(QString const& );

    /*
     * 返回设备标识
     */
	QString GetDeviceFlag(QString const& );

    /*
     *返回 指定设备ID的 配置信息
     */
    QString GetDeviceConfig(QString const& strID);

    /*
     *指定 继电器关联设备ID 返回 继电器ID
     */
    QString GetDeviceIDByID(const QString &, QString const& strID);

    /*
     * 指定关联设备id 返回 继电器的DI 口
     */
    QString GetDIByDeviceID(QString const& ,QString const& strID);

    /*
     * 指定关联设备的id 返回 继电器的DO口号
     */
    QString GetDOByDeviceID(const QString &, QString const& strID);
/*********************************************************************************/
    /*
    *获取错误信息
    */
    QString GetErrInfo(void);

    /*
     *
     * 获取设备信息map
     * 参数0 1 2 分别表示
     * 设备信息
     * 客户端信息
     * 数据库信息
     */
    QMap<int,QString> & GetMap(int);

    /*
     *
     * 获取service port
     */
    int GetServicePort(void);



    /*
     * 操作m_config_list
     */

    ~Config();
/***************************************************/


    //CAN 设备相关函数

    /*
     * 获取CAN模块相关配置MAP
     */
    QMap<QString,CANDATA>& GetCANConfigMap();


    /***************************************************/

    /*
     * 返回指定设备类型的 ip port配置信息
     * flag 指定设备类型   device_name_config
     */

    QMap<QString,ip_port>const& GetTypeDeviceMap(int flag);
    /***************************************************/

private:

   Config();

   /*
    * 编码器 配置
    */
    QMap<QString,BMQCOf>    m_bmqID_to_config;



   /*
    * CAN模块设备相关配置
    */
    QMap<QString,CANDATA>   m_CANIp_to_config; //CAN模块设备IP与对应的配置
    QMap<QString,QString>   m_CANIp_to_ClientIp; //CAN模块设备IP与对应的客户端IP


   /*********************************************************/

    /*设备 信息*/
    QMap<int,QString>  m_deviceMap;
    /*客户端信息*/
    QMap<int,QString>  m_clientMap;

    /*程序数据库配置信息*/
    QMap<int,QString> m_databaseMap;

    /*钢丝绳数据库配置信息*/
    QMap<int,QString> m_databaseMap_gss;

    /*服务端port配置
     */
    int     m_service_port;

    /*记录错误信息*/
    QString m_error;

    /*
     *
     * 从数据库 读取 设备详细的配置信息
     *
     */
    QStringList**    m_config_list[config_max];
    int                     m_config_list_num[config_max];
    QMap<QString, int> m_deviceID_map;  //保存 设备id 对应的 enum
    QMap<QString,boost::shared_ptr<QStringList>> m_devcice_idMap; //设备id 映射 设备list
    QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip 映射list

	QMap<QString,QString>     m_device_IPPort_ID_map; //设备表 设备ip+port 获取设备ID
    QMap<QString,QString>     m_deviceID_to_id_map; //设备表 设备ID映射到设备ip
    QMap<QString,QString>     m_deviceIp_byID_map; //通过 继电器关联的 设备ID 返回继电器的IP
    QMap<QString,QString>     m_deviceID_to_di_map; //设备号对应的DI号
    QMap<QString,QString>     m_deviceID_to_do_map; //设备号对应的DO号
    /***********************************************************/
    //2017/6/7

      //设备IP 对应 设备ip port结构体
      /*
       * 继电器类型设备
       */
      QMap<QString,ip_port>  m_jdq_device_to_ip_port;

      /*
       * 传感器类型设备
       */
       QMap<QString,ip_port>  m_cgq_device_to_ip_port;


       /*
        * 编码器类型设备
        */
        QMap<QString,ip_port>  m_bmq_device_to_ip_port;

      /*
       * client类型map
       */
      QMap<QString,ip_port> m_client_to_ip_port;
      /*
       *电压 电流 传感器设备
       */
      QMap<QString,ip_port> m_dydl_to_ip_port;

      /*
       *钢丝绳 数据采集
       */
      QMap<QString,ip_port> m_gss_to_ip_port;
};

#endif // CONFIG_H
