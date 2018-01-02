#include "Lifter_client_mscv.h"

#include<QLineSeries>
#include<QPointF>
#include<QChart>
#include<QLabel>
#include<QTime>
#include<QTextEdit>
#include<QPushButton>
#include<QtMath>
#include<QDomDocument>
#include<QSettings>
#include<qDebug>
#include<QMovie>
#include<QMessageBox>
#include<QScrollBar>
#include<QProcess>

#include"custom/custom.h"
#include"usebuffer.h" //缓冲区类
#include"enum.h"
#include"configdialog.h" //配置窗口类

#include<QStatusBar>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif


//默认的窗口标题
#define DEFTITLE "电梯测控系统(客户端)"


//server* myserver = NULL;
QThread thread;
QT_CHARTS_USE_NAMESPACE
//QString t[] = {QString::fromLocal8Bit("电压")};
QString  str[] = { "电压电流","电流","电压"
,"频率",NULL };

//单位都是cm
static double gSizeR = 10;//圈的半径
static double gSizeP = 1000; //脉冲 刻度
void Lifter_client_mscv::closeEvent(QCloseEvent *event)
{

}

Lifter_client_mscv::Lifter_client_mscv(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	// ui->setupUi(this);


	m_button_state_old_style = QString("QPushButton{background-color:rgb(101,203,211)}QPushButton:pressed{background-color:rgb(21,172,230)}");
	m_button_state_new_style = QString("QPushButton{background-color:rgb(21,172,230)}");

	m_label_bitmap_1 = QString(":/images/gray");
	m_label_bitmap_2 = QString(":/images/red");
	m_label_bitmap_3 = QString(":/images/green");

	//初始化窗口
	InitMainWnd();

	/*启动 客户端 服务*/
	client_manager::GetInstance()->start_server();

	//先实现一个效果 后期再优化  已优化
	connect(&m_changeState_timer, &QTimer::timeout, this, &Lifter_client_mscv::ChangeButtonState);

	/*****************************************************/
	
	//ui->pushButton_zdy->setStyleSheet(m_button_state_old_style);
	//ui->pushButton_sz->setStyleSheet(m_button_state_old_style);



	setWindowIcon(QIcon(":/images/logo"));

	QLabel* msgLabel = new QLabel;
	msgLabel->setMinimumSize(msgLabel->sizeHint());
	msgLabel->setAlignment(Qt::AlignHCenter);

	statusBar()->addWidget(msgLabel);
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框

	statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
	statusBar()->showMessage("软件成功启动!", 3000); // 显示临时信息，时间3秒钟.
	/*****************************************************/

	/*
	* 初始化旋转编码器的一些设置项
	*/
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);
	double configd = iniRead.value("bmqConfig/configR").toDouble();//圈的半径
	double configp = iniRead.value("bmqConfig/configP").toDouble();//脉冲 刻度
	if (0 != configd)
		gSizeR = configd;
	if (0 != configp)
		gSizeP = configp;
	/*****************************************************/
	/*
	应力 初始化
	钢丝绳数据表 显示
	初始化继电器DI DO 设备映射表 
	*/
	 
	//initYlTableView(); //初始化应力数据表
	//initGssTableView();//钢丝绳数据表显示
	//InitButtonIDMap();//初始化继电器DI DO 设备映射表
					 



	//init(); //主窗口界面初始化工作
	qRegisterMetaType<QMap<int, QString> >("QMap<int,QString>");
	qRegisterMetaType<QMap<QString, int> >("QMap<QString, int>");


	ShowWorker* worker = new ShowWorker();
	bool ret = connect(worker, &ShowWorker::showCgqData, this, &Lifter_client_mscv::show_ui_CgqData);
	 connect(worker, &ShowWorker::showJdqData, this, &Lifter_client_mscv::show_ui_JdqData);
	 connect(worker, &ShowWorker::showBmpData, this, &Lifter_client_mscv::show_ui_BmpData);
	 connect(worker, &ShowWorker::showYlData, this, &Lifter_client_mscv::show_ui_YlData);
	 connect(worker, &ShowWorker::showControlRes, this, &Lifter_client_mscv::show_ui_ControlRes);
	 
	worker->moveToThread(&m_thread_pool[0]);
	connect(&m_thread_pool[0], &QThread::finished, worker, &QObject::deleteLater);
	connect(&m_timer_pool[0], &QTimer::timeout, worker, &ShowWorker::analyseAllData); //解析数据包
	m_timer_pool[0].start(10);
	m_thread_pool[0].start();


	/*自定义任务
	*/
	connect(&m_taskThread, &TaskThread::show_button_zdy, this, &Lifter_client_mscv::show_ui_button_zdy);

	/*发送命令*/
	//connect(&client_manager::GetInstance())


	//初始化 界面显示,不同种类电梯 显示不同的控件
	InitLifterList();

	/*
	* 开始 禁用 结束投放 菜单
	*/
	//ui->action_3->setDisabled(true);
}



Lifter_client_mscv::~Lifter_client_mscv()
{
	m_taskThread.stop();
	m_taskThread.wait();

	client_manager::GetInstance()->stop_server();
	m_consumerThread.quit();
	m_consumerThread.wait();
	m_project.quit();
	m_project.wait();
	for (int index = 0; index < MAX_THREAD_POOL; ++index)
	{
		m_thread_pool[index].quit();
		m_thread_pool[index].wait();
	}
	delete ui;
}




/*
*
* 自定义连续作业 流程步骤
*/
void Lifter_client_mscv::on_pushButton_zdy_clicked()
{

	custom m;
	connect(&m, &custom::sendresult, this, &Lifter_client_mscv::get_task_result);
	m.exec();

}

/*
* 处理自定义 任务
*/
void    TaskThread::run()
{
	/*list 为    起点层数  终点层数  周期时间
	* 周期时间 1表示 天数 ,0 表示 次数
	*/
#ifndef ZDY_TASK
	QStringList strSendList;

	QString strSend = QString("<begin>%1</begin>\
                    <end>%2</end>\
                    <times type='%3'>%4</times>")
		.arg(m_list[0])
		.arg(m_list[1])
		.arg(m_list[2])
		.arg(m_times)
		;
	strSendList << strSend;
	client_manager::GetInstance()->send_to_server(QString("4002"), strSendList);
#else
	QStringList strSend;
	for (int index = 0; index <m_times; ++index)
	{
		strSend.clear();
		for (int begin = 0; begin<m_list.size(); ++begin)
		{
			strSend.append(QString("<dev ID = '%1'>%2</dev>").arg(m_list[begin]).arg(1));
		}
		if (!m_stop) //退出线程
			return;
		client_manager::GetInstance()->send_to_server(strSend);
		::Sleep(50);
	}
	emit show_button_zdy();
#endif
}
void Lifter_client_mscv::show_ui_button_zdy(void)
{
	ui->pushButton_zdy->setEnabled(true);
}

void Lifter_client_mscv::get_task_result(QStringList list, int times)
{
	ui->pushButton_zdy->setEnabled(false);
	m_taskThread.m_list = list;
	m_taskThread.m_times = times;
	m_taskThread.start();
}



/*设置电压*/
void Lifter_client_mscv::on_pushButton_sz_clicked(bool checked)
{

}

/*初始化图表 电压 电流*/
void Lifter_client_mscv::init(void)
{
	//    QMap<QString,QLabel*> m_button_ID_to_ctl; //DI设备ID 映射 按钮
	//    QMap<QString,QLabel*> m_label_ID_to_aqcd; //DI设备DI映射 安全触点

	/*电压电流频率*/



	/*图表显示*/
	QLineSeries *series = new QLineSeries();

	series->append(0, 6);
	series->append(2, 4);
	series->append(3, 8);
	series->append(7, 4);
	series->append(10, 5);
	*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);


	for (float index = 0; index < 10; index += 0.5)
	{
		series->append(index, qSin(index));
	}
	QChart *chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setTitle("Simple line chart example");

	ui->graphicsView_charts->setChart(chart);

	/*说明*/
	//电压说明
	ui->label_dy_sm->setText("分辨力0.1v、精度0.2%rdg+0.2%FS");

	//电流说明
	ui->label_dl_sm->setText("分辨力0.1A/1A，精度0.3%rdg+0.3%FS");

	//频率说明
	ui->label_pl_sm->setText("分辨力0.1Hz、精度0.05%");
}

//传感器 加速度 水平度
void Lifter_client_mscv::show_ui_CgqData(QStringList const& strList
	, QMap<int, QLabel*>& map)
{
	if (strList.size()<6)
		return;
	//  ui->label_jsd_x->setText(QString("%1g").arg(strList[0]));

	//   ui->label_jsd_y->setText(QString("%1g").arg(strList[1]));
	if (map.contains(cgq_jsd_y)&& nullptr != map[cgq_jsd_y])
	{
		map[cgq_jsd_y]->setText(QString("%1g").arg(strList[2]));
	}
	if (map.contains(cgq_spd_x) && nullptr != map[cgq_spd_x])
	{
		map[cgq_spd_x]->setText(QString("%1度").arg(strList[3]));
	}
	if (map.contains(cgq_spd_z) && nullptr != map[cgq_spd_z])
	{
		map[cgq_spd_z]->setText(QString("%1度").arg(strList[4]));
	}
	
	//ui->label_spd_z->setText(strList[5]);

}

//编码器  速度 位置 制动距离
void Lifter_client_mscv::show_ui_BmpData(QString strDir, double iSd, double iWz
	, double zdjl, QMap<int, QLabel*>& map)//方向 速度 位置
{
	map[bmq_fx]->setText(strDir);
	map[bmq_sd]->setText(QString::number(iSd, 'f', 2) + "cm/s");
	map[bmq_wz]->setText(QString::number(iWz, 'f', 2) + "cm");
	if (zdjl != 0)
		map[bmq_zdjl]->setText(QString::number(zdjl, 'f', 2) + "cm");

}

//继电器 开关 控制 显示
/*
map 继电器值 key 为 DI 标识 2001...  value  为 0 或 1
map_id  普通继电器 DI 标识 2001...对应 label
map_id_aqcd 安全触点 DI 标识 对应  label
*/
void Lifter_client_mscv::show_ui_JdqData(QMap<QString, int> map
	, QMap<QString, QLabel*>& map_id, QMap<QString, QLabel*>& map_id_aqcd)
{
	QMap<QString, int>::iterator begin = map.begin(), end = map.end();
	int iValue = -1;
	QString strKey;
	for (; begin != end; ++begin)
	{
		strKey = begin.key(); //DI 标识 2001...
		iValue = begin.value(); //状态 0 关   1 开
		// if(begin.key() < QString("20010021")) //表示不是安全触点
		{

			if (map_id.contains(strKey))
			{
				if (0 == iValue)
					
					map_id[strKey]->setPixmap(m_label_bitmap_2);
				else if (1 == iValue)
					
					map_id[strKey]->setPixmap(m_label_bitmap_3);
				else
					map_id[strKey]->setPixmap(m_label_bitmap_1);
				map_id[strKey]->setVisible(true);

				//20010021 - 20010026 表示平层开关
				if (m_lifter_floor.contains(strKey) && 1 == iValue )
				{
					m_lifter_floor[strKey]->setStyleSheet(m_button_state_old_style);
				}


			}

		}
		//安全触点
		if (map_id_aqcd.contains(strKey))
		{
			if (0 == iValue)
				
				map_id_aqcd[strKey]->setPixmap(m_label_bitmap_2);
			else if (1 == iValue)
				
				map_id_aqcd[strKey]->setPixmap(m_label_bitmap_3);
			else
				map_id_aqcd[strKey]->setPixmap(m_label_bitmap_1);
			map_id_aqcd[strKey]->setVisible(true);
		}

	}


}
//应力
void    Lifter_client_mscv::show_ui_YlData(QString const& strTd, QString const& strData, QStandardItemModel* pModel)
{
	// m_yl_tableV_mode->setItem(0,2,new QStandardItem(QString("通道-1")));
	if (nullptr != pModel)
	{
		pModel->setItem(strTd.toInt(), 2, new QStandardItem(strData));
		return;
	}
	qDebug() << "[show_ui_YlData] # 应力显示 错误,传入的参数无效!";
}


//解析 数据包队列
void ShowWorker::analyseAllData()
{
	QDomDocument doc;
	while (UserBuffer::GetInstance()->ReturnClientServerQueueSize()>0)
	{
		QString strData = UserBuffer::GetInstance()->PopClientServerQueue();
		// qDebug() << strData;
		QString error;
		int row = 0, column = 0;
		QString strDir; //编码器方向
		double  dZs, dWz, dZdjl; //编码器转速 和位置 制动距离
		if (!doc.setContent(strData, false, &error, &row, &column))
		{
			qDebug() << "解析数据错误:" << strData;
			continue;
		}
		// qDebug() << "ui" << strData;
		QDomElement root = doc.firstChildElement();
		QDomElement child, rootFist;
		if (!root.hasAttribute(QString("type")))
			return;
		QString strBelongs = root.attribute(QString("belongs")); //找类型
		/*
		所属电梯
		0001...(曳引式简易升降机)
		0002...(强制式简易升降机)
		0003...(sc双笼施工升降机)
		*/
		QMap<QString, QLabel*>* map_id = nullptr/*普通开关量*/
		, *map_id_aqcd = nullptr;/*安全触点开关量*/;
		QMap<int, QLabel*> *map_cgq = nullptr; /*传感器 数据类型 对应 控件*/;
		QMap<int, QLabel*> *map_bmq = nullptr; /*编码器 数据类型 对应 控件*/;
		QStandardItemModel* pModel = nullptr;
		
		if ("1" == strBelongs.right(1)) //表示 笼A
		{
			map_id = &(m_pMainwindow->m_button_ID_to_ctl_left);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_left);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_left); 
			map_bmq = &(m_pMainwindow->m_bmq_to_label_left);
			pModel = m_pMainwindow->m_yl_tableV_mode_left;
		}
		if ("2" == strBelongs.right(1))//表示 笼B
		{
			map_id = &(m_pMainwindow->m_button_ID_to_ctl_right);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_right);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_right);
			map_bmq = &(m_pMainwindow->m_bmq_to_label_right);
			pModel = m_pMainwindow->m_yl_tableV_mode_right;
		}
		QString strRootID = root.attribute(QString("type")); //找类型
		rootFist = root;
		if (!root.hasAttribute(QString("state"))) //没有state 属性 表示 不是退出
		{

			QStringList strList;
			if (QString("1001") == strRootID) //传感器
			{
				root = rootFist.firstChildElement(QString("jsd")); //加速度 节点开始
				if (!root.isNull()) //节点有效
				{
					child = root.firstChildElement(QString("x"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("y"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("z"));
					if (!child.isNull())
						strList.append(child.text());

					root = rootFist.firstChildElement(QString("jd")); //角度 节点开始
					child = root.firstChildElement(QString("x"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("y"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("z"));
					if (!child.isNull())
						strList.append(child.text());

					emit showCgqData(strList, *map_cgq);
				}
			}
			if (QString("1003") == strRootID) //继电器
			{
				QMap<QString, int>   device_map; //DI 关联的设备 与对应的值 0 1
				root = rootFist.firstChildElement(QString("DI")); //找第一个子节点
				if (!root.isNull())
				{
					while (!root.isNull())
					{
						if (root.hasAttribute(QString("LINK")))
							device_map[root.attribute(QString("LINK"))] = root.text().toInt();
						root = root.nextSiblingElement();
					}
					emit showJdqData(device_map, *map_id, *map_id_aqcd);
				}
				else
				{
					root = rootFist.firstChildElement(QString("RET"));
					//qDebug() << root.text();
				}
			}
			if (QString("1004") == strRootID) //应力通道数据
			{
				root = root.firstChildElement(); //找第一个子节点
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("ID")))
						emit showYlData(root.attribute(QString("ID")), root.text(), pModel);
					root = root.nextSiblingElement();
				}
			}
			if (QString("1002") == strRootID) //编码器
			{

				root = rootFist.firstChildElement(QString("dir")); //找dir子节点 方向
				if (!root.isNull())
				{
					if ("1" == root.text()) //正向
						strDir = "正向";
					else
						strDir = "反向";

				}
				root = rootFist.firstChildElement(QString("jd")); //找jd子节点 角度
				if (!root.isNull())
				{

				}

				root = rootFist.firstChildElement(QString("zs")); //找zs子节点 转速
				if (!root.isNull())
				{

					dZs = root.text().toDouble();
					dZs = dZs*gSizeR * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //找jsz子节点 计数值
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					dWz = dWz * 2 * 3.14*gSizeR / (2 * gSizeP);
				}
				root = rootFist.firstChildElement(QString("zdjl")); //找zdjl子节点 制动距离

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}

				emit showBmpData(strDir, dZs, dWz, dZdjl, *map_bmq);
			}
			if (QString("4005") == strRootID) //表示 服务器返回的 回应包
			{
				root = rootFist.firstChildElement(QString("ret"));
				if (!root.isNull())
				{
					//填充 服务器 应答队列
					UserBuffer::GetInstance()->PushServerResponse(root.text().toInt());
					
				}

			}

		}
		else //表示 退出
		{
			if (QString("1002") == strRootID) //继电器退出
			{
				QMap<QString, int>   device_map; //DI 关联的设备 与对应的值 0 1
				root = rootFist.firstChildElement(); //找第一个子节点
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("LINK")))
						device_map[root.attribute(QString("LINK"))] = root.text().toInt();
					root = root.nextSiblingElement();
				}
				emit showJdqData(device_map, *map_id, *map_id_aqcd);
				
			}
		}

	}
}

void ShowWorker::analyseBmqData()
{

}

//解析数据 并显示数据
void ShowWorker::consumer()
{

}

void ShowWorker::start(void)
{

}


//初始化 map 
void  Lifter_client_mscv::InitButtonIDMap(void)
{

}
int i = 0;
/*
* 处理所有按钮点击事件
*
*/
bool Lifter_client_mscv::eventFilter(QObject *watched, QEvent *event)
{
	//按钮下压
	if (event->type() == QEvent::MouseButtonPress)
	{

		
	}
	

	//按钮释放
	if (event->type() == QEvent::MouseButtonRelease)
	{
		QStringList strSend;
		QString strID, strLifter;
		if (m_button_ID_left.contains((QPushButton*)watched))
		{
			strID = m_button_ID_left[(QPushButton*)watched];
			strLifter = "0001";
		}
		else if (m_button_ID_right.contains((QPushButton*)watched))
		{
			 strID = m_button_ID_right[(QPushButton*)watched];
			 strLifter = "0002";
		}
		else
			return QMainWindow::eventFilter(watched, event);
		if (m_choose_lifter.isEmpty())
		{
			m_choose_lifter = "0003";
		}
		strSend.append(QString("<dev ID = '%1'>%2</dev><lifter>%3</lifter>")
			.arg(strID).arg(i).arg(QString("%1%2").arg(m_choose_lifter.left(4))
				.arg(strLifter)));
		bool retB = client_manager::GetInstance()->send_to_server(QString("4001"), strSend);
		i = 1 - i;
		if (retB)
			statusBar()->showMessage("发送控制命令成功,等待回复!", 3000); // 显示临时信息，时间3秒钟.
		else
			statusBar()->showMessage("发送控制命令失败,网络错误!", 3000); // 显示临时信息，时间3秒钟.

	}

	//双击 选择按钮时 取消 选择
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		QStringList strSend;
		QString strID, strLifter;
		if (m_button_ID_left.contains((QPushButton*)watched))
		{
			strID = m_button_ID_left[(QPushButton*)watched];
			strLifter = "0001";
		}
		else if (m_button_ID_right.contains((QPushButton*)watched))
		{
			strID = m_button_ID_right[(QPushButton*)watched];
			strLifter = "0002";
		}
		else
			return QMainWindow::eventFilter(watched, event);
		if (m_choose_lifter.isEmpty())
		{
			m_choose_lifter = "0003";
		}

		strSend.append(QString("<dev ID = '%1'>%2</dev><lifter>%3</lifter>").arg(strID).arg(0)
			.arg(QString("%1%2").arg(m_choose_lifter.left(4))
			.arg(strLifter)));
		client_manager::GetInstance()->send_to_server(QString("4001"), strSend);

	}

	return QMainWindow::eventFilter(watched, event);
}
/*
* 控制按钮 点击 改变 背景色
*/
void Lifter_client_mscv::ChangeButtonState()
{
	if (m_changeState_button)
	{
		m_changeState_button->setStyleSheet(m_button_state_old_style);
		m_changeState_button = NULL;
	}
}

void Lifter_client_mscv::on_action_triggered()
{
	ConfigDialog dlg(gSizeR, gSizeP);
	connect(&dlg, &ConfigDialog::SetBmqConfig, this, &Lifter_client_mscv::SetBmqConfig);
	connect(&dlg, &ConfigDialog::SetTitle, this, &Lifter_client_mscv::SetTitle);
	dlg.exec();

}

void  Lifter_client_mscv::SetTitle(QString const& strName)
{

	setWindowTitle(DEFTITLE + QString("-") + strName);
}

/*
* 编码器配置
*/
void  Lifter_client_mscv::SetBmqConfig(double d, double p, QString const& strLifter, QString const& strBmqID)
{
	if (strLifter.isEmpty())
		return;

	m_choose_lifter = strLifter; //保存选择的电梯ID
	gSizeR = d;
	gSizeP = p;

	//发送配置信息
	QStringList strSend;

	strSend.append(QString("<lifter>%1</lifter>\
                     <bmqID>%2</bmqID>\
                     <configR>%3</configR> \
                     <configP>%4</configP>")
		.arg(strLifter)
		.arg(strBmqID)
		.arg(d)
		.arg(p));
	client_manager::GetInstance()->send_to_server(QString("4004"), strSend);

	// Sleep(10000);
}
/*
* 初始化 m_lifter_list
* 
* 不同电梯 开启不同的操作界面
*
*/
void    Lifter_client_mscv::InitLifterList(void)
{
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	QString strRet;

	//查看是否选择了 关联的电梯
	strRet = iniRead.value("lifter/lifter_choose", "").toString();
	if (strRet.isEmpty())
	{
		ConfigDialog dlg;
		connect(&dlg, &ConfigDialog::SetBmqConfig, this, &Lifter_client_mscv::SetBmqConfig);
		connect(&dlg, &ConfigDialog::SetTitle, this, &Lifter_client_mscv::SetTitle);

		dlg.exec();
	}
	else
	{
		m_choose_lifter = strRet;
		ConfigDialog dlg;
		QString strTitle = dlg.GetLifterName(m_choose_lifter);
		if (strTitle.isEmpty())
		{
			m_choose_lifter = QString("00010001");
			iniRead.setValue("lifter/lifter_choose", m_choose_lifter);
			strTitle = dlg.GetLifterName(m_choose_lifter);
		}
		setWindowTitle((DEFTITLE + QString("-") + strTitle));
	}



}

//选择动画
void Lifter_client_mscv::on_action_2_triggered()
{
	ui->action_2->setDisabled(true);
	QStringList strSendList;

	QString str = QString("<lifter>%1</lifter><choose>1</choose>").arg(m_choose_lifter);
	strSendList << str;
	client_manager::GetInstance()->send_to_server(QString("4003"), strSendList);

	QTime dieTime = QTime::currentTime().addMSecs(WAIT_FOR_TIME);
	int response = 2;
	while (QTime::currentTime() < dieTime)
	{
		response = UserBuffer::GetInstance()->PopServerResponse();
		if (-1 == response)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		else
			break;
	}
	QString text("请重试!");
	if (0 == response)
		text = "设置失败";
	else if (1 == response)
		text = "设置成功";
	else
		text = "设置超时";
	QMessageBox message(QMessageBox::Warning, "提示", text, QMessageBox::Ok, NULL);
	message.exec();

	if (1 == response)
		ui->action_3->setDisabled(false);
	else
		ui->action_2->setDisabled(false);
}
//结束投放
void Lifter_client_mscv::on_action_3_triggered()
{
	ui->action_3->setDisabled(true);
	QStringList strSendList;

	QString str = QString("<lifter>%1</lifter><choose>0</choose>").arg(m_choose_lifter);
	strSendList << str;
	client_manager::GetInstance()->send_to_server(QString("4003"), strSendList);

	QTime dieTime = QTime::currentTime().addMSecs(WAIT_FOR_TIME);
	int response = 2;
	while (QTime::currentTime() < dieTime)
	{
		response = UserBuffer::GetInstance()->PopServerResponse();
		if (-1 == response)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		else
			break;
	}
	QString text("请重试!");
	if (0 == response)
		text = "设置失败";
	else if (1 == response)
		text = "设置成功";
	else
		text = "设置超时";
	QMessageBox message(QMessageBox::Warning, "提示", text, QMessageBox::Ok, NULL);
	message.exec();
	if (1 == response)
		ui->action_2->setDisabled(false);
	else
		ui->action_3->setDisabled(false);
}

void Lifter_client_mscv::on_action_CAN_triggered()
{

	//QString strCanIp = "192.168.1.118";
	//关闭
	QStringList strSendList;

	QString  str = QString("<lifter>%1</lifter><choose>%2</choose>").arg(m_choose_lifter).arg(0);
	strSendList << str;
	client_manager::GetInstance()->send_to_server(QString("4006"), strSendList);

	QTime dieTime = QTime::currentTime().addMSecs(WAIT_FOR_TIME);
	int response = 2;
	while (QTime::currentTime() < dieTime)
	{
		response = UserBuffer::GetInstance()->PopServerResponse();
		if (-1 == response)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		else
			break;
	}
	if (1 != response)
	{
		QString strTest;

		if (0 == response)
		{
			strTest = "启动配置失败";
		}
		if (-1 == response)
		{
			strTest = "启动配置超时,服务端未启动!";
		}

		QMessageBox message(QMessageBox::Warning, "提示", strTest, QMessageBox::Ok, NULL);
		message.exec();
		return;
	}
	/**********启动配置exe***************/
	QString strPath = qApp->applicationDirPath() + "//config_can.exe";

	QProcess process(this);
	process.execute(strPath);

	/*************************/
	//启动
	strSendList.clear();

	str = QString("<lifter>%1</lifter><choose>%2</choose>").arg(m_choose_lifter).arg(1);
	strSendList << str;
	client_manager::GetInstance()->send_to_server(QString("4006"), strSendList);

	dieTime = QTime::currentTime().addMSecs(WAIT_FOR_TIME);
	response = 2;
	while (QTime::currentTime() < dieTime)
	{
		response = UserBuffer::GetInstance()->PopServerResponse();
		if (-1 == response)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		else
			break;
	}
}


// 返回 操作结果 0 失败 1成功
void    Lifter_client_mscv::show_ui_ControlRes(int ret)
{
	if (0 == ret) //失败
	{
		statusBar()->showMessage("操作失败,请重试!", 3000);
	}
	if (1 == ret) //成功
	{
		statusBar()->showMessage("操作成功!", 3000);
	}
}



/*
初始化界面
主窗口显示施工升降机2个笼的 界面
*/
void Lifter_client_mscv::InitMainWnd()
{
	int iWidth = 1366, iHeight = 768;
	QWidget* pCenterWnd = new QWidget;
	pCenterWnd->setMinimumSize(iWidth, iHeight);
	setCentralWidget(pCenterWnd);
	QGroupBox* pGbLeftA = new QGroupBox();
	pGbLeftA->setTitle("  笼A  ");
	//pGbLeftA->setMinimumWidth(iWidth / 2);
	pGbLeftA->setMinimumHeight(iHeight / 2);

	QGroupBox* pGbRightB = new QGroupBox();
	pGbRightB->setTitle("  笼B  ");
	pGbRightB->setMinimumHeight(iHeight / 2);
/*
左边笼A界面初始化
*/
	//1.1远程控制
#define  UIYCKZ_LEFT
#ifdef UIYCKZ_LEFT
	QGroupBox* pGbLeftYckz_left = new QGroupBox; //远程控制
	pGbLeftYckz_left->setTitle("远程控制");
	QPushButton* pButtonSx_left = new QPushButton("上行");
	QPushButton* pButtonXx_left = new QPushButton("下行");
	QPushButton* pButtonJx_left = new QPushButton("检修");
	QPushButton* pButtonZd_left = new QPushButton("制动");
	QPushButton* pButtonJt_left = new QPushButton("急停");

	QPushButton* pButtonCzbh_left = new QPushButton("超载保护");
	QPushButton* pButtonSxw_left = new QPushButton("上限位");
	QPushButton* pButtonXxw_left = new QPushButton("下限位");
	QPushButton* pButtonSjx_left = new QPushButton("上极限");
	QPushButton* pButtonXjx_left = new QPushButton("下极限");
	QPushButton* pButtonJtkg_left = new QPushButton("急停开关");


	m_button_ID_left[pButtonSx_left] = QString("20020007");
	m_button_ID_left[pButtonXx_left] = QString("20020008");
	m_button_ID_left[pButtonZd_left] = QString("20020009");
	m_button_ID_left[pButtonJt_left] = QString("20020012");
	m_button_ID_left[pButtonJx_left] = QString("20020013");
	m_button_ID_left[pButtonCzbh_left] = QString("20020014");
	m_button_ID_left[pButtonSxw_left] = QString("20020015");
	m_button_ID_left[pButtonXxw_left] = QString("20020016");
	m_button_ID_left[pButtonSjx_left] = QString("20020017");
	m_button_ID_left[pButtonXjx_left] = QString("20020018");
	m_button_ID_left[pButtonJtkg_left] = QString("20020019");

	//1.1.1设置控件的属性
	QMap<QPushButton*, QString>::iterator begin_left = m_button_ID_left.begin()
		, end_left = m_button_ID_left.end();
	QPushButton* pTemp_left = nullptr;
	for (; begin_left != end_left; ++begin_left)
	{
		pTemp_left = nullptr;
		pTemp_left = begin_left.key();
		if (nullptr == pTemp_left)
		{
			continue;
		}
		pTemp_left->installEventFilter(this);
		pTemp_left->setStyleSheet(m_button_state_old_style);
	}

	//1.1.2 远程控制 模块 网格布局
	QGridLayout* pGrid_left = new QGridLayout;
	pGrid_left->addWidget(pButtonSx_left, 1, 1);
	pGrid_left->addWidget(pButtonXx_left, 1, 2);
	pGrid_left->addWidget(pButtonJx_left, 2, 1);
	pGrid_left->addWidget(pButtonZd_left, 2, 2);
	pGrid_left->addWidget(pButtonJt_left, 3, 1);
	pGrid_left->addWidget(pButtonCzbh_left, 3, 2);
	pGrid_left->addWidget(pButtonSxw_left, 4, 1);
	pGrid_left->addWidget(pButtonXxw_left, 4, 2);
	pGrid_left->addWidget(pButtonSjx_left, 5, 1);
	pGrid_left->addWidget(pButtonXjx_left, 5, 2);
	pGrid_left->addWidget(pButtonJtkg_left, 6, 1);
	pGbLeftYckz_left->setLayout(pGrid_left);
#endif
	//1.2 运行状态
#define  UISDZZJK_LEFT
#ifdef UISDZZJK_LEFT
	//1.2.1 创建控件
	QGroupBox* pGbLeftYxzt_left = new QGroupBox; //运行状态
	pGbLeftYxzt_left->setTitle("运行状态");

	QLabel* pLabelSx_text_left = new QLabel("上行");
	QLabel* pLabelXx_text_left = new QLabel("下行");
	QLabel* pLabelJx_text_left = new QLabel("检修");
	QLabel* pLabelZd_text_left = new QLabel("制动");
	QLabel* pLabelJt_text_left = new QLabel("急停");
	QLabel* pLabelCzbh_text_left = new QLabel("超载保护");
	QLabel* pLabelSxw_text_left = new QLabel("上限位");
	QLabel* pLabelXxw_text_left = new QLabel("下限位");
	QLabel* pLabelSjx_text_left = new QLabel("上极限");
	QLabel* pLabelXjx_text_left = new QLabel("下极限");
	QLabel* pLabelJtkg_text_left = new QLabel("急停开关");

	QLabel* pLabelSx_left = new QLabel();
	QLabel* pLabelXx_left = new QLabel();
	QLabel* pLabelJx_left = new QLabel();
	QLabel* pLabelZd_left = new QLabel();
	QLabel* pLabelJt_left = new QLabel();
	QLabel* pLabelCzbh_left = new QLabel();
	QLabel* pLabelSxw_left = new QLabel();
	QLabel* pLabelXxw_left = new QLabel();
	QLabel* pLabelSjx_left = new QLabel();
	QLabel* pLabelXjx_left = new QLabel();
	QLabel* pLabelJtkg_left = new QLabel();

	//1.2.2 存储 DI 对应的 普通开关状态 控件变量
	m_button_ID_to_ctl_left[QString("20010001")] = pLabelSx_left; //上行
	m_button_ID_to_ctl_left[QString("20010002")] = pLabelXx_left; //下行
	m_button_ID_to_ctl_left[QString("20010003")] = pLabelZd_left; //制动
	m_button_ID_to_ctl_left[QString("20010006")] = pLabelJt_left; //急停
	m_button_ID_to_ctl_left[QString("20010007")] = pLabelJx_left; //检修

	//安全触点 开关
	m_label_ID_to_aqcd_left[QString("20010008")] = pLabelCzbh_left;
	m_label_ID_to_aqcd_left[QString("20010009")] = pLabelSxw_left;
	m_label_ID_to_aqcd_left[QString("20010010")] = pLabelXxw_left;
	m_label_ID_to_aqcd_left[QString("20010011")] = pLabelSjx_left;
	m_label_ID_to_aqcd_left[QString("20010012")] = pLabelXjx_left;
	m_label_ID_to_aqcd_left[QString("20010013")] = pLabelJtkg_left;

	//1.2.3 设置开关label 最初的信号灯
	QMap<QString, QLabel*>::iterator beginID_left = m_button_ID_to_ctl_left.begin()
		, endID_left = m_button_ID_to_ctl_left.end(); //DI设备ID 映射 按钮
	QMap<QString, QLabel*>::iterator beginaqcd_left = m_label_ID_to_aqcd_left.begin()
		, endaqcd_left = m_label_ID_to_aqcd_left.end(); //DI设备DI映射 安全触点

	for (; beginID_left != endID_left; ++beginID_left)
		beginID_left.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd_left != endaqcd_left; ++beginaqcd_left)
		beginaqcd_left.value()->setPixmap(m_label_bitmap_1);

	//实时位置 实时速度
// 	QLabel* pLabelWz_text_left = new QLabel("实时位置");
// 	QLabel* pLabelSd_text_left = new QLabel("实时速度");
// 	QLabel* pLabelWz_left = new QLabel;
// 	QLabel* pLabelSd_left = new QLabel;

	QGridLayout* pGridYxzt_left = new QGridLayout;
	pGridYxzt_left->addWidget(pLabelSx_left, 1, 2);
	pGridYxzt_left->addWidget(pLabelXx_left, 1, 4);
	pGridYxzt_left->addWidget(pLabelJx_left, 2, 2);
	pGridYxzt_left->addWidget(pLabelZd_left, 2, 4);
	pGridYxzt_left->addWidget(pLabelJt_left, 3, 2);
	pGridYxzt_left->addWidget(pLabelCzbh_left, 3, 4);
	pGridYxzt_left->addWidget(pLabelSxw_left, 4, 2);
	pGridYxzt_left->addWidget(pLabelXxw_left, 4, 4);
	pGridYxzt_left->addWidget(pLabelSjx_left, 5, 2);
	pGridYxzt_left->addWidget(pLabelXjx_left, 5, 4);
	pGridYxzt_left->addWidget(pLabelJtkg_left, 6, 2);

	pGridYxzt_left->addWidget(pLabelSx_text_left, 1, 1);
	pGridYxzt_left->addWidget(pLabelXx_text_left, 1, 3);
	pGridYxzt_left->addWidget(pLabelJx_text_left, 2, 1);
	pGridYxzt_left->addWidget(pLabelZd_text_left, 2, 3);
	pGridYxzt_left->addWidget(pLabelJt_text_left, 3, 1);
	pGridYxzt_left->addWidget(pLabelCzbh_text_left, 3, 3);
	pGridYxzt_left->addWidget(pLabelSxw_text_left, 4, 1);
	pGridYxzt_left->addWidget(pLabelXxw_text_left, 4, 3);
	pGridYxzt_left->addWidget(pLabelSjx_text_left, 5, 1);
	pGridYxzt_left->addWidget(pLabelXjx_text_left, 5, 3);
	pGridYxzt_left->addWidget(pLabelJtkg_text_left, 6, 1);

// 	pGridYxzt_left->addWidget(pLabelWz_text_left, 7, 1);
// 	pGridYxzt_left->addWidget(pLabelWz_left, 7, 2);
// 
// 	pGridYxzt_left->addWidget(pLabelSd_text_left, 7, 3);
// 	pGridYxzt_left->addWidget(pLabelSd_left, 7, 4);

	pGbLeftYxzt_left->setLayout(pGridYxzt_left);
#endif
	
	//1.3 电压 电流 频率 监测状态
#define  UIDYDL_LEFT
#ifdef  UIDYDL_LEFT
	QGroupBox* pGbLeftDydljczt_left = new QGroupBox; //电压 电流 频率 监测状态
	pGbLeftDydljczt_left->setTitle("电压 电流 频率 检测状态");

	/*图表显示*/
	QLineSeries *series_left = new QLineSeries();

	series_left->append(0, 6);
	series_left->append(2, 4);
	series_left->append(3, 8);
	series_left->append(7, 4);
	series_left->append(10, 5);
	*series_left << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);


	for (float index = 0; index < 10; index += 0.5)
	{
		series_left->append(index, qSin(index));
	}
	QChart *chart_left = new QChart();
	chart_left->legend()->hide();
	chart_left->addSeries(series_left);
	chart_left->createDefaultAxes();
	chart_left->setTitle("Simple line chart example");

	
	QChartView* pChartView_left = new QChartView;
	pChartView_left->setChart(chart_left);
	/*说明*/
	//电压说明
	QLabel* pLabelDySm_left = new QLabel("分辨力0.1v、精度0.2%rdg+0.2%FS");
	//电流说明
	QLabel* pLabelDlSm_left = new QLabel("分辨力0.1A/1A，精度0.3%rdg+0.3%FS");

	//频率说明
	QLabel* pLabelPlSm_left = new QLabel("分辨力0.1Hz、精度0.05%");

	QLabel* pLabelDy_text_left = new QLabel("电压:");
	QLabel* pLabelDl_text_left = new QLabel("电流:");
	QLabel* pLabelPl_text_left = new QLabel("频率:");
	//电压
	QLineEdit* pLabelDy_left = new QLineEdit();
	//电流
	QLineEdit* pLabelDl_left = new QLineEdit();

	//频率
	QLineEdit* pLabelPl_left = new QLineEdit();

	QGridLayout* pGridDydl_left = new QGridLayout;
	
	pGridDydl_left->addWidget(pLabelDy_text_left, 0, 0);
	pGridDydl_left->addWidget(pLabelDy_left, 0, 1);
	pGridDydl_left->addWidget(pLabelDySm_left, 1, 0,1,4);

	pGridDydl_left->addWidget(pLabelDl_text_left, 2, 0);
	pGridDydl_left->addWidget(pLabelDl_left, 2, 1);
	pGridDydl_left->addWidget(pLabelDlSm_left, 3, 0,1,4);

	pGridDydl_left->addWidget(pLabelPl_text_left, 4, 0);
	pGridDydl_left->addWidget(pLabelPl_left, 4, 1);
	pGridDydl_left->addWidget(pLabelPlSm_left, 5, 0,1,4);



	pGbLeftDydljczt_left->setLayout(pGridDydl_left);
#endif
	//1.4 应力 监测  编码器 监测
#define  UIYLJC_LEFT
#ifdef UIYLJC_LEFT
	QGroupBox* pGbLeftYljc_left = new QGroupBox;//应力 监测
	pGbLeftYljc_left->setTitle("应力 检测");
	QTableView* pTableView_left = new QTableView();

	//保存笼a 应力变量
	m_tableView_left = pTableView_left;

	m_yl_tableV_mode_left = new QStandardItemModel();
	m_yl_tableV_mode_left->setColumnCount(4);
	m_yl_tableV_mode_left->setHeaderData(0, Qt::Horizontal, QString("检测部位"));
	m_yl_tableV_mode_left->setHeaderData(1, Qt::Horizontal, QString("正常值"));
	m_yl_tableV_mode_left->setHeaderData(2, Qt::Horizontal, QString("实际值"));
	m_yl_tableV_mode_left->setHeaderData(3, Qt::Horizontal, QString("状态"));
	pTableView_left->setModel(m_yl_tableV_mode_left);
	pTableView_left->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可编辑
																	//表头信息显示居左
	pTableView_left->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//设置列宽不可变
	//    pTableView->horizontalHeader()->setmo(0,QHeaderView::Fixed);
	//    pTableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
	pTableView_left->verticalHeader()->hide();
	pTableView_left->horizontalScrollBar()->setVisible(false);
	pTableView_left->verticalScrollBar()->setVisible(false);
	int nWidth_left = pTableView_left->width() / 4;
	for (int index = 0; index < 4; ++index)
		pTableView_left->setColumnWidth(index, nWidth_left);



	for (int i = 0; i < 32; i++)
	{
		m_yl_tableV_mode_left->setItem(i, 0, new QStandardItem(QString("通道%1").arg(i + 1)));
		//设置字符颜色
		m_yl_tableV_mode_left->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置
		m_yl_tableV_mode_left->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode_left->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_left->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_left->setItem(i, 3, new QStandardItem(QString("xx")));
	}
	/**********************************************/
	//传感器相关数据
	QLabel* pLabelSpdx_text_left = new QLabel("x轴水平度");
	QLabel* pLabelSpdz_text_left = new QLabel("z轴水平度");
	QLabel* pLabelJsdy_text_left = new QLabel("y轴加速度");


	QLabel* pLabelSpdx_left = new QLabel();
	QLabel* pLabelSpdz_left = new QLabel();
	QLabel* pLabelJsdy_left = new QLabel();

	m_cgq_to_label_left[cgq_spd_x] = pLabelSpdx_left;
	m_cgq_to_label_left[cgq_spd_z] = pLabelSpdz_left;
	m_cgq_to_label_left[cgq_jsd_y] = pLabelJsdy_left;
	/**********************************************/

	/**********************************************/
	// 编码器相关监测 数据
	QLabel* pLabelFx_text_left = new QLabel("电梯运行方向");
	
	QLabel* pLabelZdjl_text_left = new QLabel("制动距离");
	QLabel* pLabelWz_text_left = new QLabel("位置");
	QLabel* pLabelJsz_text_left = new QLabel("速度");

	QLabel* pLabelFx_left = new QLabel();
	
	QLabel* pLabelZdjl_left = new QLabel();
	QLabel* pLabelWz_left = new QLabel();
	QLabel* pLabelJsz_left = new QLabel();

	m_bmq_to_label_left[bmq_fx] = pLabelFx_left;
	m_bmq_to_label_left[bmq_zdjl] = pLabelZdjl_left;
	m_bmq_to_label_left[bmq_wz] = pLabelWz_left;
	m_bmq_to_label_left[bmq_sd] = pLabelJsz_left;
	/**********************************************/

	QGridLayout* pLayoutYl_left = new QGridLayout;
	int iRow_left = 30;
	//传感器
	pLayoutYl_left->addWidget(pTableView_left,0,0, iRow_left,4);
	pLayoutYl_left->addWidget(pLabelSpdx_text_left, iRow_left +1, 0);
	pLayoutYl_left->addWidget(pLabelSpdx_left, iRow_left +1, 1);
	pLayoutYl_left->addWidget(pLabelSpdz_text_left, iRow_left +2, 0);
	pLayoutYl_left->addWidget(pLabelSpdz_left, iRow_left +2, 1);
	pLayoutYl_left->addWidget(pLabelJsdy_text_left, iRow_left +3, 0);
	pLayoutYl_left->addWidget(pLabelJsdy_left, iRow_left +3, 1);

	//编码器
	pLayoutYl_left->addWidget(pLabelFx_text_left, iRow_left + 1, 2);
	pLayoutYl_left->addWidget(pLabelFx_left, iRow_left + 1, 3);
	
	pLayoutYl_left->addWidget(pLabelZdjl_text_left, iRow_left + 3, 2);
	pLayoutYl_left->addWidget(pLabelZdjl_left, iRow_left + 3, 3);
	pLayoutYl_left->addWidget(pLabelWz_text_left, iRow_left + 4, 2);
	pLayoutYl_left->addWidget(pLabelWz_left, iRow_left + 4, 3);
	pLayoutYl_left->addWidget(pLabelJsz_text_left, iRow_left + 2, 2);
	pLayoutYl_left->addWidget(pLabelJsz_left, iRow_left + 2, 3);

	pGbLeftYljc_left->setLayout(pLayoutYl_left);
#endif
	//左边 整体布局
	QGridLayout* pGridLeft_left = new QGridLayout;
	pGridLeft_left->addWidget(pGbLeftYckz_left, 0, 0);
	pGridLeft_left->addWidget(pGbLeftYxzt_left, 1, 0);
	pGridLeft_left->addWidget(pGbLeftDydljczt_left, 2, 0);
	pGridLeft_left->addWidget(pGbLeftYljc_left, 0, 1, 3, 1);

	//设置 行比
	pGridLeft_left->setRowStretch(0, 1);
	pGridLeft_left->setRowStretch(1, 1);
	pGridLeft_left->setRowStretch(2, 1);

	pGridLeft_left->setColumnStretch(0, 1);
	pGridLeft_left->setColumnStretch(1, 3);

	pGbLeftA->setLayout(pGridLeft_left);



/*
右边 笼B 窗口界面初始化
*/
//2.1远程控制
#define  UIYCKZ_RIGHT
#ifdef UIYCKZ_RIGHT
	QGroupBox* pGbLeftYckz_right = new QGroupBox; //远程控制
	pGbLeftYckz_right->setTitle("远程控制");
	QPushButton* pButtonSx_right = new QPushButton("上行");
	QPushButton* pButtonXx_right = new QPushButton("下行");
	QPushButton* pButtonJx_right = new QPushButton("检修");
	QPushButton* pButtonZd_right = new QPushButton("制动");
	QPushButton* pButtonJt_right = new QPushButton("急停");

	QPushButton* pButtonCzbh_right = new QPushButton("超载保护");
	QPushButton* pButtonSxw_right = new QPushButton("上限位");
	QPushButton* pButtonXxw_right = new QPushButton("下限位");
	QPushButton* pButtonSjx_right = new QPushButton("上极限");
	QPushButton* pButtonXjx_right = new QPushButton("下极限");
	QPushButton* pButtonJtkg_right = new QPushButton("急停开关");


	m_button_ID_right[pButtonSx_right] = QString("20020007");
	m_button_ID_right[pButtonXx_right] = QString("20020008");
	m_button_ID_right[pButtonZd_right] = QString("20020009");
	m_button_ID_right[pButtonJt_right] = QString("20020012");
	m_button_ID_right[pButtonJx_right] = QString("20020013");
	m_button_ID_right[pButtonCzbh_right] = QString("20020014");
	m_button_ID_right[pButtonSxw_right] = QString("20020015");
	m_button_ID_right[pButtonXxw_right] = QString("20020016");
	m_button_ID_right[pButtonSjx_right] = QString("20020017");
	m_button_ID_right[pButtonXjx_right] = QString("20020018");
	m_button_ID_right[pButtonJtkg_right] = QString("20020019");

	//2.1.1设置控件的属性
	QMap<QPushButton*, QString>::iterator begin_right = m_button_ID_right.begin()
		, end_right = m_button_ID_right.end();
	QPushButton* pTemp_right = nullptr;
	for (; begin_right != end_right; ++begin_right)
	{
		pTemp_right = nullptr;
		pTemp_right = begin_right.key();
		if (nullptr == pTemp_right)
		{
			continue;
		}
		pTemp_right->installEventFilter(this);
		pTemp_right->setStyleSheet(m_button_state_old_style);
	}

	//2.1.2 远程控制 模块 网格布局
	QGridLayout* pGrid_right = new QGridLayout;
	pGrid_right->addWidget(pButtonSx_right, 1, 1);
	pGrid_right->addWidget(pButtonXx_right, 1, 2);
	pGrid_right->addWidget(pButtonJx_right, 2, 1);
	pGrid_right->addWidget(pButtonZd_right, 2, 2);
	pGrid_right->addWidget(pButtonJt_right, 3, 1);
	pGrid_right->addWidget(pButtonCzbh_right, 3, 2);
	pGrid_right->addWidget(pButtonSxw_right, 4, 1);
	pGrid_right->addWidget(pButtonXxw_right, 4, 2);
	pGrid_right->addWidget(pButtonSjx_right, 5, 1);
	pGrid_right->addWidget(pButtonXjx_right, 5, 2);
	pGrid_right->addWidget(pButtonJtkg_right, 6, 1);
	pGbLeftYckz_right->setLayout(pGrid_right);
#endif
	//2.2 运行状态
#define  UISDZZJK_RIGHT
#ifdef UISDZZJK_RIGHT
	//2.2.1 创建控件
	QGroupBox* pGbLeftYxzt_right = new QGroupBox; //运行状态
	pGbLeftYxzt_right->setTitle("运行状态");

	QLabel* pLabelSx_text_right = new QLabel("上行");
	QLabel* pLabelXx_text_right = new QLabel("下行");
	QLabel* pLabelJx_text_right = new QLabel("检修");
	QLabel* pLabelZd_text_right = new QLabel("制动");
	QLabel* pLabelJt_text_right = new QLabel("急停");
	QLabel* pLabelCzbh_text_right = new QLabel("超载保护");
	QLabel* pLabelSxw_text_right = new QLabel("上限位");
	QLabel* pLabelXxw_text_right = new QLabel("下限位");
	QLabel* pLabelSjx_text_right = new QLabel("上极限");
	QLabel* pLabelXjx_text_right = new QLabel("下极限");
	QLabel* pLabelJtkg_text_right = new QLabel("急停开关");

	QLabel* pLabelSx_right = new QLabel();
	QLabel* pLabelXx_right = new QLabel();
	QLabel* pLabelJx_right = new QLabel();
	QLabel* pLabelZd_right = new QLabel();
	QLabel* pLabelJt_right = new QLabel();
	QLabel* pLabelCzbh_right = new QLabel();
	QLabel* pLabelSxw_right = new QLabel();
	QLabel* pLabelXxw_right = new QLabel();
	QLabel* pLabelSjx_right = new QLabel();
	QLabel* pLabelXjx_right = new QLabel();
	QLabel* pLabelJtkg_right = new QLabel();

	//2.2.2 存储 DI 对应的 普通开关状态 控件变量
	m_button_ID_to_ctl_right[QString("20010001")] = pLabelSx_right; //上行
	m_button_ID_to_ctl_right[QString("20010002")] = pLabelXx_right; //下行
	m_button_ID_to_ctl_right[QString("20010003")] = pLabelZd_right; //制动
	m_button_ID_to_ctl_right[QString("20010006")] = pLabelJt_right; //急停
	m_button_ID_to_ctl_right[QString("20010007")] = pLabelJx_right; //检修

																	//安全触点 开关
	m_label_ID_to_aqcd_right[QString("20010008")] = pLabelCzbh_right;
	m_label_ID_to_aqcd_right[QString("20010009")] = pLabelSxw_right;
	m_label_ID_to_aqcd_right[QString("20010010")] = pLabelXxw_right;
	m_label_ID_to_aqcd_right[QString("20010011")] = pLabelSjx_right;
	m_label_ID_to_aqcd_right[QString("20010012")] = pLabelXjx_right;
	m_label_ID_to_aqcd_right[QString("20010013")] = pLabelJtkg_right;

	//2.2.3 设置开关label 最初的信号灯
	QMap<QString, QLabel*>::iterator beginID_right = m_button_ID_to_ctl_right.begin()
		, endID_right = m_button_ID_to_ctl_right.end(); //DI设备ID 映射 按钮
	QMap<QString, QLabel*>::iterator beginaqcd_right = m_label_ID_to_aqcd_right.begin()
		, endaqcd_right = m_label_ID_to_aqcd_right.end(); //DI设备DI映射 安全触点

	for (; beginID_right != endID_right; ++beginID_right)
		beginID_right.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd_right != endaqcd_right; ++beginaqcd_right)
		beginaqcd_right.value()->setPixmap(m_label_bitmap_1);

	//实时位置 实时速度
// 	QLabel* pLabelWz_text_right = new QLabel("实时位置");
// 	QLabel* pLabelSd_text_right = new QLabel("实时速度");
// 	QLabel* pLabelWz_right = new QLabel;
// 	QLabel* pLabelSd_right = new QLabel;

	QGridLayout* pGridYxzt_right = new QGridLayout;
	pGridYxzt_right->addWidget(pLabelSx_right, 1, 2);
	pGridYxzt_right->addWidget(pLabelXx_right, 1, 4);
	pGridYxzt_right->addWidget(pLabelJx_right, 2, 2);
	pGridYxzt_right->addWidget(pLabelZd_right, 2, 4);
	pGridYxzt_right->addWidget(pLabelJt_right, 3, 2);
	pGridYxzt_right->addWidget(pLabelCzbh_right, 3, 4);
	pGridYxzt_right->addWidget(pLabelSxw_right, 4, 2);
	pGridYxzt_right->addWidget(pLabelXxw_right, 4, 4);
	pGridYxzt_right->addWidget(pLabelSjx_right, 5, 2);
	pGridYxzt_right->addWidget(pLabelXjx_right, 5, 4);
	pGridYxzt_right->addWidget(pLabelJtkg_right, 6, 2);

	pGridYxzt_right->addWidget(pLabelSx_text_right, 1, 1);
	pGridYxzt_right->addWidget(pLabelXx_text_right, 1, 3);
	pGridYxzt_right->addWidget(pLabelJx_text_right, 2, 1);
	pGridYxzt_right->addWidget(pLabelZd_text_right, 2, 3);
	pGridYxzt_right->addWidget(pLabelJt_text_right, 3, 1);
	pGridYxzt_right->addWidget(pLabelCzbh_text_right, 3, 3);
	pGridYxzt_right->addWidget(pLabelSxw_text_right, 4, 1);
	pGridYxzt_right->addWidget(pLabelXxw_text_right, 4, 3);
	pGridYxzt_right->addWidget(pLabelSjx_text_right, 5, 1);
	pGridYxzt_right->addWidget(pLabelXjx_text_right, 5, 3);
	pGridYxzt_right->addWidget(pLabelJtkg_text_right, 6, 1);

// 	pGridYxzt_right->addWidget(pLabelWz_text_right, 7, 1);
// 	pGridYxzt_right->addWidget(pLabelWz_right, 7, 2);
// 
// 	pGridYxzt_right->addWidget(pLabelSd_text_right, 7, 3);
// 	pGridYxzt_right->addWidget(pLabelSd_right, 7, 4);

	pGbLeftYxzt_right->setLayout(pGridYxzt_right);
#endif

	//2.3 电压 电流 频率 监测状态
#define  UIDYDL_RIGHT
#ifdef  UIDYDL_RIGHT
	QGroupBox* pGbLeftDydljczt_right = new QGroupBox; //电压 电流 频率 监测状态
	pGbLeftDydljczt_right->setTitle("电压 电流 频率 检测状态");

	/*图表显示*/
	QLineSeries *series_right = new QLineSeries();

	series_right->append(0, 6);
	series_right->append(2, 4);
	series_right->append(3, 8);
	series_right->append(7, 4);
	series_right->append(10, 5);
	*series_right << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);


	for (float index = 0; index < 10; index += 0.5)
	{
		series_right->append(index, qSin(index));
	}
	QChart *chart_right = new QChart();
	chart_right->legend()->hide();
	chart_right->addSeries(series_right);
	chart_right->createDefaultAxes();
	chart_right->setTitle("Simple line chart example");


	QChartView* pChartView_right = new QChartView;
	pChartView_right->setChart(chart_right);
	/*说明*/
	//电压说明
	QLabel* pLabelDySm_right = new QLabel("分辨力0.1v、精度0.2%rdg+0.2%FS");
	//电流说明
	QLabel* pLabelDlSm_right = new QLabel("分辨力0.1A/1A，精度0.3%rdg+0.3%FS");

	//频率说明
	QLabel* pLabelPlSm_right = new QLabel("分辨力0.1Hz、精度0.05%");

	QLabel* pLabelDy_text_right = new QLabel("电压:");
	QLabel* pLabelDl_text_right = new QLabel("电流:");
	QLabel* pLabelPl_text_right = new QLabel("频率:");
	//电压
	QLineEdit* pLabelDy_right = new QLineEdit();
	//电流
	QLineEdit* pLabelDl_right = new QLineEdit();

	//频率
	QLineEdit* pLabelPl_right = new QLineEdit();

	QGridLayout* pGridDydl_right = new QGridLayout;

	pGridDydl_right->addWidget(pLabelDy_text_right, 0, 0);
	pGridDydl_right->addWidget(pLabelDy_right, 0, 1);
	pGridDydl_right->addWidget(pLabelDySm_right, 1, 0, 1, 4);

	pGridDydl_right->addWidget(pLabelDl_text_right, 2, 0);
	pGridDydl_right->addWidget(pLabelDl_right, 2, 1);
	pGridDydl_right->addWidget(pLabelDlSm_right, 3, 0, 1, 4);

	pGridDydl_right->addWidget(pLabelPl_text_right, 4, 0);
	pGridDydl_right->addWidget(pLabelPl_right, 4, 1);
	pGridDydl_right->addWidget(pLabelPlSm_right, 5, 0, 1, 4);



	pGbLeftDydljczt_right->setLayout(pGridDydl_right);
#endif
	//2.4 应力 监测  编码器 监测
#define  UIYLJC_RIGHT
#ifdef UIYLJC_RIGHT
	QGroupBox* pGbLeftYljc_right = new QGroupBox;//应力 监测
	pGbLeftYljc_right->setTitle("应力 检测");
	QTableView* pTableView_right = new QTableView();
	//保存 笼B 应力控件变量
	m_tableView_right = pTableView_right;

	m_yl_tableV_mode_right = new QStandardItemModel();
	m_yl_tableV_mode_right->setColumnCount(4);
	m_yl_tableV_mode_right->setHeaderData(0, Qt::Horizontal, QString("检测部位"));
	m_yl_tableV_mode_right->setHeaderData(1, Qt::Horizontal, QString("正常值"));
	m_yl_tableV_mode_right->setHeaderData(2, Qt::Horizontal, QString("实际值"));
	m_yl_tableV_mode_right->setHeaderData(3, Qt::Horizontal, QString("状态"));
	pTableView_right->setModel(m_yl_tableV_mode_right);
	pTableView_right->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可编辑
																		  //表头信息显示居左
	pTableView_right->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//设置列宽不可变
	//    pTableView->horizontalHeader()->setmo(0,QHeaderView::Fixed);
	//    pTableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
	pTableView_right->verticalHeader()->hide();
	pTableView_right->horizontalScrollBar()->setVisible(false);
	pTableView_right->verticalScrollBar()->setVisible(false);
	int nWidth_right = pTableView_right->width() / 4;
	for (int index = 0; index < 4; ++index)
		pTableView_right->setColumnWidth(index, nWidth_right);



	for (int i = 0; i < 32; i++)
	{
		m_yl_tableV_mode_right->setItem(i, 0
			, new QStandardItem(QString("通道%1").arg(i + 1)));
		//设置字符颜色
		m_yl_tableV_mode_right->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置
		m_yl_tableV_mode_right->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode_right->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_right->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_right->setItem(i, 3, new QStandardItem(QString("xx")));
	}

	/**********************************************/
	// 传感器相关 监测数据
	QLabel* pLabelSpdx_text_right = new QLabel("x轴水平度");
	QLabel* pLabelSpdz_text_right = new QLabel("z轴水平度");
	QLabel* pLabelJsdy_text_right = new QLabel("y轴加速度");


	QLabel* pLabelSpdx_right = new QLabel();
	QLabel* pLabelSpdz_right = new QLabel();
	QLabel* pLabelJsdy_right = new QLabel();

	m_cgq_to_label_right[cgq_spd_x] = pLabelSpdx_right;
	m_cgq_to_label_right[cgq_spd_z] = pLabelSpdz_right;
	m_cgq_to_label_right[cgq_jsd_y] = pLabelJsdy_right;
	/**********************************************/

	/**********************************************/
	// 编码器相关监测 数据
	QLabel* pLabelFx_text_right = new QLabel("电梯运行方向");
	QLabel* pLabelJsz_text_right = new QLabel("速度");
	QLabel* pLabelZdjl_text_right = new QLabel("制动距离");
	QLabel* pLabelWz_text_right = new QLabel("位置");

	QLabel* pLabelFx_right = new QLabel();
	QLabel* pLabelJsz_right = new QLabel();
	QLabel* pLabelZdjl_right = new QLabel();
	QLabel* pLabelWz_right = new QLabel();

	m_bmq_to_label_right[bmq_fx] = pLabelFx_right;
	m_bmq_to_label_right[bmq_sd] = pLabelJsz_right;
	m_bmq_to_label_right[bmq_zdjl] = pLabelZdjl_right;
	m_bmq_to_label_right[bmq_wz] = pLabelWz_right;

	/**********************************************/



	QGridLayout* pLayoutYl_right = new QGridLayout;
	int iRow_right = 30;
	pLayoutYl_right->addWidget(pTableView_right, 0, 0, iRow_right, 4);
	//传感器
	pLayoutYl_right->addWidget(pLabelSpdx_text_right, iRow_right + 1, 0);
	pLayoutYl_right->addWidget(pLabelSpdx_right, iRow_right + 1, 1);
	pLayoutYl_right->addWidget(pLabelSpdz_text_right, iRow_right + 2, 0);
	pLayoutYl_right->addWidget(pLabelSpdz_right, iRow_right + 2, 1);
	pLayoutYl_right->addWidget(pLabelJsdy_text_right, iRow_right + 3, 0);
	pLayoutYl_right->addWidget(pLabelJsdy_right, iRow_right + 3, 1);
	//编码器
	pLayoutYl_right->addWidget(pLabelFx_text_right, iRow_right + 1, 2);
	pLayoutYl_right->addWidget(pLabelFx_right, iRow_right + 1, 3);
	pLayoutYl_right->addWidget(pLabelJsz_text_right, iRow_right + 2, 2);
	pLayoutYl_right->addWidget(pLabelJsz_right, iRow_right + 2, 3);
	pLayoutYl_right->addWidget(pLabelZdjl_text_right, iRow_right + 3, 2);
	pLayoutYl_right->addWidget(pLabelZdjl_right, iRow_right + 3, 3);
	pLayoutYl_right->addWidget(pLabelWz_text_right, iRow_right + 4, 2);
	pLayoutYl_right->addWidget(pLabelWz_right, iRow_right + 4, 3);

	pGbLeftYljc_right->setLayout(pLayoutYl_right);


#endif
	//左边 整体布局
	QGridLayout* pGridLeft_right = new QGridLayout;
	pGridLeft_right->addWidget(pGbLeftYckz_right, 0, 0);
	pGridLeft_right->addWidget(pGbLeftYxzt_right, 1, 0);
	pGridLeft_right->addWidget(pGbLeftDydljczt_right, 2, 0);
	pGridLeft_right->addWidget(pGbLeftYljc_right, 0, 1, 3, 1);

	//设置 行比
	pGridLeft_right->setRowStretch(0, 1);
	pGridLeft_right->setRowStretch(1, 1);
	pGridLeft_right->setRowStretch(2, 1);

	pGridLeft_right->setColumnStretch(0, 1);
	pGridLeft_right->setColumnStretch(1, 3);

	pGbRightB->setLayout(pGridLeft_right);




	QGridLayout* pCenterLayout = new QGridLayout;
	pCenterLayout->addWidget(pGbLeftA, 0, 0);
	pCenterLayout->addWidget(pGbRightB, 0, 1);
	pCenterLayout->setColumnStretch(0, 1);
	pCenterLayout->setColumnStretch(1, 1);
	pCenterWnd->setLayout(pCenterLayout);

}