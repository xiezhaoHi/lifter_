#ifndef ENUM_H
#define ENUM_H
#include<QString>
//����һЩ��������

//����
#define _TEST_

//�ͻ��˵ȴ������ʱʱ��
#define SOCKET_WAIT_TIME  3000

//���ݿ�����������
#define DATABASE_QUEUE_MAX 5000

//��ʶ ��� ���� λ��
#define  USER_ID_COUNT 4

#define  DISPENSE_TIMES 30 //�ַ���� 30 ����
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
    device_cgq = 1001, //�������豸
    device_bmq = 1002, //������
    device_jdq , //�̵���
    device_can, //CANģ���豸
    device_dydl, //��ѹ����
    device_gss, //��˿��
    device_max
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
    device_jdq_data_len = 9 //�̵�������֡����
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
    PCClientPkg_Jdq = 4001, //���ݰ�Ϊ�̵��� ����
    PCClientPkg_Lxzy, //������ҵ ʵ�� ���ݰ�
    PCClientPkg_Dhxz, //����ѡ��
    PCClientPkg_Config, //�������ݰ�
    PCClientPkg_CAN_CONTROL = 4006,
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
#define CONFIG_DHXZ       "50010001"  //����ѡ������ ID


#endif // ENUM_H