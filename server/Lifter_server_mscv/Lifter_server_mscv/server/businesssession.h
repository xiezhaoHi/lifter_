#ifndef BUSINESSSESSION_H
#define BUSINESSSESSION_H
#include<QString>
#include<boost/thread/mutex.hpp>
#include"buffer/usebuffer.h"
#include"buffer/canmodulebuffer.h"
#include"experimentthread.h"
#include<QMap>

/*
业务处理类
*/
//客户端发来的继电器命令 数据
typedef struct JDQ_DEAL_DATA 
{
		QString  m_strBelongs	; //所属电梯
		QString  m_strID		; //设备DO值
		QString  m_strRec		; //设置开关值 0 或者 1
		int		 m_openFlag; //点动 或者一直保持
}JDQ_DEAL_DATA;

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
	 * 返回 0 通用,解析数据完成
	 * 返回 -1 通用,解析错误哦
	 * 返回 1, 显示客户端 登陆
	 * lifterID 返回 客户端关联的电梯ID  
	 * clientID 返回 客户端ID
     */
    int DealClientPackg(QString strData, QString const& ,QString & ,QString &);

    /*
     * 获取继电器指令
     */
    bool GetJdqControlOrder(JDQ_DEAL_DATA &, QString &, char *buff, int &len, int packCount = 1);

/**********************************************************/
    /*
     *下线通知 客户端
     */
    QString GetExitDevicePackg(QString const& );


    /*
     * 开始执行实验线程
     */
    void StartExperimentThread(QString const& ,experimentData const& strData);

  /**********************************************************************/
    /*
    *
    * 处理传感器数据信息
    *
    *
    */
    QString  DealCgqData(const QString &, char* , int );

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

    QString DealBmqData(const QString &strID, char* data, int len);
    //编码器
    QString DealBmqQuit(QString const& strIp);

    /*服务器处理数据的统一接口
     *
     * strIp 设备IP
     * data 设备采集的数据缓存指针
     * len 设备采集数据的长度
     *
	 *20180620 修改:增加一个flag标志,标识数据的类型
     *
*/

    QString InterfaceFun(QString strIp,char* data, int len,  int flag=-1);




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
    QString DealJdqData(QString const&,char* data,int len);

    //继电器
  QString DealJdqQuit(QString const& strIp);

	/************************************************************************/
	/*
	* 电源 电压 电流 功率 数据解析
	* 20180620 修改: 新增 flag 标识 数据的类型
	*/
  QString DealDydlData(QString const&, char* , int , int );

  QString DealDydlQuit(QString const& strIp);
    /**********************************************************************/
    /*
     * 通用 获取各个设备 Userbuffer对象指针
     * 参数 设备id 和 设备类型 标识
     */
    UserBuffer* GetDeviceUserBuffer(QString const& strId, int flag);


    /*
     * can module 写缓存
     */
    void    WriteCanModuleBuffer(QString const& ,VCI_CAN_OBJ* const& ,int const& );

    /*
     * 读 can module 缓存
     */
    int ReadCanModuleBuffer(QString const& ,VCI_CAN_OBJ* ,int const& );
     /**********************************************************************/
    /*
     * 返回错误信息
     */
    QString GetLastError();

private:
	
	//大小端标识 true 为大端(低地址存 高字节数据)
	bool m_bigFlag;

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
     * can 设备模块id 映射 buffer
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
