#ifndef CONFIG_H
#define CONFIG_H
#include<QMap>
#include<enum.h>
#include<boost/shared_ptr.hpp>
/*������
 * ʵ�������ļ��Ķ�д
 * ��ʼ�� �û���¼��Ϣ�� �� ���ݿ��¼��Ϣ��� ��ʼ������
 */
class Config
{
public:
    static Config* m_singleton;
    /*��ȡ��ʵ��*/
    static Config* GetInstance(void);

    /*���ó�ʼ��
   *���3�� ��Ϣmap
   *m_deviceMap     m_clientMap m_databaseMap
    */
    bool InitConfig(void);

    /*
     * ��ʼ�� �豸������Ϣ
     */
    bool InitDeviceConfig();

    /*
     * ��ȡ �豸 �ͻ��� ע����Ϣ
     * flag  0 ��ʾ �豸  1��ʾ client �û�
     */
    QMap<QString,boost::shared_ptr<QStringList>>& GetUserInfo(int flag=0);

    /*
     *���� ָ���豸�� client ip
     */
    QString     GetDeviceIp(QString strIp);

    /*
     * ��ȡ�豸 ID
     * �豸ip��Ϊ����
     */
    QString GetDeviceID(QString strIp);

    /*
     * �����豸��ʶ
     */
    int GetDeviceFlag(QString strIp);

    /*
     *���� ָ���豸ID�� ������Ϣ
     */
    QString GetDeviceConfig(QString strID);

    /*
     *ָ�� �̵��������豸ID ���� �̵���IP
     */
    QString GetDeviceIpByID(QString strID);

/*********************************************************************************/
    /*
    *��ȡ������Ϣ
    */
    QString GetErrInfo(void);

    /*
     *
     * ��ȡ�豸��Ϣmap
     * ����0 1 2 �ֱ��ʾ
     * �豸��Ϣ
     * �ͻ�����Ϣ
     * ���ݿ���Ϣ
     */
    QMap<int,QString> & GetMap(int);

    /*
     *
     * ��ȡservice port
     */
    int GetServicePort(void);



    /*
     * ����m_config_list
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
    /*�豸 ��Ϣ*/

    QMap<int,QString>  m_deviceMap;
    /*�ͻ�����Ϣ*/
    QMap<int,QString>  m_clientMap;

    /*���ݿ�������Ϣ*/
    QMap<int,QString> m_databaseMap;

    /*�����port����
     */
    int     m_service_port;

    /*��¼������Ϣ*/
    QString m_error;

    /*
     *
     * �����ݿ� ��ȡ �豸��ϸ��������Ϣ
     *
     */
    QStringList**    m_config_list[config_max];
    int                     m_config_list_num[config_max];
    QMap<QString, int> m_deviceID_map;  //���� �豸id ��Ӧ�� enum
    QMap<QString,boost::shared_ptr<QStringList>> m_devcice_ipMap; //�豸ip ӳ�� �豸list
        QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip ӳ��list

      QMap<QString,QString>     m_deviceID_to_ip_map; //�豸�� �豸IDӳ�䵽ip
      QMap<QString,QString>     m_deviceIp_byID_map; //ͨ�� �̵��������� �豸ID ���ؼ̵�����IP
    /***********************************************************/
};

#endif // CONFIG_H
