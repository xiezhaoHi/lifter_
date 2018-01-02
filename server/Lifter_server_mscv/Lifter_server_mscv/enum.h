#ifndef ENUM_H
#define ENUM_H
#include<QString>
//包含一些公共数据

//测试
#define _TEST_

//客户端等待的最大超时时间
#define SOCKET_WAIT_TIME  3000

//数据库队列最大容量
#define DATABASE_QUEUE_MAX 5000

//标识 编号 类型 位数
#define  USER_ID_COUNT 4

#define  DISPENSE_TIMES 30 //分发间隔 30 毫秒
typedef struct{
    int flag; //标识数据是什么
    QString strData;//具体的数据项
}struct_data;

//检测数据标识o
enum enum_detection
{
    detection_dydl, //电压 电流
     detection_aqcd,//安全触点
     detection_hxdlwz,//货箱吊笼位置
     detection_slyxsd, //矢量运行速度
    detection_pcjd,//平层精度（仅适用于简易升降机）
    detection_jgyl,//主要受力部位结构应力
                        //钢丝绳使用状态
    detection_gssnwbds, //钢丝绳内外部断丝

                    //吊笼/货厢静态水平度检测功能：
    detection_cgq, //传感器信息
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
    device_cgq = 1001, //传感器设备
    device_bmq = 1002, //编码器
    device_jdq , //继电器
    device_can, //CAN模块设备
    device_dydl, //电压电流
    device_gss, //钢丝绳
    device_max
};

/*
 * 数据库 连接 标识
 */
enum connect_db_flag
{
    connect_db_mydb, //程序自己的db
    connect_db_gssdb //钢丝绳 db
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
    map_database, // 程序数据库配置
    map_database_gss, //钢丝绳 数据库配置
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
 * 更改:
 * session.h session.cpp 只处理 动画 和显示控制客户端
 * 设备 数据采集和 控制 交给DeviceControl.h DeviceControl.cpp
 */
#define     SESSION_TO_DEVICECONTROL


/*
 *
 * CAN module lib 初始化
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
 * pc客户端数据包 解析
 */
enum PCClientPkg
{
    PCClientPkg_Jdq = 4001, //数据包为继电器 控制
    PCClientPkg_Lxzy, //连续作业 实验 数据包
    PCClientPkg_Dhxz, //动画选择
    PCClientPkg_Config, //配置数据包
    PCClientPkg_CAN_CONTROL = 4006,
};

/*
 * 编码器每层计数值  电梯每层的标识
 * 暂设 6层
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
 * 除设备 外的配置信息
 */
#define CONFIG_DHXZ       "50010001"  //动画选择配置 ID


#endif // ENUM_H
