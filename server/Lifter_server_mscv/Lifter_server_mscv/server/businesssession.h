#ifndef BUSINESSSESSION_H
#define BUSINESSSESSION_H
#include<QString>
#include<boost/thread/mutex.hpp>
#include"buffer/usebuffer.h"
#include"buffer/canmodulebuffer.h"
#include"experimentthread.h"
#include<QMap>

/*
ҵ������
*/
//�ͻ��˷����ļ̵������� ����
typedef struct JDQ_DEAL_DATA 
{
		QString  m_strBelongs	; //��������
		QString  m_strID		; //�豸DOֵ
		QString  m_strRec		; //���ÿ���ֵ 0 ���� 1
		int		 m_openFlag; //�㶯 ����һֱ����
}JDQ_DEAL_DATA;

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
	 * ���� 0 ͨ��,�����������
	 * ���� -1 ͨ��,��������Ŷ
	 * ���� 1, ��ʾ�ͻ��� ��½
	 * lifterID ���� �ͻ��˹����ĵ���ID  
	 * clientID ���� �ͻ���ID
     */
    int DealClientPackg(QString strData, QString const& ,QString & ,QString &);

    /*
     * ��ȡ�̵���ָ��
     */
    bool GetJdqControlOrder(JDQ_DEAL_DATA &, QString &, char *buff, int &len, int packCount = 1);

/**********************************************************/
    /*
     *����֪ͨ �ͻ���
     */
    QString GetExitDevicePackg(QString const& );


    /*
     * ��ʼִ��ʵ���߳�
     */
    void StartExperimentThread(QString const& ,experimentData const& strData);

  /**********************************************************************/
    /*
    *
    * ��������������Ϣ
    *
    *
    */
    QString  DealCgqData(const QString &, char* , int );

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

    QString DealBmqData(const QString &strID, char* data, int len);
    //������
    QString DealBmqQuit(QString const& strIp);

    /*�������������ݵ�ͳһ�ӿ�
     *
     * strIp �豸IP
     * data �豸�ɼ������ݻ���ָ��
     * len �豸�ɼ����ݵĳ���
     *
	 *20180620 �޸�:����һ��flag��־,��ʶ���ݵ�����
     *
*/

    QString InterfaceFun(QString strIp,char* data, int len,  int flag=-1);




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
    QString DealJdqData(QString const&,char* data,int len);

    //�̵���
  QString DealJdqQuit(QString const& strIp);

	/************************************************************************/
	/*
	* ��Դ ��ѹ ���� ���� ���ݽ���
	* 20180620 �޸�: ���� flag ��ʶ ���ݵ�����
	*/
  QString DealDydlData(QString const&, char* , int , int );

  QString DealDydlQuit(QString const& strIp);
    /**********************************************************************/
    /*
     * ͨ�� ��ȡ�����豸 Userbuffer����ָ��
     * ���� �豸id �� �豸���� ��ʶ
     */
    UserBuffer* GetDeviceUserBuffer(QString const& strId, int flag);


    /*
     * can module д����
     */
    void    WriteCanModuleBuffer(QString const& ,VCI_CAN_OBJ* const& ,int const& );

    /*
     * �� can module ����
     */
    int ReadCanModuleBuffer(QString const& ,VCI_CAN_OBJ* ,int const& );
     /**********************************************************************/
    /*
     * ���ش�����Ϣ
     */
    QString GetLastError();

private:
	
	//��С�˱�ʶ true Ϊ���(�͵�ַ�� ���ֽ�����)
	bool m_bigFlag;

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
     * can �豸ģ��id ӳ�� buffer
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
