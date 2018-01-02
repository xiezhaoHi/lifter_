#ifndef BUSINESSSESSION_H
#define BUSINESSSESSION_H
#include<QString>
#include<boost/thread/mutex.hpp>
#include"buffer/usebuffer.h"
#include"buffer/canmodulebuffer.h"
#include"experimentthread.h"
#include<QMap>
#define MAX_LEN 64 //传感器中间缓冲区大小
#define PACKAGE_SIZE 11 // 数据包长度
#define  BMQBUFF_SIZE  22 //编码器buff size
#define  BMQPACKAGE_ONESIZE   13 //编码器 03 指令反悔数据包大小为 13   方向 角度 转速
#define  BMQPACKAGE_TWOSIZE  9 //编码器 04指令返回数据包大小为 9   计数值
#define  BMQPACKAGE_SENDSIZE 8 //编码器 发送的指令长度
/*
业务处理类
*/

class BusinessSession
{
public:
    static BusinessSession* GetInstance(void);
    ~BusinessSession();
    /*
     *
     * 处理客户端 发送来的控制数据包 返回相应的设备ip 和 命令
     *data len 为 客户端 发来的 数据 和数据长度
     * strIp 和 buff len 是 返回的数据设备ip 和命令 命令长度
     */
    bool DealClientPackg(QString strData, const QString &strClientIp);

    /*
     * 获取继电器指令
     */
    bool GetJdqControlOrder(const QString &strClientIp, const QString &strID, const QString &strRec, QString &strDeviceIp, char *buff, int &len, int packCount = 1);

/**********************************************************/
    /*
     *下线通知 客户端
     */
    QString GetExitDevicePackg(QString const& strIp);


    /*
     * 开始执行实验线程
     */
    void StartExperimentThread(QString const& strClientIp,experimentData const& strData);

  /**********************************************************************/
    /*
    *
    * 处理传感器数据信息
    *
    *
    */
    QString  DealCgqData(const QString &strIp, char* data, int len);

    //传感器
    QString DealCgqQuit(QString const& strIp);
    /**********************************************************************/

    /*
    *
    *
    * 处理旋转编码器信息
    *
    *
    *
    */
    //获取crc校验码
    unsigned short usMBCRC16( unsigned char * pucFrame, unsigned short usLen );

    QString DealBmqData(const QString &strIp, const QString &strID, char* data, int len);
    //编码器
    QString DealBmqQuit(QString const& strIp);

    /*服务器处理数据的统一接口
     *
     * strIp 设备IP
     * data 设备采集的数据缓存指针
     * len 设备采集数据的长度
     *
     *
*/
    QString InterfaceFun(QString strIp,char* data, int len);

    /*m_bmqFirst*/
    bool  getBmqFirst(QString const& strIp);

    /*m_bmqFirst*/
    void setBmqFirst(QString const& strIp, bool flag);

    /*设置每转大小*/
    void setBmqSize(QString const& strIp,int size);

    /*获取每转大小*/
    int getBmqSize(QString const& strIp);

    /*设置每转脉冲  刻度*/
    void setBmqKd(QString const& strIp,int size);

    /*获取每转脉冲  刻度*/
    int getBmqKd(QString const& strIp);

    /*设置指令长度*/
    void setBmqDataLen(QString const& strIp,int size);

    /*指令长度*/
    int getBmqDataLen(QString const& strIp);

    /*设置m_bmqMapBuff*/
    void setBmqMapBuff(QString const& strIp,char* buff,int size);

    /*m_bmqMapBuff*/
    char* getBmqMapBuff(QString const& strIp);
    /**********************************************************************/

    /*
    *
    *
    * 处理继电器数据
    *
    *
    *
    */
    QString DealJdqData(QString strIp,char* data,int len);

    //继电器
  QString DealJdqQuit(QString const& strIp);

    /**********************************************************************/
    /*
     * 通用 获取各个设备 Userbuffer对象指针
     * 参数 设备id 和 设备类型 标识
     */
    UserBuffer* GetDeviceUserBuffer(QString const& strId, int flag);


    /*
     * can module 写缓存
     */
    void    WriteCanModuleBuffer(QString const& strIp,VCI_CAN_OBJ* const& buff,int const& size);

    /*
     * 读 can module 缓存
     */
    int ReadCanModuleBuffer(QString const& strIp,VCI_CAN_OBJ* buff,int const& size);
     /**********************************************************************/
    /*
     * 返回错误信息
     */
    QString GetLastError();

private:
       /*
        * 实现线程
        */
      QMap<QString,ExperimentThread*>   m_experimentThread_map;


      /**********************************************************************/
    /*
     * 各个设备 id 映射 缓冲区类
     */
    //传感器类型设备 map
    QMap<QString,UserBuffer*>   m_userbuffer_map;
//    //编码器类型设备 map
//    QMap<QString,UserBuffer*>   m_cgq_userbuffer_map;
//    //继电器类型设备 map
//    QMap<QString,UserBuffer*>   m_cgq_userbuffer_map;


    /*
     * can 设备模块ip 映射 buffer
     */
    QMap<QString,CanModuleBuffer*>   m_canModule_buffer_map;
     /**********************************************************************/
    QString  m_lastErr; //记录最后一次错误
    BusinessSession();
    static BusinessSession* m_singleton;
    char m_cgq_buff[MAX_LEN];
    int   m_index;
    boost::mutex m_cgq_mutex;

    /*
     * 编码器 2个命令 发送 顺序
     * true 表示 第一条指令  获取转速指令
     * false 表示发送第二条指令 获取计数值指令
     * 这些map 都是设备ID 或者设备ip 映射
     */
    QMap<QString,bool>    m_bmqFirst; //ip map 是否是第一条指令
    QMap<QString,char*>   m_bmqMapBuff;//[BMQBUFF_SIZE];
    QMap<QString,int>       m_bmqDataLen; //编码器2包数据长度
    QMap<QString,int>       m_mz_size; //每转 大小
    QMap<QString,int>       m_mz_kd; // 每转刻度  每转的脉冲数
};

#endif // BUSINESSSESSION_H
