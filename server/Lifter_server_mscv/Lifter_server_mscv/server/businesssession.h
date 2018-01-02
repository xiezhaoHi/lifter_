#ifndef BUSINESSSESSION_H
#define BUSINESSSESSION_H
#include<QString>
#include<boost/thread/mutex.hpp>
#include"buffer/usebuffer.h"
#include"buffer/canmodulebuffer.h"
#include"experimentthread.h"
#include<QMap>
#define MAX_LEN 64 //�������м仺������С
#define PACKAGE_SIZE 11 // ���ݰ�����
#define  BMQBUFF_SIZE  22 //������buff size
#define  BMQPACKAGE_ONESIZE   13 //������ 03 ָ������ݰ���СΪ 13   ���� �Ƕ� ת��
#define  BMQPACKAGE_TWOSIZE  9 //������ 04ָ������ݰ���СΪ 9   ����ֵ
#define  BMQPACKAGE_SENDSIZE 8 //������ ���͵�ָ���
/*
ҵ������
*/

class BusinessSession
{
public:
    static BusinessSession* GetInstance(void);
    ~BusinessSession();
    /*
     *
     * ����ͻ��� �������Ŀ������ݰ� ������Ӧ���豸ip �� ����
     *data len Ϊ �ͻ��� ������ ���� �����ݳ���
     * strIp �� buff len �� ���ص������豸ip ������ �����
     */
    bool DealClientPackg(QString strData, const QString &strClientIp);

    /*
     * ��ȡ�̵���ָ��
     */
    bool GetJdqControlOrder(const QString &strClientIp, const QString &strID, const QString &strRec, QString &strDeviceIp, char *buff, int &len, int packCount = 1);

/**********************************************************/
    /*
     *����֪ͨ �ͻ���
     */
    QString GetExitDevicePackg(QString const& strIp);


    /*
     * ��ʼִ��ʵ���߳�
     */
    void StartExperimentThread(QString const& strClientIp,experimentData const& strData);

  /**********************************************************************/
    /*
    *
    * ��������������Ϣ
    *
    *
    */
    QString  DealCgqData(const QString &strIp, char* data, int len);

    //������
    QString DealCgqQuit(QString const& strIp);
    /**********************************************************************/

    /*
    *
    *
    * ������ת��������Ϣ
    *
    *
    *
    */
    //��ȡcrcУ����
    unsigned short usMBCRC16( unsigned char * pucFrame, unsigned short usLen );

    QString DealBmqData(const QString &strIp, const QString &strID, char* data, int len);
    //������
    QString DealBmqQuit(QString const& strIp);

    /*�������������ݵ�ͳһ�ӿ�
     *
     * strIp �豸IP
     * data �豸�ɼ������ݻ���ָ��
     * len �豸�ɼ����ݵĳ���
     *
     *
*/
    QString InterfaceFun(QString strIp,char* data, int len);

    /*m_bmqFirst*/
    bool  getBmqFirst(QString const& strIp);

    /*m_bmqFirst*/
    void setBmqFirst(QString const& strIp, bool flag);

    /*����ÿת��С*/
    void setBmqSize(QString const& strIp,int size);

    /*��ȡÿת��С*/
    int getBmqSize(QString const& strIp);

    /*����ÿת����  �̶�*/
    void setBmqKd(QString const& strIp,int size);

    /*��ȡÿת����  �̶�*/
    int getBmqKd(QString const& strIp);

    /*����ָ���*/
    void setBmqDataLen(QString const& strIp,int size);

    /*ָ���*/
    int getBmqDataLen(QString const& strIp);

    /*����m_bmqMapBuff*/
    void setBmqMapBuff(QString const& strIp,char* buff,int size);

    /*m_bmqMapBuff*/
    char* getBmqMapBuff(QString const& strIp);
    /**********************************************************************/

    /*
    *
    *
    * ����̵�������
    *
    *
    *
    */
    QString DealJdqData(QString strIp,char* data,int len);

    //�̵���
  QString DealJdqQuit(QString const& strIp);

    /**********************************************************************/
    /*
     * ͨ�� ��ȡ�����豸 Userbuffer����ָ��
     * ���� �豸id �� �豸���� ��ʶ
     */
    UserBuffer* GetDeviceUserBuffer(QString const& strId, int flag);


    /*
     * can module д����
     */
    void    WriteCanModuleBuffer(QString const& strIp,VCI_CAN_OBJ* const& buff,int const& size);

    /*
     * �� can module ����
     */
    int ReadCanModuleBuffer(QString const& strIp,VCI_CAN_OBJ* buff,int const& size);
     /**********************************************************************/
    /*
     * ���ش�����Ϣ
     */
    QString GetLastError();

private:
       /*
        * ʵ���߳�
        */
      QMap<QString,ExperimentThread*>   m_experimentThread_map;


      /**********************************************************************/
    /*
     * �����豸 id ӳ�� ��������
     */
    //�����������豸 map
    QMap<QString,UserBuffer*>   m_userbuffer_map;
//    //�����������豸 map
//    QMap<QString,UserBuffer*>   m_cgq_userbuffer_map;
//    //�̵��������豸 map
//    QMap<QString,UserBuffer*>   m_cgq_userbuffer_map;


    /*
     * can �豸ģ��ip ӳ�� buffer
     */
    QMap<QString,CanModuleBuffer*>   m_canModule_buffer_map;
     /**********************************************************************/
    QString  m_lastErr; //��¼���һ�δ���
    BusinessSession();
    static BusinessSession* m_singleton;
    char m_cgq_buff[MAX_LEN];
    int   m_index;
    boost::mutex m_cgq_mutex;

    /*
     * ������ 2������ ���� ˳��
     * true ��ʾ ��һ��ָ��  ��ȡת��ָ��
     * false ��ʾ���͵ڶ���ָ�� ��ȡ����ֵָ��
     * ��Щmap �����豸ID �����豸ip ӳ��
     */
    QMap<QString,bool>    m_bmqFirst; //ip map �Ƿ��ǵ�һ��ָ��
    QMap<QString,char*>   m_bmqMapBuff;//[BMQBUFF_SIZE];
    QMap<QString,int>       m_bmqDataLen; //������2�����ݳ���
    QMap<QString,int>       m_mz_size; //ÿת ��С
    QMap<QString,int>       m_mz_kd; // ÿת�̶�  ÿת��������
};

#endif // BUSINESSSESSION_H
