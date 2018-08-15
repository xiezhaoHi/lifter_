#ifndef DEVICETHREADCONTROL_H
#define DEVICETHREADCONTROL_H
#include<QMap>
#include<QDebug>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <winsock2.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include<QThread>
#include<QFile>
#include<QTextStream>
#include<QQueue>
#include"server/businesssession.h" //处理会话 返回数据的 通用类
#include"server/config/config.h" //配置信息 包含 设备 ip 与设备标识的信息表
#include"buffer/usebuffer.h"
using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

#define CGQ_BUFFSIZE 4096

/*
 * 继电器控制线程 获取开关量 或者 控制开关
 */
class JdqControlThread:public QThread
{
public:
    JdqControlThread(QString strIp,int iPort);


    void run();
    void stop();

private:
#ifdef TCP_ASIO //异步socket
    /*
     * 给继电器发送指令
     */
    bool   sendJdqData(tcp::socket& socket_);
#else
    bool   sendJdqData(SOCKET & clientsocket);
#endif
    /*
     * 继电器的ip 设备ID 和 端口号
     */
    QString m_strIp;
    QString m_strID;
    int m_iPort;


volatile  bool    m_stop;

};


/*
 * 传感器读写线程  获取水平度 距离 加速度
 */
class CgqControlThread:public QThread
{
public:
    CgqControlThread(QString strIp,int iPort);


    void run();
    void stop();

private:
    #ifdef TCP_ASIO
    /*
     * 给继电器发送指令 同步读取
     */
    bool   sendCgqData(tcp::socket& socket_);
    #else
    bool   sendCgqData(SOCKET & clientsocket);
    #endif
    /*
     * 异步读取
     */
    bool   ansy_sendCgqData(tcp::socket& socket_);
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    /*
     * 继电器的ip 和 端口号
     */
    QString m_strIp;
	QString m_strID;
    int m_iPort;
    char m_cgq_buff[CGQ_BUFFSIZE];
    boost::asio::ip::tcp::socket* m_psocket;
    boost::asio::io_service* m_iosev;
volatile  bool    m_stop;

};


/*
 * 编码器控制线程  获取脉冲计数 运动方向 转速
 */
class BmqControlThread:public QThread
{
public:
    BmqControlThread(QString strIp,int iPort);


    void run();
    void stop();

private:
#if TCP_ASIO
    /*
     * 给编码器发送指令
     */
    bool   sendBmqData(tcp::socket& socket_);
#else
    bool   sendBmqData(SOCKET &clientsocket );
#endif
    /*
     * 编码器的ip 和 端口号
     */
    QString m_strIp;
    int m_iPort;
	//设备ID属性
	QString m_strID;

volatile  bool    m_stop;

};


/*
 * 电压电流 传感器 设备
 */
class DydlControlThread:public QThread
{
public:
    DydlControlThread(QString strIp,int iPort);


    void run();

    void stop();
	bool   sendDydlData(SOCKET & clientsocket);

private:
    /*
     * 电压电流 传感器 设备的ip 和 端口号
     */
    QString m_strIp;
    int m_iPort;
	QString m_strID;//设备标识
	volatile  bool    m_stop;

};

class DeviceThreadControl
{
public:
    static DeviceThreadControl *&GetInstance();
    /*
     * 开始执行所有设备的读写操作
     */
    void    StartThreads();

    /*
     * 停止所有设备的读写
     */
    void  StopThreads();


    ~DeviceThreadControl();
private:

    /*
     * 保存多个继电器的处理线程
     */
    QMap<QString,boost::shared_ptr<JdqControlThread>> m_jdq_threads_map;


    /*JdqControlThread
     * 保存多个传感器的处理线程
     */
    QMap<QString,boost::shared_ptr<CgqControlThread>> m_cgq_threads_map;


    /*BmqControlThread
     * 保存多个编码器的处理线程
     */
    QMap<QString,boost::shared_ptr<BmqControlThread>> m_bmq_threads_map;

    /*DydlControlThread
     * 保存多个电压 电流 传感器的处理线程
     */
    QMap<QString,boost::shared_ptr<DydlControlThread>> m_dydl_threads_map;



    /*
     * CAN 应力模块处理
     *   应力CAN数据采集模块
     * flag 1 开启 采集
     * 0 停止采集
     */
    bool     startCANMod(bool   flag);


    DeviceThreadControl();
    static DeviceThreadControl* m_singleton;
};

#endif // DEVICETHREADCONTROL_H
