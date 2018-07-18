#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lifter_client_mscv.h"

#include<QList>
#include<QGraphicsPixmapItem>
#include<QThread>
#include<QTimer>
#include"client_manager.h"
#include<QStandardItemModel>


/*
传感器相关数据
*/
enum CGQ_DATA
{
	cgq_jsd_y, //加速度 y轴
	cgq_spd_x, //水平度 x轴
	cgq_spd_z, //水平度 z轴
	cgq_max
};

/*
编码器相关数据监测
*/
enum BMQ_DATA
{
	bmq_fx, //电梯运行方向
	bmq_sd, //速度
	bmq_wz, //位置
	bmq_zdjl, //制动距离
	bmq_max
};


/*
电源 3相 电压 、电流、频率
*/
enum DYDATA_SHOW
{
	DYDATA_SHOW_DY, //电源 电压
	DYDATA_SHOW_DL, //电源 电流
	DYDATA_SHOW_PL, //电源 频率
	DYDATA_SHOW_max
};
enum DYDATA_DYDLPL
{
	DYDATA_DYDLPL_A,  //3相电  A相
	DYDATA_DYDLPL_B,  //3相电  B相
	DYDATA_DYDLPL_C,  //3相电  C相
	DYDATA_DYDLPL_max
};


//控制按钮 属性 和 相关联的 控件
typedef struct BtAttribute
{
	bool  m_flag; //启动与否的标志 
	QString  m_strName; //按钮的名字
	QPushButton* m_bt_renLSB;//人脸识别 开关
	QPushButton* m_bt_yaoS; //钥匙开关
	QPushButton* m_bt_qiD; //启动 开关
	QPushButton* m_bt_dir; //1.加速 前置(上、下 开关) 
	QPushButton* m_bt_jiaS; //加速按钮
	BtAttribute(QString & strName)
	{
		m_flag = false;
		m_strName = strName;
		m_bt_renLSB = nullptr;
		m_bt_yaoS = nullptr;
		m_bt_qiD = nullptr;
		m_bt_dir = nullptr;

		m_bt_jiaS = nullptr;
	}
}BtAttribute;


/*
* 自定义任务 处理线程
*/
class TaskThread :public QThread
{
	Q_OBJECT
public:
	volatile  bool    m_stop;
	QStringList m_list;
	int m_times;
	TaskThread() :m_stop(true)
	{}
	void run();
public:

	void stop()
	{
		m_stop = false;
	}
signals:
	void show_button_zdy();
};

#define MAX_THREAD_POOL 5
class QPushButton;
class QLabel;
namespace Ui {
	class MainWindow;
}
class Lifter_client_mscv : public QMainWindow
{
	Q_OBJECT

public:
	Lifter_client_mscv(QWidget *parent = Q_NULLPTR);
	~Lifter_client_mscv();
private:
	Ui::MainWindow* ui;

	ClientServer m_client;


	void closeEvent(QCloseEvent *event);
	//void mouseReleaseEvent(QMouseEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
public:
	/*
	笼A
	*/
	QMap<QString, QLabel*> m_button_ID_to_ctl_left; //DI设备ID 映射 按钮
	QMap<QString, QLabel*> m_label_ID_to_aqcd_left; //DI设备DI映射 安全触点
	QMap<int, QLabel*>  m_cgq_to_label_left; //传感器相关 label
	QMap<int, QLabel*>  m_bmq_to_label_left; //编码器相关 label
	QTableView*  m_tableView_left;
	QStandardItemModel*     m_yl_tableV_mode_left;
	QLabel*		m_label_dydl_show_left[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_left[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_left_set;//设置按钮
	/*
	笼B
	*/
	QMap<QString, QLabel*> m_button_ID_to_ctl_right; //DI设备ID 映射 按钮
	QMap<QString, QLabel*> m_label_ID_to_aqcd_right; //DI设备DI映射 安全触点
	QMap<int, QLabel*>  m_cgq_to_label_right; //传感器相关 label
	QMap<int, QLabel*>  m_bmq_to_label_right; //编码器相关 label
	QTableView* m_tableView_right;
	QStandardItemModel*     m_yl_tableV_mode_right;
	QLabel*		m_label_dydl_show_right[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_right[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_right_set;//设置按钮
private:
	/*
	初始化界面
	主窗口显示施工升降机2个笼的 界面
	*/
	void InitMainWnd();



	/*
	* 选择的电梯
	* 电梯的ID
	*/
	QString       m_choose_lifter;

	/*
	* 自定义处理线程
	*/
	TaskThread      m_taskThread;

	
	QStandardItemModel*     m_gss_tableV_mode;

	
	

	QList<QGraphicsPixmapItem*> m_item_list;

	QThread m_consumerThread; //处理数据的线程
	QThread m_project;
	QThread m_thread_pool[MAX_THREAD_POOL];
	QTimer   m_timer;
	QTimer   m_timer2;
	QTimer   m_timer_pool[MAX_THREAD_POOL];

	/*
	笼A 对应界面上 左边 显示
	*/
	QMap<QPushButton*, QString> m_button_ID_left; //按钮 映射DO设备ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_left; //按钮 映射按钮的属性

	/***************************************************/

	/*
	笼B 对应界面上 右边 显示
	*/
	QMap<QPushButton*, QString> m_button_ID_right; //按钮 映射DO设备ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_right; //按钮 映射按钮的属性
  /***************************************************/

	//平层开关的状态ID 对应控制选择开关 按钮 指针
	QMap<QString, QPushButton*> m_lifter_floor;


   /*
	* 保持 改变状态的button
	*/
	QPushButton* m_changeState_button;
	QTimer m_changeState_timer;
	QTimer m_onlineStatus_timer;
	QString m_baseTitle; //基础的标题

	//点击控制button 新旧风格
	QString m_button_state_old_style; 
	QString m_button_state_new_style;

	// gray   信号灯 三种状态颜色
	// red
	// green
	QString m_label_bitmap_1; 
	QString m_label_bitmap_2; 
	QString m_label_bitmap_3; 

	//双笼 两个选择按钮
	QAction*  m_choose_one;
	QAction*  m_choose_two;

	QStringList     m_lifter_list;

	/***********************************************************/

	/*
	* 配置文件中的电梯选择
	* 保存 配置文件中 罗列出来的电梯 ID
	*/

	/*
	* 初始化 m_lifter_list
	*/
	void    InitLifterList(void);

	//初始化上面两个map
	void  InitButtonIDMap(void); 

	// 做一些界面的初始化工作
	void init(void);
	


	public slots:

	/*
	* 编码器配置
	*/
	void  SetBmqConfig(double d, double p, const QString &strLifter, const QString &strBmqID);

	/*
	* 设置窗口名字
	*/
	void  SetTitle(QString const& strName);

	/*
	* 控制按钮 点击 改变 背景色
	*/
	void ChangeButtonState();
	/*
	* 与服务器的连接状态
	*/
	void ShowOnlineStatus();

	/*
	* 自定义任务处理
	*/
	void show_ui_button_zdy(void);
	void get_task_result(QStringList  list, int times);
	void show_ui_CgqData(QStringList const& strList
		, QMap<int, QLabel*>& map);
	//编码器
	void show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl
		, QMap<int, QLabel*>&); //方向 速度 位置

																			   //继电器
	void show_ui_JdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>& map_id
		, QMap<QString, QLabel*>& map_id_aqcd);

	//应力
	void    show_ui_YlData(QString const& strTd, QString const& strData, QStandardItemModel*);

	//返回 操作结果 0 失败 1成功
	void    show_ui_ControlRes(int ret);

	//自定义 测试流程
	void on_pushButton_zdy_clicked();
	void on_pushButton_sz_clicked(bool checked);

	//电源 数据的 显示 
	void show_ui_dyData(int, QString &, QString&, QString&, QLabel***);
	//电源 设置 电压 电流 频率
	void on_pushButton_dySet_right(bool flag);
	void on_pushButton_dySet_left(bool flag);
	private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_CAN_triggered();


};
//处理数据缓冲区中的数据,并更新到界面上
/*
* 处理服务器 数据包
*/

class ShowWorker : public QObject
{

	Q_OBJECT

public:
	ShowWorker(Lifter_client_mscv* pMainwindow)
	{
		m_pMainwindow = pMainwindow;
	}

	Lifter_client_mscv* m_pMainwindow; //主ui窗口的指针
	void start(void);


private:



public slots :

		//!把数据望 缓冲区 存
		// void producer(const QString &parameter) ;

		//!从缓冲区取数据 计算 显示
		//解析数据 并显示数据
		void consumer();

	void analyseBmqData();

	/*
	* 解析所有的数据包
	*/
	void analyseAllData();
signals:

	//传感器
	void showCgqData(QStringList const& strList
		, QMap<int, QLabel*>& map);

	//编码器
	void showBmpData(QString strDir, double iSd, double iWz, double zdjl
		, QMap<int, QLabel*>& map); //方向 速度 位置 制动距离

																		   //继电器
	void showJdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>& map_id
		, QMap<QString, QLabel*>& map_id_aqcd);

	//应力
	void    showYlData(QString const& strTd, QString const& strData, QStandardItemModel*);

	//返回 操作结果 0 失败 1成功
	void    showControlRes(int ret);

	//电源 数据显示
	void showdyData(int, QString &, QString&, QString&, QLabel***);
	
};


