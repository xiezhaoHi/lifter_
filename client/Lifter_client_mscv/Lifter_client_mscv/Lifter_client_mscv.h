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


private:


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


	QStandardItemModel*     m_yl_tableV_mode;

	QList<QGraphicsPixmapItem*> m_item_list;

	QThread m_consumerThread; //处理数据的线程
	QThread m_project;
	QThread m_thread_pool[MAX_THREAD_POOL];
	QTimer   m_timer;
	QTimer   m_timer2;
	QTimer   m_timer_pool[MAX_THREAD_POOL];

	QMap<QString, int> m_device_map; //设备ID对应的控件表
	QMap<int, QString> m_control_map; //控件到设备ID
	QMap<QPushButton*, QString> m_button_ID; //按钮 映射DO设备ID
	QMap<QPushButton*, int>      m_button_keep_state; //保存一些特殊的按钮 点击按钮后 状态改变 一直存在 直到 返回
	QMap<QString, QLabel*> m_button_ID_to_ctl; //DI设备ID 映射 按钮
	QMap<QString, QLabel*> m_label_ID_to_aqcd; //DI设备DI映射 安全触点
	
	//平层开关的状态ID 对应控制选择开关 按钮 指针
	QMap<QString, QPushButton*> m_lifter_floor;


   /*
	* 保持 改变状态的button
	*/
	QPushButton* m_changeState_button;
	QTimer m_changeState_timer;

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
	
					//应力tableView
	void  initYlTableView();

	//钢丝绳tableView
	void  initGssTableView();
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
	* 自定义任务处理
	*/
	void show_ui_button_zdy(void);
	void get_task_result(QStringList  list, int times);
	void show_ui_CgqData(QStringList const& strList);
	//编码器
	void show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl); //方向 速度 位置

																			   //继电器
	void show_ui_JdqData(QMap<QString, int> map);

	//应力
	void    show_ui_YlData(QString const& strTd, QString const& strData);

	//返回 操作结果 0 失败 1成功
	void    show_ui_ControlRes(int ret);

	//自定义 测试流程
	void on_pushButton_zdy_clicked();
	void on_pushButton_sz_clicked(bool checked);

	//双笼 电梯 创建两个切换 菜单
	void create_choose_acction();
	private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_CAN_triggered();
	void on_choose_one();
	void on_choose_two();
};
//处理数据缓冲区中的数据,并更新到界面上
/*
* 处理服务器 数据包
*/

class ShowWorker : public QObject
{

	Q_OBJECT

public:
	ShowWorker()
	{

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
	void showCgqData(QStringList const& strList);

	//编码器
	void showBmpData(QString strDir, double iSd, double iWz, double zdjl); //方向 速度 位置 制动距离

																		   //继电器
	void showJdqData(QMap<QString, int> map);

	//应力
	void    showYlData(QString const& strTd, QString const& strData);

	//返回 操作结果 0 失败 1成功
	void    showControlRes(int ret);
};