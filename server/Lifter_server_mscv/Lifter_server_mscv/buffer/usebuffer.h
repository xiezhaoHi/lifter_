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
//can模块每个通道的数据结构

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


//编码器 数据长度定义
#define  DATA_SIZE_CGQ  22 //传感器两包数据的长度  两包数据才能组成一个完整的数据包
#define  SEND_MAX_DATA 4096

//发送数据结构
typedef struct  SendMessageData
{
    char buff[SEND_MAX_DATA];
    int len;
}SendData;

/*buffer 集中管理*/
class UserBuffer
{
public:
    /*
     * 回复 客户端队列 读写
     * m_response_queue
     */
    void    PushResponseQueue(const QString &strIp, response_data const& data);

    /*
     * strIp  客户端ip
     * 返回 回复的 数据包
     */
    QString PopResponseQueue(QString const& strIp);


    /**************************************************/
    /*
     * 动画选择 记录 选择的 电梯ID
     */
    void    WriteDhxzBuffer(QString const& );
    QString ReadDhxzBuffer();


    /**************************************************/

    /*
     * 获取编码器相关计数值
     * strip 编码器ip
     * flag  上限位 下限位 每层的标识
     * jsz 计数值
     */
    void    SetBmqJsz(QString const& strIp,int const& flag,int const& jsz);

    /*
     *获取 指定编码器 指定位置的 初始 计数值
     */

    int     GetBmqJsz(QString const& strIp,int const& flag);


    /*
     * 设置 当前 编码器 最新的计数值
     */
    void    SetBmqNewestJsz(QString const& strIp,int const& jsz);

    /*
     * 获取 指定编码器 最新的计数值
     */
    int     GetBmqNewestJsz(QString const& strIp);


    /*
     * 记录 制动 开始时 计数值
     * strID 电梯 ID
     */
    void     SetBmqBeginZdJsz(QString const& strIp,int const& jsz);

    /*
     * 获取 制动 开始时 计数值
     */
    int      GetBmqBeginZdJsz(QString const& strIp);

    /*
     * 获取 制动距离
     */
    double  GetBmqZdjl(QString const& strIp);

    /**************************************************/
    /*
     * can 模块数据队列处理  m_can_device_queue
     */

    void  PushCanDeviceQueue(device_buf_T const& buffT);

    device_buf_T PopCanDeviceQueue();

  /**************************************************/
    //动画 控制 数据包 读写
    /*
     *写入数据   m_device_to_dh_data
     */
    void WriteDeviceToDhData(QString const& strIp, QString const& strData);

    /*
     * 读出指定 客户端 的动画 数据包
     */
    QString  ReadDeviceToDhData(QString const& strLifterID);

  /**************************************************/
    /*
     *写入数据   m_device_to_client_map
	 *参数 为 strIp 设备IP 
	 *strData  数据
     */
    void WriteDeviceToClientMap(QString const& strIp, QString const& strData);

    /*
     * 读出数据 m_device_to_client_map
     * 参数 为 电梯 ID
     */
    QStringList ReadDeviceToClientMap(QString const& strLifterID);

    /**********************************************************/
    /*
     * 继电器指令队列
     */
    //设备ID 对应的 控制命令 list
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
     *用户登录信息
     *list:
     *0: 上下线 标识
     * 1: 用户
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

/**************************************************/

   /*读数据库任务队列
    *
    */
     QString PopDatabaseQueue();

     //写数据库任务队列
     void PushDatabaseQueue(int flag, QString  const &strSql);

     //写数据库队列

     void PushDatabaseQueueAll(QString const& strSql,int flag=config_device_config);

     int  ReturnDatabaseQueueSize();

     bool   IsEmptyDatabaseQueue();

/**************************************************/
    static UserBuffer* GetInstance(void);

        ~UserBuffer();
    /*
     * 程序结束,清理剩余的 sql 缓存
     */
    void      ClearBuff();
    /*
     * 初始化 m_strSql 缓存
     * 参数为
     * flag 表示数据类型 例如:device_jdq
     * strDBName 表示 数据库名字
     * strTableName 表名字
     */
    void   InitDBSqlBuff(int flag,QString const& strDBName,QString const& strTableName);

    /*
     * 初始化 m_strSql 缓存
     * 参数为
     * flag 表示数据类型 例如:device_jdq
     * strDBName 表示 数据库名字
     * strTableName 表名字
     */
    void   InitDBSqlBuffAll(int flag,QString const& strDBName,QString const& strTableName);


    UserBuffer();
private:


    static UserBuffer* m_singleton;

    /*
     * 回复 队列
     */
    QMap<QString,response_data>   m_response_queue;
    boost::mutex            m_response_queue_mutex;


    /**************************************************/

 /*
  *动画选择  记录电梯ID
  */
    QString   m_dhxz_id;

/**************************************************/
/*
 * 编码器 相关 计数值 缓存
 */
    //编码器初始  每层的计数值  记录个层数对应的计数值
    QMap<QString,QStringList>   m_bmq_begin_jsz;
    //编码器 最新的计数值
    QMap<QString,int>   m_bmq_newest_jsz;
    //编码器 制动开始时 计数值

    QMap<QString,int>   m_bmq_begin_zd_jsz;

    //制动 距离
    QMap<QString,double>    m_bmq_zd_jl;

    //制动 相关buff 统一 互斥锁
    boost::mutex        m_mutex_bmq_zd;
/**************************************************/
    /*
     *can模块 数据队列
     */
    QQueue<device_buf_T > m_can_device_queue;
    boost::mutex              m_mutex_can_device;

    /*
     * 数据库任务队列
     */
    //队列最多容纳数据条数
     int      m_database_queue_max;
     QQueue<QString> m_database_queue;
     boost::mutex      m_mutex_DB;
    boost::condition  m_cond_DB;


    /*
     * 每种需要的数据 分开存储
     */
    int         m_maxLen[device_max];
    QString m_strDBName;
    QString m_strTableName[device_max];
    QString m_strSql[device_max]; //保存最后执行的 sql 语句
    QString m_strDefSql[device_max]; //保存初始化的 sql语句
    int         m_iSqlCount[device_max];
    QMap<int,QString> m_strSqlAll;


     /*
      *
      * 管理所有设备 需要的
      * 缓冲区
      * 互斥锁
      * 条件变量
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
     * 设备关联客户端数据
     */
   // QMap<QString,QStringList>       m_device_to_client_map;
    boost::condition          m_device_to_client_map_cond;
    boost::mutex              m_device_to_client_map_mutex;
    /*
     *存储不同ip设备关联的数据
     */
    QMap<QString,QString>       m_device_to_data;
    /*
     * 存储不同设备ip 动画客户端所需要的数据
     */
    boost::condition          m_device_to_dh_data_map_cond;
    boost::mutex              m_device_to_dh_data_map_mutex;
    QMap<QString,QString>       m_device_to_dh_data;

};

#endif // USEBUFFER_H
