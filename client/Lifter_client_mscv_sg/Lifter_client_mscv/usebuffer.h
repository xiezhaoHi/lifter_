#ifndef USEBUFFER_H
#define USEBUFFER_H
#include"CCycleBuffer.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include<boost/thread/condition.hpp>
#include <boost/thread.hpp>
#include<QWaitCondition>
#include<QQueue>
#include"enum.h"
typedef struct
{
    int data_id;
    char* data_begin;
    int data_len;
}recv_data;

/*buffer ���й���*/
class UserBuffer
{
public:
    /*
     * ����˻ظ� Ӧ�� �������
     */
    void    PushServerResponse(int const& res);

    /*
     * ���� -1 û������ 0 ʧ�� 1 �ɹ�
     */
    int     PopServerResponse();

    /****************************************************/
    /*
     * ����һ����������ʾ
     *
     * list ��������
     * 0.�����߱�ʶ  ���� ����
     * 1.�û���ʶ
     * 2.��¼ip
     * 3.��¼port
     *
     */
    QQueue<QStringList>     m_userDL_queue;
    boost::mutex      m_mutex_userDL;
   boost::condition  m_cond_userDL;
   bool     PushUserDLQueue(QStringList const& list);
   QStringList    PopUserDLQueue();

    /*
     * ���ݿ� �����Ļ�����
     */

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


   //�����ݿ��������
     QString PopDatabaseQueue();

     //д���ݿ��������
     void PushDatabaseQueue(int flag, QString  const &strSql);

     int  ReturnDatabaseQueueSize();


    /*
     * client ���ݶ��� ������
     */

    void PushClientServerQueue(QString const& strData);

    QString PopClientServerQueue();

    int ReturnClientServerQueueSize();


    static UserBuffer* GetInstance(void);
private:
    UserBuffer();

    static UserBuffer* m_singleton;

    /*
     * ����˻ظ� Ӧ�� �������
     */
    QQueue<int>     m_server_response;
    boost::mutex    m_server_response_mutex;

    /****************************************************/
    /*
     * �ͻ��˷��� ���ݶ���
     */
    QQueue<QString>    m_clientServer_queue;
    boost::mutex      m_mutex_client;
   boost::condition  m_cond_client;

    /*
     * ���ݿ��������
     */
     QQueue<QString> m_database_queue;
     boost::mutex      m_mutex_DB;
    boost::condition  m_cond_DB;


    /*
     * ÿ����Ҫ������ �ֿ��洢
     */
    int         m_maxLen[detection_max];
    QString m_strSql[detection_max];
    int         m_iSqlCount[detection_max];



     /*
      *
      * ���������豸 ��Ҫ��
      * ������
      * ������
      * ��������
      */
     CCycleBuffer   m_deviceData_buff[device_max]; //����һ�� ������ ����
    //QMap<int,CCycleBuffer>  m_deviceData_buff;
    boost::condition  m_deviceData_cond[device_max];
  //  QMap<int ,boost::condition> m_deviceData_cond;
    boost::mutex      m_deviceData_mutex[device_max];
   // QMap<int ,boost::mutex> m_deviceData_mutex;



};

#endif // USEBUFFER_H
