#ifndef CCANMODULESERVER_H
#define CCANMODULESERVER_H

//extern "C"


#include<QThread>
#include<QLibrary>
#include<QString>
#include<QMap>
#include<QMutex>

#include"buffer/usebuffer.h"
#include"ControlCAN/ControlCAN.h"
#define CANBUFF_SIZE 8
#define CANMODULE_MAX 10
typedef struct CANDATA_STRUCT
{
    QString strIp;//"192.168.1.1"类似 字符串
	QString strID;//设备ID
    int    port;
    int cannum;
    DWORD devind;
      int connect;
        int devtype;
}CANDATA,*PCANDATA;
typedef enum UP_CMD
{
    UP_DATA = 0x10,
}UP_CMD_T;

typedef struct strainDeviceData
{
    time_t time_sec;
    struct tm *time;
    unsigned short time_ms;
    unsigned short time_hz;
    int strain;
    strainDeviceData()
    {
        time_sec  =time_ms= time_hz  =strain= 0;
        time = NULL;
    }
}strainDeviceData_T;

typedef enum
{
    STRAINGAUGES_CH1 = 0,
    STRAINGAUGES_CH2,
    STRAINGAUGES_CH3,
    STRAINGAUGES_CH4,
    STRAINGAUGES_CH_NUM,
    STRAINGAUGES_ALL,
    STRAINGAUGES_NULL
}STRAINGAUGES_CH_T;


typedef struct strainDeviceDataS
{
    struct tm time;
    unsigned short time_ms;
    int strain;
}strainDeviceDataS_T;

#define START_ID 0x168
#define MAX_CHANNEL 32
#define MAX_DISP_WAVE 1000



typedef unsigned int UINT32;
typedef unsigned short UINT16;


typedef struct strainDeviceFifoS
{
    UINT16 cnt;
    UINT32 id;
    strainDeviceDataS_T fifoBuf[FIFO_LEN];
}strainDeviceFifo_T;


/*
 * CAN dll 初始化
 */
class CANModuleLib
{
public:
    static CANModuleLib* GetInstance();

    void  UnLoadLib();
    bool  InitLib();
     void* GetLibFunc(int flag);
     QString GetLastError();

private:
    CANModuleLib();

    static CANModuleLib* m_singleton;

    VCI_CloseDevice	 m_VCI_CloseDevice		;
    VCI_GetReceiveNum	 m_VCI_GetReceiveNum;
    VCI_GetReference	 m_VCI_GetReference	;
    VCI_InitCAN		 m_VCI_InitCAN		;
    VCI_OpenDevice		 m_VCI_OpenDevice	;
    VCI_ReadBoardInfo	 m_VCI_ReadBoardInfo;
    VCI_ReadCANStatus	 m_VCI_ReadCANStatus;
    VCI_ReadErrInfo	 m_VCI_ReadErrInfo	;
    VCI_Receive		 m_VCI_Receive		;
    VCI_ResetCAN		 m_VCI_ResetCAN		;
    VCI_SetReference	 m_VCI_SetReference	;
    VCI_StartCAN		 m_VCI_StartCAN		;
    VCI_Transmit        m_VCI_Transmit		;
    QMap<int,void*>     m_func_map; //保存dll 中导出的函数指针
    QLibrary* m_lib;
    QString m_lastError;
};

class DataConnectionThread;

//解析 分发给客户端
class DataAnalyzeThread:public QThread
{
public:
    DataAnalyzeThread(QString const& strId,device_buf_T* gSaveFifo_buf):m_gSaveFifo_buf(gSaveFifo_buf)
    {
        m_strID = strId;
        m_stopFlag = true;
        m_connectThread = NULL;
    }

    void stop()
    {
        m_stopFlag = false;
    }
    void run();
    volatile  bool    m_stopFlag;
    QString     m_strIp;
	QString     m_strID;
    device_buf_T* m_gSaveFifo_buf;
    DataConnectionThread*   m_connectThread;
    bool TongDate(UINT32 id);
};

//解析数据 并 分发给数据存储 队列
class DataAnalyzeSaveDB:public QThread
{
public:
    DataAnalyzeSaveDB(QString const& strID,QString const& strDeviceID)
    {
        m_strID = strID;
        m_stopFlag = true;
        m_strDeviceID = strDeviceID;
    }

    void stop()
    {
        m_stopFlag = false;
    }
    void run();
    volatile  bool    m_stopFlag;
    QString     m_strID;
    QString     m_strDeviceID;

};

//不断读取CAN模块数据
class DataConnectionThread : public QThread
{
public:

   // strainDeviceFifo_T gFifo_buf[STRAINGAUGES_CH_NUM];
    device_buf_T m_gSaveFifo_buf[STRAINGAUGES_CH_NUM];

    void stop()
    {
        m_stopFlag = false;
        if(m_VCI_CloseDevice)
            m_VCI_CloseDevice(m_devtype,m_devind);
    }
    ~DataConnectionThread()
    {

    }

    DataConnectionThread(CANDATA const& canData);

    bool      InitCANModule(); //初始化CAN MODULE 连接

    bool      ControlCANModule(int flag); //控制CAN模块 0 暂停 1启动

    QString GetLastError(); //获取最后一个错误信息


 //   void AnalsyData(const device_buf_T &buff); //1.解析数据 2.组包分发出去 3.存数据库
public:


    bool    m_controlStop; //true 等待 false 唤醒
    /****************************************/
    bool    m_initFlag; //InitCANModule 初始化成功与否
    void run();
    QString m_strIp;//"192.168.1.1"类似 字符串
	QString m_strID; //设备ID
    int    m_port;
    int m_cannum;

    DWORD m_devind;
      int m_connect;
        int m_devtype;
   volatile  bool    m_stopFlag;

    QString     m_lastError;
    unsigned char gCAN_BUF[CANBUFF_SIZE];

    VCI_CloseDevice	 m_VCI_CloseDevice		;
    VCI_GetReceiveNum	 m_VCI_GetReceiveNum;
    VCI_GetReference	 m_VCI_GetReference	;
    VCI_InitCAN		 m_VCI_InitCAN		;
    VCI_OpenDevice		 m_VCI_OpenDevice	;
    VCI_ReadBoardInfo	 m_VCI_ReadBoardInfo;
    VCI_ReadCANStatus	 m_VCI_ReadCANStatus;
    VCI_ReadErrInfo	 m_VCI_ReadErrInfo	;
    VCI_Receive		 m_VCI_Receive		;
    VCI_ResetCAN		 m_VCI_ResetCAN		;
    VCI_SetReference	 m_VCI_SetReference	;
    VCI_StartCAN		 m_VCI_StartCAN		;
    VCI_Transmit        m_VCI_Transmit		;

    /*标识返回数据包中ID 所标识的通道 ID 例如:
     *'00000068' 对应 0 表示 它的数据为  0*通道数 + (1,2,3,4) 号通道的 数据 1 2 3 4
     * '00000069' 对应  1 表示 它的数据为 0*通道数 +（1,2,3,4）号通道的数据 5 6 7 8
     */
    QMap<QString,int> m_ID_map;

};

//QString const& strIp,int const& port,int cannum=0
//            ,int devind=0,int connect=1,int devtype=VCI_CANETTCP
class CCanModuleServer
{
public:

    /*
     * 启动 或 停止 指定
     * 设备ip 的CAN设备
     * 0 停止 1启动
     *
     */
     void    ControlCANModule(QString const& strIp,int flag);

    /************************************/
    /*开始和停止CAN采集模块
     */
    bool    StartCANModule(int index, CANDATA const& canData);
    void    StopCANModuel();

    QString GetLastError();
    static CCanModuleServer*   GetInstance();
    ~CCanModuleServer()
    {
        for(int index = 0; index < CANMODULE_MAX;++index)
        {
            if(m_dataConThread[index])
            {
                m_dataConThread[index]->stop();
                m_dataConThread[index]->quit();
                m_dataConThread[index]->wait();
                delete m_dataConThread[index];
            }
            if(m_analyzeThread[index])
            {
                m_analyzeThread[index]->stop();
                m_analyzeThread[index]->quit();
                m_analyzeThread[index]->wait();
                delete m_analyzeThread[index];
            }

        }

    }

private:
    QMap<QString,DataConnectionThread*> m_controlThread_map;
    DataConnectionThread* m_dataConThread[CANMODULE_MAX];
    DataAnalyzeThread*    m_analyzeThread[CANMODULE_MAX];
    DataAnalyzeSaveDB*     m_analyzeSaveDB[CANMODULE_MAX];
    static  CCanModuleServer*   m_singleton;
    QString m_lastError;

    CCanModuleServer();
};

#endif // CCANMODULESERVER_H
