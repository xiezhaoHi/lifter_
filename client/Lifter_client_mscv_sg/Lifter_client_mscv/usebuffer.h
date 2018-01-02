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

/*buffer 集中管理*/
class UserBuffer
{
public:
    /*
     * 服务端回复 应答 结果队列
     */
    void    PushServerResponse(int const& res);

    /*
     * 返回 -1 没有数据 0 失败 1 成功
     */
    int     PopServerResponse();

    /****************************************************/
    /*
     * 增加一个上下线提示
     *
     * list 包含数据
     * 0.上下线标识  上线 下线
     * 1.用户标识
     * 2.登录ip
     * 3.登录port
     *
     */
    QQueue<QStringList>     m_userDL_queue;
    boost::mutex      m_mutex_userDL;
   boost::condition  m_cond_userDL;
   bool     PushUserDLQueue(QStringList const& list);
   QStringList    PopUserDLQueue();

    /*
     * 数据库 操作的互斥锁
     */

    //测试读取缓冲区
    bool TestRead_buffer();

//写buffer
     bool Write_buffer(int deviceID,char*buff, int len);
   //读buffer
     bool Read_buffer(int deviceID,char* buff,int len);

  //传感器 写数据纠正 buffer
     bool Write_buffer_jz(char* buff,int len);
     //读传感器数据纠正buffer
       bool Read_buffer_jz(char* buff,int len);


   //读数据库任务队列
     QString PopDatabaseQueue();

     //写数据库任务队列
     void PushDatabaseQueue(int flag, QString  const &strSql);

     int  ReturnDatabaseQueueSize();


    /*
     * client 数据队列 处理函数
     */

    void PushClientServerQueue(QString const& strData);

    QString PopClientServerQueue();

    int ReturnClientServerQueueSize();


    static UserBuffer* GetInstance(void);
private:
    UserBuffer();

    static UserBuffer* m_singleton;

    /*
     * 服务端回复 应答 结果队列
     */
    QQueue<int>     m_server_response;
    boost::mutex    m_server_response_mutex;

    /****************************************************/
    /*
     * 客户端服务 数据队列
     */
    QQueue<QString>    m_clientServer_queue;
    boost::mutex      m_mutex_client;
   boost::condition  m_cond_client;

    /*
     * 数据库任务队列
     */
     QQueue<QString> m_database_queue;
     boost::mutex      m_mutex_DB;
    boost::condition  m_cond_DB;


    /*
     * 每种需要的数据 分开存储
     */
    int         m_maxLen[detection_max];
    QString m_strSql[detection_max];
    int         m_iSqlCount[detection_max];



     /*
      *
      * 管理所有设备 需要的
      * 缓冲区
      * 互斥锁
      * 条件变量
      */
     CCycleBuffer   m_deviceData_buff[device_max]; //建立一个 缓冲区 数组
    //QMap<int,CCycleBuffer>  m_deviceData_buff;
    boost::condition  m_deviceData_cond[device_max];
  //  QMap<int ,boost::condition> m_deviceData_cond;
    boost::mutex      m_deviceData_mutex[device_max];
   // QMap<int ,boost::mutex> m_deviceData_mutex;



};

#endif // USEBUFFER_H
