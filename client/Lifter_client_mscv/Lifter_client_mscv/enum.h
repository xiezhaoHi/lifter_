#ifndef ENUM_H
#define ENUM_H
#include<QString>
//����һЩ��������

//��ʶ ��� ���� λ��
#define  USER_ID_COUNT 4

typedef struct{
    int flag; //��ʶ������ʲô
    QString strData;//�����������
}struct_data;

//������ݱ�ʶ
enum enum_detection
{
    detection_dy, //��ѹ
     detection_dl, //����
     detection_aqcd,//��ȫ����
     detection_hxdlwz,//�������λ��
     detection_slyxsd, //ʸ�������ٶ�
    detection_pcjd,//ƽ�㾫�ȣ��������ڼ�����������
    detection_jgyl,//��Ҫ������λ�ṹӦ��
                        //��˿��ʹ��״̬
    detection_gssnwbds, //��˿�����ⲿ��˿

                    //����/���ᾲ̬ˮƽ�ȼ�⹦�ܣ�
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
    device_cgq = 1000, //�������豸
    device_bmq = 1001, //������
    device_jdq , //�̵���
    device_max = 3
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
    map_database, //���ݿ�
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
 * ��������
 */
#define  LIFTER_TYPE_YLS "0001" //ҷ��ʽ����������
#define  LIFTER_TYPE_QZS "0002" //ǿ��ʽ����������
#define  LIFTER_TYPE_SCSL "0003" //sc˫��ʩ��������

/*
 * ����ȴ� ʱ��
 */
#define WAIT_FOR_TIME   3000    //3��

/*
 *�Զ������ ���� ���� ʹ�ù̶����Բ���  ѡ����� �� �յ�
 */
//#define ZDY_TASK

#endif // ENUM_H
