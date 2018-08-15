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
#include"server/businesssession.h" //����Ự �������ݵ� ͨ����
#include"server/config/config.h" //������Ϣ ���� �豸 ip ���豸��ʶ����Ϣ��
#include"buffer/usebuffer.h"
using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

#define CGQ_BUFFSIZE 4096

/*
 * �̵��������߳� ��ȡ������ ���� ���ƿ���
 */
class JdqControlThread:public QThread
{
public:
    JdqControlThread(QString strIp,int iPort);


    void run();
    void stop();

private:
#ifdef TCP_ASIO //�첽socket
    /*
     * ���̵�������ָ��
     */
    bool   sendJdqData(tcp::socket& socket_);
#else
    bool   sendJdqData(SOCKET & clientsocket);
#endif
    /*
     * �̵�����ip �豸ID �� �˿ں�
     */
    QString m_strIp;
    QString m_strID;
    int m_iPort;


volatile  bool    m_stop;

};


/*
 * ��������д�߳�  ��ȡˮƽ�� ���� ���ٶ�
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
     * ���̵�������ָ�� ͬ����ȡ
     */
    bool   sendCgqData(tcp::socket& socket_);
    #else
    bool   sendCgqData(SOCKET & clientsocket);
    #endif
    /*
     * �첽��ȡ
     */
    bool   ansy_sendCgqData(tcp::socket& socket_);
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    /*
     * �̵�����ip �� �˿ں�
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
 * �����������߳�  ��ȡ������� �˶����� ת��
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
     * ������������ָ��
     */
    bool   sendBmqData(tcp::socket& socket_);
#else
    bool   sendBmqData(SOCKET &clientsocket );
#endif
    /*
     * ��������ip �� �˿ں�
     */
    QString m_strIp;
    int m_iPort;
	//�豸ID����
	QString m_strID;

volatile  bool    m_stop;

};


/*
 * ��ѹ���� ������ �豸
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
     * ��ѹ���� ������ �豸��ip �� �˿ں�
     */
    QString m_strIp;
    int m_iPort;
	QString m_strID;//�豸��ʶ
	volatile  bool    m_stop;

};

class DeviceThreadControl
{
public:
    static DeviceThreadControl *&GetInstance();
    /*
     * ��ʼִ�������豸�Ķ�д����
     */
    void    StartThreads();

    /*
     * ֹͣ�����豸�Ķ�д
     */
    void  StopThreads();


    ~DeviceThreadControl();
private:

    /*
     * �������̵����Ĵ����߳�
     */
    QMap<QString,boost::shared_ptr<JdqControlThread>> m_jdq_threads_map;


    /*JdqControlThread
     * �������������Ĵ����߳�
     */
    QMap<QString,boost::shared_ptr<CgqControlThread>> m_cgq_threads_map;


    /*BmqControlThread
     * �������������Ĵ����߳�
     */
    QMap<QString,boost::shared_ptr<BmqControlThread>> m_bmq_threads_map;

    /*DydlControlThread
     * ��������ѹ ���� �������Ĵ����߳�
     */
    QMap<QString,boost::shared_ptr<DydlControlThread>> m_dydl_threads_map;



    /*
     * CAN Ӧ��ģ�鴦��
     *   Ӧ��CAN���ݲɼ�ģ��
     * flag 1 ���� �ɼ�
     * 0 ֹͣ�ɼ�
     */
    bool     startCANMod(bool   flag);


    DeviceThreadControl();
    static DeviceThreadControl* m_singleton;
};

#endif // DEVICETHREADCONTROL_H
