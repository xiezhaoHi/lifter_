#ifndef CONFIG_H
#define CONFIG_H
#include<QMap>
#include<enum.h>
#include<boost/shared_ptr.hpp>
/*配置类
 * 实现配置文件的读写
 * 初始化 用户登录信息表 与 数据库登录信息表的 初始化工作
 */
class Config
{
public:
    static Config* m_singleton;
    /*获取单实例*/
    static Config* GetInstance(void);

    /*配置初始化
   *填充3个 信息map
   *m_deviceMap     m_clientMap m_databaseMap
    */
    bool InitConfig(void);

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
     *返回 指定设备的 client ip
     */
    QString     GetDeviceIp(QString strIp);

    /*
     * 获取设备 ID
     * 设备ip作为参数
     */
    QString GetDeviceID(QString strIp);

    /*
     * 返回设备标识
     */
    int GetDeviceFlag(QString strIp);

    /*
     *返回 指定设备ID的 配置信息
     */
    QString GetDeviceConfig(QString strID);

    /*
     *指定 继电器关联设备ID 返回 继电器IP
     */
    QString GetDeviceIpByID(QString strID);

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

    ~Config()
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
    }

private:

   Config();
    /*设备 信息*/

    QMap<int,QString>  m_deviceMap;
    /*客户端信息*/
    QMap<int,QString>  m_clientMap;

    /*数据库配置信息*/
    QMap<int,QString> m_databaseMap;

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
    QMap<QString,boost::shared_ptr<QStringList>> m_devcice_ipMap; //设备ip 映射 设备list
        QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip 映射list

      QMap<QString,QString>     m_deviceID_to_ip_map; //设备表 设备ID映射到ip
      QMap<QString,QString>     m_deviceIp_byID_map; //通过 继电器关联的 设备ID 返回继电器的IP
    /***********************************************************/
};

#endif // CONFIG_H
