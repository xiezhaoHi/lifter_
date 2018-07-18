#ifndef ENUM_H
#define ENUM_H
#include<QString>
//包含一些公共数据

//发送到 服务器 成功 与 否 
enum SEND_FLAG
{
	SEND_FLAG_ERROR = -1,
	SEND_FLAG_FAIL,//失败
	SEND_FLAG_SUCESS,
	SEND_FLAG_NOCHANGE,
};


//标识 编号 类型 位数
#define  USER_ID_COUNT 4

typedef struct{
    int flag; //标识数据是什么
    QString strData;//具体的数据项
}struct_data;

//检测数据标识
enum enum_detection
{
    detection_dy, //电压
     detection_dl, //电流
     detection_aqcd,//安全触点
     detection_hxdlwz,//货箱吊笼位置
     detection_slyxsd, //矢量运行速度
    detection_pcjd,//平层精度（仅适用于简易升降机）
    detection_jgyl,//主要受力部位结构应力
                        //钢丝绳使用状态
    detection_gssnwbds, //钢丝绳内外部断丝

                    //吊笼/货厢静态水平度检测功能：
    detection_spd,//吊笼/货厢水平度

                    //吊笼/货厢运行加、减速度监测功能
    detection_jjsd,//吊笼/货厢的运行加减速度
    detection_zdjl,//制动距离的自动检测功能 制动距离
    detection_max
};

/*设备 标识*/
#define device_begin_ID 1000
enum device_name_config
{
    device_cgq = 1000, //传感器设备
    device_bmq = 1001, //编码器
    device_jdq , //继电器
    device_max =3
};
/*设备对应的数据帧长度*/
enum device_data_len
{
    device_cgq_data_len = 11, //传感器单条信息长度
    device_bmq_data_len = 8, //编码器数据帧长度
    device_jdq_data_len = 9 //继电器数据帧长度
};

/*用户标识*/
enum client_name_config
{
    client_qzsjysjj, //强制式简易升降机
    client_max
};

/*数据库配置信息*/
enum database_config
{
    database_hostName, //主机名字 ip
    database_dbName, //数据库名字
    database_userName, //用户名
    database_password, //密码
    database_dbType, //数据库类型
    database_test, //测试链接与否
    database_testSql, //测试sql
    database_maxWaitTime, //最长等待链接时间
    database_waitTime, //等待时间
    database_maxConnectCount, //最大连接数
    database_max
};

/*日志错误级别
DEBUG、INFO、WARN、ERROR和FATAL
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
 * 配置文件中
 * 表示不同的map
 */

enum map_name
{
    map_device = 0, //设备
    map_client, //客户端
    map_database, //数据库
    map_max
};

/*
 *
 * 数据库中 设备的配置信息
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
 * 获取 设备 或 client 信息map
 */
enum device_or_client
{
    device_or_client_device = 0,
    device_or_client_client,
    device_or_client_max
};


/*
 * config_observe_control_device 对应的项
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
 * 电梯种类
 */
#define  LIFTER_TYPE_YLS "0001" //曳引式简易升降机
#define  LIFTER_TYPE_QZS "0002" //强制式简易升降机
#define  LIFTER_TYPE_SCSL "0003" //sc双笼施工升降机
#define  LIFTER_SC_ID_A "00030001" //sc双笼 A		(面对电梯 左手)
#define  LIFTER_SC_ID_B "00030002" //sc双笼 B		(面对电梯 右手)
/*
 * 定义等待 时间
 */
#define WAIT_FOR_TIME   3000    //3秒

/*
 *自定义测试 步骤 或者 使用固定测试步骤  选择起点 和 终点
 */
//#define ZDY_TASK
//客户端通知消息
//"<msg>0</msg>"
#define  CLIENTMSG "4007" 
enum client_msg
{
	client_offline = 0,//下线
	client_online,  //上线
	client_heartbeat,  //心跳消息
	client_msg_max
};


/*
开关状态
*/
enum JDQ_DO_flag
{
	//关 开
	JDQ_DO_flag_close = 0,
	JDQ_DO_flag_open = 1
};

/*
打开开关是 常开 还是 打开一段时间就关闭(点动)
*/
enum JDQ_DO_open_flag
{
	JDQ_DO_open_all_yes, //长时间 保持
	JDQ_DO_open_all_no  //点动
};

/*
继电器 开关 设置标志
*/
#define  JDQ_DO_set_flag  "4001"

/*
对于继电器开关的定义标志
*/

#define JDQ_DO_flag_shangX		"20020007"	//上行
#define JDQ_DO_flag_xiaX		"20020008"	//下行
#define JDQ_DO_flag_jiT			"20020012"	//急停
#define JDQ_DO_flag_fuW			"20020013"	//故障报警 / 复位
#define JDQ_DO_flag_qiD			"20020014"	//电铃 / 启动
#define JDQ_DO_flag_shouBST		"20020015"	//手柄上提(动作许可)
#define JDQ_DO_flag_renLSB		"20020016"	//人脸识别
#define JDQ_DO_flag_yaoS		"20020017"	//钥匙
#define JDQ_DO_flag_jiaS		"20020018"	//加速


#endif // ENUM_H
