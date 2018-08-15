#ifndef CONFIG_H
#define CONFIG_H
#include<QMap>
#include<enum.h>
#include<boost\shared_ptr.hpp>
#include"server/ccanmoduleserver.h"
/*������
 * ʵ�������ļ��Ķ�д
 * ��ʼ�� �û���¼��Ϣ�� �� ���ݿ��¼��Ϣ��� ��ʼ������
 */

typedef struct struct_ip_port
{
    QString m_strIp;
    int  m_strPort;
}ip_port;

//������ ������
typedef struct struct_bmq_config
{
	//�ɱ�����
    double m_d; //�뾶
    double m_p; //�̶�

	//�±�����
	double m_pLen; //ÿ���̶ȵĳ��� cm
}BMQCOf;


//�����ڴ�����
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
     * ��ȡ�ͻ��˱����� �� �������ݵ� �������ݰ�  ���� �ڴ�����
     */
    void   SetLifterConfig(MemoryConfig const& config);



    /*
     * ����ָ������ID���豸ID map
	 * strLifterID:����ID
	 * flag:�豸����
     */
    QMap<int,QString>  GetDeviceIDListByLifterID(QString const& strLifterID, int flag);

/*********************************************************************************/
    /*
     * �������ݿ� ������Ϣ
	 * flag:��ʶ
     */
    QString GetDBConfigData(int flag);


    /*
     * ��ȡָ��ip ��������������
     *
     */
    BMQCOf GetBmqConfig(QString const& );

    /*
     * �ƶ�
     * ���� ������id ��Ӧ�� ������id
     */
    QString GetCgqToBmqID(QString const& strId);


    /*
     * ���ؿͻ��˵�ID
     * ���� �ͻ���ip
     */
   QString  GetClientIDByIp(QString const& strIp);

   /*
    * ���ؿͻ��˰󶨵ĵ���ID
    * ���� �ͻ���ip
    */
  QString GetLifterIDByClientIp(QString const& strIp);

   /*********************************************************************************/
    /*��ȡ��ʵ��*/
    static Config* GetInstance(void);

    /*���ó�ʼ��
   *���3�� ��Ϣmap
   *m_deviceMap     m_clientMap m_databaseMap
    */
    bool InitConfig(QString strPath);

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
     *���� ָ���豸�� ��������ID
     * ����Ϊ�豸IP
     *
     */
    QString     GetLifterIDByDeviceId(QString const& strIp);


    /*
     * ��ȡ�豸 ID
     * �豸ip��Ϊ����
     */
    QString GetDeviceID(QString const& );

    /*
     * �����豸��ʶ
     */
	QString GetDeviceFlag(QString const& );

    /*
     *���� ָ���豸ID�� ������Ϣ
     */
    QString GetDeviceConfig(QString const& strID);

    /*
     *ָ�� �̵��������豸ID ���� �̵���ID
     */
    QString GetDeviceIDByID(const QString &, QString const& strID);

    /*
     * ָ�������豸id ���� �̵�����DI ��
     */
    QString GetDIByDeviceID(QString const& ,QString const& strID);

    /*
     * ָ�������豸��id ���� �̵�����DO�ں�
     */
    QString GetDOByDeviceID(const QString &, QString const& strID);
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

    ~Config();
/***************************************************/


    //CAN �豸��غ���

    /*
     * ��ȡCANģ���������MAP
     */
    QMap<QString,CANDATA>& GetCANConfigMap();


    /***************************************************/

    /*
     * ����ָ���豸���͵� ip port������Ϣ
     * flag ָ���豸����   device_name_config
     */

    QMap<QString,ip_port>const& GetTypeDeviceMap(int flag);
    /***************************************************/

private:

   Config();

   /*
    * ������ ����
    */
    QMap<QString,BMQCOf>    m_bmqID_to_config;



   /*
    * CANģ���豸�������
    */
    QMap<QString,CANDATA>   m_CANIp_to_config; //CANģ���豸IP���Ӧ������
    QMap<QString,QString>   m_CANIp_to_ClientIp; //CANģ���豸IP���Ӧ�Ŀͻ���IP


   /*********************************************************/

    /*�豸 ��Ϣ*/
    QMap<int,QString>  m_deviceMap;
    /*�ͻ�����Ϣ*/
    QMap<int,QString>  m_clientMap;

    /*�������ݿ�������Ϣ*/
    QMap<int,QString> m_databaseMap;

    /*��˿�����ݿ�������Ϣ*/
    QMap<int,QString> m_databaseMap_gss;

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
    QMap<QString,boost::shared_ptr<QStringList>> m_devcice_idMap; //�豸id ӳ�� �豸list
    QMap<QString,boost::shared_ptr<QStringList>> m_clientMap_ipMap; //client ip ӳ��list

	QMap<QString,QString>     m_device_IPPort_ID_map; //�豸�� �豸ip+port ��ȡ�豸ID
    QMap<QString,QString>     m_deviceID_to_id_map; //�豸�� �豸IDӳ�䵽�豸ip
    QMap<QString,QString>     m_deviceIp_byID_map; //ͨ�� �̵��������� �豸ID ���ؼ̵�����IP
    QMap<QString,QString>     m_deviceID_to_di_map; //�豸�Ŷ�Ӧ��DI��
    QMap<QString,QString>     m_deviceID_to_do_map; //�豸�Ŷ�Ӧ��DO��
    /***********************************************************/
    //2017/6/7

      //�豸IP ��Ӧ �豸ip port�ṹ��
      /*
       * �̵��������豸
       */
      QMap<QString,ip_port>  m_jdq_device_to_ip_port;

      /*
       * �����������豸
       */
       QMap<QString,ip_port>  m_cgq_device_to_ip_port;


       /*
        * �����������豸
        */
        QMap<QString,ip_port>  m_bmq_device_to_ip_port;

      /*
       * client����map
       */
      QMap<QString,ip_port> m_client_to_ip_port;
      /*
       *��ѹ ���� �������豸
       */
      QMap<QString,ip_port> m_dydl_to_ip_port;

      /*
       *��˿�� ���ݲɼ�
       */
      QMap<QString,ip_port> m_gss_to_ip_port;
};

#endif // CONFIG_H
