#ifndef ENUM_H
#define ENUM_H
#include<QString>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
//����һЩ��������

//����
#define _TEST_

//�û�����(���û�client�˺Ͷ���client��������)�̳߳�����
#define	THREAD_POOL_NUM  1

//�ͻ��˵ȴ������ʱʱ��
#define SOCKET_WAIT_TIME  3000

//���ݿ�����������
#define DATABASE_QUEUE_MAX 5000

//��ʶ ��� ���� λ��
#define  USER_ID_COUNT 4

#define  DISPENSE_TIMES 100 //�ַ�-�ɼ�ʱ����Ϊ100ms
typedef struct{
    int flag; //��ʶ������ʲô
    QString strData;//�����������
}struct_data;

//������ݱ�ʶo
enum enum_detection
{
    detection_dydl, //��ѹ ����
     detection_aqcd,//��ȫ����
     detection_hxdlwz,//�������λ��
     detection_slyxsd, //ʸ�������ٶ�
    detection_pcjd,//ƽ�㾫�ȣ��������ڼ�����������
    detection_jgyl,//��Ҫ������λ�ṹӦ��
                        //��˿��ʹ��״̬
    detection_gssnwbds, //��˿�����ⲿ��˿

                    //����/���ᾲ̬ˮƽ�ȼ�⹦�ܣ�
    detection_cgq, //��������Ϣ
    detection_spd,//����/����ˮƽ��
                    //����/�������мӡ����ٶȼ�⹦��
    detection_jjsd,//����/��������мӼ��ٶ�
    detection_zdjl,//�ƶ�������Զ���⹦�� �ƶ�����
    detection_max
};

/*�豸 ��ʶ*/
#define device_begin_ID 1000
enum device_name_config
{
    device_cgq = 1, //�������豸
    device_bmq , //������
    device_jdq , //�̵���
    device_can, //CANģ���豸
    device_dydl, //��ѹ����
    device_gss, //��˿��
    device_max =6
};

/*
 * ���ݿ� ���� ��ʶ
 */
enum connect_db_flag
{
    connect_db_mydb, //�����Լ���db
    connect_db_gssdb //��˿�� db
};

/*�豸��Ӧ������֡����*/
enum device_data_len
{
    device_cgq_data_len = 11, //������������Ϣ����
    device_bmq_data_len = 8, //����������֡����
    device_jdq_data_len = 9, //�̵�������֡����
	device_dydl_data_len =8 //��Դ ����֡����
};

#define MAX_LEN 64 //�������м仺������С
#define PACKAGE_SIZE 11 // ���ݰ�����
#define  BMQBUFF_SIZE  9 //�������������� buff size 
#define  BMQPACKAGE_ONESIZE   13 //������ 03 ָ������ݰ���СΪ 13   ���� �Ƕ� ת��
#define  BMQPACKAGE_TWOSIZE  9 //������ 04ָ������ݰ���СΪ 9   ����ֵ
#define  BMQPACKAGE_SENDSIZE 8 //������ ���͵�ָ���

//��Դ ��ѹ ���� 
//������
#define DY_FUNCODE_03	0x03 //�����ּĴ���,��ָ���ȡ���豸���ּĴ�����4X���Ķ�����ֵ
#define DY_FUNCODE_06	0x06 //д�����Ĵ�������ָ����豸��ĳ�����ּĴ�����4X������Ϊָ��ֵ
#define DY_FUNCODE_80	0x80 //�쳣������	������+80H

//�����
enum DY_DATA_3
{
	DY_DATA3_A,
	DY_DATA3_B,
	DY_DATA3_C,
	DY_DATA3_max,
};
enum DY_RES_INDEX
{
	DY_RES_0, //�ӻ���ַ
	DY_RES_1, //������
};
//��Դ�������ݵı�ʶ
enum DY_DATA_FLAG
{
	DY_DATA_FLAG_RET = 1000, //д�Ĵ����ķ�������
	DY_DATA_FLAG_DY = 1003, //��ѹ����
	DY_DATA_FLAG_DL = 1006, //��������
	DY_DATA_FLAG_GL = 1012, //��������
};

/*�û���ʶ*/
enum client_name_config
{
    client_qzsjysjj, //ǿ��ʽ����������
    client_max
};

/*���ݿ�������Ϣ*/
enum database_config
{
    database_hostName, //�������� ip
    database_dbName, //���ݿ�����
    database_userName, //�û���
    database_password, //����
    database_dbType, //���ݿ�����
    database_test, //�����������
    database_testSql, //����sql
    database_maxWaitTime, //��ȴ�����ʱ��
    database_waitTime, //�ȴ�ʱ��
    database_maxConnectCount, //���������
    database_max
};

/*��־���󼶱�
DEBUG��INFO��WARN��ERROR��FATAL
*/
enum logerr_level
{
    logerr_debug = 0 ,
     logerr_info,
     logerr_warn,
     logerr_error,
     logerr_fatal,
     logerr_max
};


/*
 * �����ļ���
 * ��ʾ��ͬ��map
 */

enum map_name
{
    map_device = 0, //�豸
    map_client, //�ͻ���
    map_database, // �������ݿ�����
    map_database_gss, //��˿�� ���ݿ�����
    map_max
};

/*
 *
 * ���ݿ��� �豸��������Ϣ
 *
 */


enum config_from_database
{
    config_observe_control_device,
    config_be_observe_control_device,
    config_device_config,
    config_lifter_config,
    config_lifter_id_describe,
    config_client_config,
    config_max
};
/*
 * ��ȡ �豸 �� client ��Ϣmap
 */
enum device_or_client
{
    device_or_client_device = 0,
    device_or_client_client,
    device_or_client_max
};


/*
 * config_observe_control_device ��Ӧ����
 */
enum observe_control_device
{
    observe_control_device_id,
    observe_control_device_name,
    observe_control_device_describe,
    observe_control_device_ip_address,
    observe_control_device_port,
    observe_control_device_belongs,
    observe_control_device_max

};
/*
 * config_client_config
 */
enum client_config
{
    client_config_id,
    client_config_address,
    client_config_belongs,
    client_config_max
};

/*
 * 2017/6/7
 * ����:
 * session.h session.cpp ֻ���� ���� ����ʾ���ƿͻ���
 * �豸 ���ݲɼ��� ���� ����DeviceControl.h DeviceControl.cpp
 */
#define     SESSION_TO_DEVICECONTROL


/*
 *
 * CAN module lib ��ʼ��
 */

enum CANMODULE
{
    CANMODULE_VCI_CloseDevice	  ,
    CANMODULE_VCI_GetReceiveNum ,
    CANMODULE_VCI_GetReference  ,
    CANMODULE_VCI_InitCAN		  ,
    CANMODULE_VCI_OpenDevice	  ,
    CANMODULE_VCI_ReadBoardInfo ,
    CANMODULE_VCI_ReadCANStatus ,
    CANMODULE_VCI_ReadErrInfo	  ,
    CANMODULE_VCI_Receive		  ,
    CANMODULE_VCI_ResetCAN	  ,
    CANMODULE_VCI_SetReference  ,
    CANMODULE_VCI_StartCAN	  ,
    CANMODULE_VCI_Transmit      ,
    CANMODULE_VCI_MAX
};

/*
 * pc�ͻ������ݰ� ����
 */
enum PCClientPkg
{
    PCClientPkg_Jdq_DO = 4001, //���ݰ�Ϊ�̵��� ����
    PCClientPkg_Lxzy, //������ҵ ʵ�� ���ݰ�
    PCClientPkg_Dhxz, //����ѡ��
    PCClientPkg_Config, //�������ݰ�
    PCClientPkg_CAN_CONTROL = 4006,
	PCClientPkg_onlineStatus,  //�ͻ�������״̬ 0 ���� 1����  2 ����
	PCClientPkg_Jdq_DO_Statue = 4009 //��ģ�鵱ǰ�̵���״̬
};

/*
 * ������ÿ�����ֵ  ����ÿ��ı�ʶ
 * ���� 6��
 */
enum   BmqJsz
{
    BmqJsz_layer_1,
    BmqJsz_layer_2,
    BmqJsz_layer_3,
    BmqJsz_layer_4,
    BmqJsz_layer_5,
    BmqJsz_layer_6,
    BmqJsz_layer_max,
};

/*
 * ���豸 ���������Ϣ
 */
#define CONFIG_DHXZ          "5001"  //����ѡ������ ID
//˫�����ݼ���ֵ ����
#define CONFIG_BMQ_JSZ       "0003"  //����ѡ������ ID

/*
�������� ����������
*/
#define BMQ_DIRTY_DATA  0x18000

/*
* ���豸 �ɼ� ���ʱ���趨
*/
//�ɼ� �豸 ��� ����(ms)
#define  Collect_jdq_interval  100   //ms
#define  Collect_bmq_interval  100	//ms

//������������Χ
#define  Bodong_bmq  1

#endif // ENUM_H
