#ifndef ENUM_H
#define ENUM_H
#include<QString>
//����һЩ��������

//���͵� ������ �ɹ� �� �� 
enum SEND_FLAG
{
	SEND_FLAG_ERROR = -1,
	SEND_FLAG_FAIL,//ʧ��
	SEND_FLAG_SUCESS,
	SEND_FLAG_NOCHANGE,
};


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
    device_max =3
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
#define  LIFTER_SC_ID_A "00030001" //sc˫�� A		(��Ե��� ����)
#define  LIFTER_SC_ID_B "00030002" //sc˫�� B		(��Ե��� ����)
/*
 * ����ȴ� ʱ��
 */
#define WAIT_FOR_TIME   3000    //3��

/*
 *�Զ������ ���� ���� ʹ�ù̶����Բ���  ѡ����� �� �յ�
 */
//#define ZDY_TASK
//�ͻ���֪ͨ��Ϣ
//"<msg>0</msg>"
#define  CLIENTMSG "4007" 
enum client_msg
{
	client_offline = 0,//����
	client_online,  //����
	client_heartbeat,  //������Ϣ
	client_msg_max
};


/*
����״̬
*/
enum JDQ_DO_flag
{
	//�� ��
	JDQ_DO_flag_close = 0,
	JDQ_DO_flag_open = 1
};

/*
�򿪿����� ���� ���� ��һ��ʱ��͹ر�(�㶯)
*/
enum JDQ_DO_open_flag
{
	JDQ_DO_open_all_yes, //��ʱ�� ����
	JDQ_DO_open_all_no  //�㶯
};

/*
�̵��� ���� ���ñ�־
*/
#define  JDQ_DO_set_flag  "4001"

/*
���ڼ̵������صĶ����־
*/

#define JDQ_DO_flag_shangX		"20020007"	//����
#define JDQ_DO_flag_xiaX		"20020008"	//����
#define JDQ_DO_flag_jiT			"20020012"	//��ͣ
#define JDQ_DO_flag_fuW			"20020013"	//���ϱ��� / ��λ
#define JDQ_DO_flag_qiD			"20020014"	//���� / ����
#define JDQ_DO_flag_shouBST		"20020015"	//�ֱ�����(�������)
#define JDQ_DO_flag_renLSB		"20020016"	//����ʶ��
#define JDQ_DO_flag_yaoS		"20020017"	//Կ��
#define JDQ_DO_flag_jiaS		"20020018"	//����


#endif // ENUM_H
