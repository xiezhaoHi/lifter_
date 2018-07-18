#ifndef USEBUFFER_H
#define USEBUFFER_H
#include"CCycleBuffer.h"
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost\lexical_cast.hpp>
#include<boost\thread/condition.hpp>
#include <boost\thread.hpp>
#include<QWaitCondition>
#include<QMap>
#include<QQueue>
#include"enum.h"
typedef struct
{
    int data_id;
    char* data_begin;
    int data_len;
}recv_data;

typedef struct
{
    QString strIp;
    QString strData;
}response_data;
//canģ��ÿ��ͨ�������ݽṹ

#define FIFO_NUM 6
#define FIFO_LEN 1024
#define CAN_READ_FRAME_NUM 1024
typedef  struct  _VCI_CAN_BUF {
    BYTE	Data[8];
}VCI_CAN_BUF_T;
typedef struct device_buf
{
    UINT32 id;
    VCI_CAN_BUF_T BUF[FIFO_LEN];
    UINT16 cnt;
}device_buf_T;


//������ ���ݳ��ȶ���
#define  DATA_SIZE_CGQ  22 //�������������ݵĳ���  �������ݲ������һ�����������ݰ�
#define  SEND_MAX_DATA 4096

//�������ݽṹ
typedef struct  SendMessageData
{
    char buff[SEND_MAX_DATA];
    int len;
}SendData;

/*buffer ���й���*/
class UserBuffer
{
public:
    /*
     * �ظ� �ͻ��˶��� ��д
     * m_response_queue
     */
    void    PushResponseQueue(const QString &strIp, response_data const& data);

    /*
     * strIp  �ͻ���ip
     * ���� �ظ��� ���ݰ�
     */
    QString PopResponseQueue(QString const& strIp);


    /**************************************************/
    /*
     * ����ѡ�� ��¼ ѡ��� ����ID
     */
    void    WriteDhxzBuffer(QString const& );
    QString ReadDhxzBuffer();


    /**************************************************/

    /*
     * ��ȡ��������ؼ���ֵ
     * strip ������ip
     * flag  ����λ ����λ ÿ��ı�ʶ
     * jsz ����ֵ
     */
    void    SetBmqJsz(QString const& strIp,int const& flag,int const& jsz);

    /*
     *��ȡ ָ�������� ָ��λ�õ� ��ʼ ����ֵ
     */

    int     GetBmqJsz(QString const& strIp,int const& flag);


    /*
     * ���� ��ǰ ������ ���µļ���ֵ
     */
    void    SetBmqNewestJsz(QString const& strIp,int const& jsz);

    /*
     * ��ȡ ָ�������� ���µļ���ֵ
     */
    int     GetBmqNewestJsz(QString const& strIp);


    /*
     * ��¼ �ƶ� ��ʼʱ ����ֵ
     * strID ���� ID
     */
    void     SetBmqBeginZdJsz(QString const& strIp,int const& jsz);

    /*
     * ��ȡ �ƶ� ��ʼʱ ����ֵ
     */
    int      GetBmqBeginZdJsz(QString const& strIp);

    /*
     * ��ȡ �ƶ�����
     */
    double  GetBmqZdjl(QString const& strIp);

    /**************************************************/
    /*
     * can ģ�����ݶ��д���  m_can_device_queue
     */

    void  PushCanDeviceQueue(device_buf_T const& buffT);

    device_buf_T PopCanDeviceQueue();

  /**************************************************/
    //���� ���� ���ݰ� ��д
    /*
     *д������   m_device_to_dh_data
     */
    void WriteDeviceToDhData(QString const& strIp, QString const& strData);

    /*
     * ����ָ�� �ͻ��� �Ķ��� ���ݰ�
     */
    QString  ReadDeviceToDhData(QString const& strLifterID);

  /**************************************************/
    /*
     *д������   m_device_to_client_map
	 *���� Ϊ strIp �豸IP 
	 *strData  ����
     */
    void WriteDeviceToClientMap(QString const& strIp, QString const& strData);

    /*
     * �������� m_device_to_client_map
     * ���� Ϊ ���� ID
     */
    QStringList ReadDeviceToClientMap(QString const& strLifterID);

    /**********************************************************/
    /*
     * �̵���ָ�����
     */
    //�豸ID ��Ӧ�� �������� list
    QMap<QString,boost::shared_ptr<QList<SendData>>>  m_sendList;
    QMap<QString,QList<SendData>> m_testList;
    //QList<SendData> m_sendList;
    boost::recursive_mutex        m_sendList_mutex;
    bool    IsEmptySendList(QString const& strID);
    SendData &FrontSendList(const QString &strID);
    void    PushBackSendList(const QString &strID, SendData const& list);
    void    PopFrontSendList(const QString &strID);
    void    ClearFrontSendList();

    /*
     *�û���¼��Ϣ
     *list:
     *0: ������ ��ʶ
     * 1: �û�
     * 2: ip
     * 3: port
     */
    QQueue<QStringList> m_userDL_queue;
    boost::mutex      m_mutex_userDL;
   boost::condition  m_cond_userDL;
   void     PushUserDLData(QStringList const& list);
   QStringList PopUserDLData();
    bool      IsEmptyUserDLData();
/**************************************************/


    //���Զ�ȡ������
    bool TestRead_buffer();

//дbuffer
     bool Write_buffer(int deviceID,char*buff, int len);
   //��buffer
     bool Read_buffer(int deviceID,char* buff,int len);

  //������ д���ݾ��� buffer
     bool Write_buffer_jz(char* buff,int len);
     //�����������ݾ���buffer
       bool Read_buffer_jz(char* buff,int len);

/**************************************************/

   /*�����ݿ��������
    *
    */
     QString PopDatabaseQueue();

     //д���ݿ��������
     void PushDatabaseQueue(int flag, QString  const &strSql);

     //д���ݿ����

     void PushDatabaseQueueAll(QString const& strSql,int flag=config_device_config);

     int  ReturnDatabaseQueueSize();

     bool   IsEmptyDatabaseQueue();

/**************************************************/
    static UserBuffer* GetInstance(void);

        ~UserBuffer();
    /*
     * �������,����ʣ��� sql ����
     */
    void      ClearBuff();
    /*
     * ��ʼ�� m_strSql ����
     * ����Ϊ
     * flag ��ʾ�������� ����:device_jdq
     * strDBName ��ʾ ���ݿ�����
     * strTableName ������
     */
    void   InitDBSqlBuff(int flag,QString const& strDBName,QString const& strTableName);

    /*
     * ��ʼ�� m_strSql ����
     * ����Ϊ
     * flag ��ʾ�������� ����:device_jdq
     * strDBName ��ʾ ���ݿ�����
     * strTableName ������
     */
    void   InitDBSqlBuffAll(int flag,QString const& strDBName,QString const& strTableName);


    UserBuffer();
private:


    static UserBuffer* m_singleton;

    /*
     * �ظ� ����
     */
    QMap<QString,response_data>   m_response_queue;
    boost::mutex            m_response_queue_mutex;


    /**************************************************/

 /*
  *����ѡ��  ��¼����ID
  */
    QString   m_dhxz_id;

/**************************************************/
/*
 * ������ ��� ����ֵ ����
 */
    //��������ʼ  ÿ��ļ���ֵ  ��¼��������Ӧ�ļ���ֵ
    QMap<QString,QStringList>   m_bmq_begin_jsz;
    //������ ���µļ���ֵ
    QMap<QString,int>   m_bmq_newest_jsz;
    //������ �ƶ���ʼʱ ����ֵ

    QMap<QString,int>   m_bmq_begin_zd_jsz;

    //�ƶ� ����
    QMap<QString,double>    m_bmq_zd_jl;

    //�ƶ� ���buff ͳһ ������
    boost::mutex        m_mutex_bmq_zd;
/**************************************************/
    /*
     *canģ�� ���ݶ���
     */
    QQueue<device_buf_T > m_can_device_queue;
    boost::mutex              m_mutex_can_device;

    /*
     * ���ݿ��������
     */
    //�������������������
     int      m_database_queue_max;
     QQueue<QString> m_database_queue;
     boost::mutex      m_mutex_DB;
    boost::condition  m_cond_DB;


    /*
     * ÿ����Ҫ������ �ֿ��洢
     */
    int         m_maxLen[device_max];
    QString m_strDBName;
    QString m_strTableName[device_max];
    QString m_strSql[device_max]; //�������ִ�е� sql ���
    QString m_strDefSql[device_max]; //�����ʼ���� sql���
    int         m_iSqlCount[device_max];
    QMap<int,QString> m_strSqlAll;


     /*
      *
      * ���������豸 ��Ҫ��
      * ������
      * ������
      * ��������
      */
     CCycleBuffer   m_deviceData_buff[device_max];
    //QMap<int,CCycleBuffer>  m_deviceData_buff;
    boost::condition  m_deviceData_cond[device_max];
  //  QMap<int ,boost::condition> m_deviceData_cond;
    boost::mutex      m_deviceData_mutex[device_max];
   // QMap<int ,boost::mutex> m_deviceData_mutex;


    /***************************************************************/
    //2017/6/7
    /*
     * �豸�����ͻ�������
     */
   // QMap<QString,QStringList>       m_device_to_client_map;
    boost::condition          m_device_to_client_map_cond;
    boost::mutex              m_device_to_client_map_mutex;
    /*
     *�洢��ͬip�豸����������
     */
    QMap<QString,QString>       m_device_to_data;
    /*
     * �洢��ͬ�豸ip �����ͻ�������Ҫ������
     */
    boost::condition          m_device_to_dh_data_map_cond;
    boost::mutex              m_device_to_dh_data_map_mutex;
    QMap<QString,QString>       m_device_to_dh_data;

};

#endif // USEBUFFER_H
