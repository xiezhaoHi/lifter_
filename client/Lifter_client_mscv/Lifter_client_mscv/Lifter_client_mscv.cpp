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

static double gSizeD = 10;//圈的半径
static double gSizeP = 1000; //脉冲 刻度
void Lifter_client_mscv::closeEvent(QCloseEvent *event)
{

}

Lifter_client_mscv::Lifter_client_mscv(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/*启动 客户端 服务*/
	client_manager::GetInstance()->start_server();

	//先实现一个效果 后期再优化
	connect(&m_changeState_timer, &QTimer::timeout, this, &Lifter_client_mscv::ChangeButtonState);

	m_button_state_old_style = QString("QPushButton{background-color:rgb(101,203,211)}QPushButton:pressed{background-color:rgb(21,172,230)}");
	m_button_state_new_style = QString("QPushButton{background-color:rgb(21,172,230)}");
	/*****************************************************/
	ui->pushButton_zdy->setStyleSheet(m_button_state_old_style);
	ui->pushButton_sz->setStyleSheet(m_button_state_old_style);

	m_label_bitmap_1 = QString(":/images/gray");
	m_label_bitmap_2 = QString(":/images/red");
	m_label_bitmap_3 = QString(":/images/green");

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
		gSizeD = configd;
	if (0 != configp)
		gSizeP = configp;
	/*****************************************************/


	initYlTableView(); //初始化应力数据表
	initGssTableView();//钢丝绳数据表显示
	InitButtonIDMap();//初始化继电器DI DO 设备映射表
					  // ui->lineEdit_mz_size->setPlaceholderText(QString("圈半径(cm)"));
					  // ui->lineEdit_mz_kd->setPlaceholderText(QString("刻度(每转脉冲数)"));
					  //     ui->lineEdit_mz_size->setPlaceholderText(QString("%1").arg(gSizeD));
					  //     ui->lineEdit_mz_kd->setPlaceholderText(QString("%1").arg(gSizeP));


					  //    ui->label_spd_z->setVisible(false);



	init(); //主窗口界面初始化工作
	qRegisterMetaType<QMap<int, QString> >("QMap<int,QString>");
	qRegisterMetaType<QMap<QString, int> >("QMap<QString, int>");
	//!
	//!
	//!
	//! 进行 数据交互类初始化
	//   if(!DataExchange::InitDataExchange())
	//   {

	//   }
	for (int index = 1; index <= 16; ++index)
	{
		m_device_map[QString("200100%1").arg(index, 2, 10, QChar('0'))] = index;
		m_control_map[index] = QString("200200%1").arg(index, 2, 10, QChar('0'));
	}
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

	/*
	* 初始化电梯列表 选择关联的电梯 设置窗口标题
	* 不同电梯 显示不同的界面 
	*/

	m_choose_one = new QAction("选择笼A", this);
	m_choose_two = new QAction("选择笼B", this);
	connect(m_choose_one, &QAction::triggered, this, &Lifter_client_mscv::on_choose_one);
	connect(m_choose_two, &QAction::triggered, this, &Lifter_client_mscv::on_choose_two);
	
	//初始化 界面显示,不同种类电梯 显示不同的控件
	InitLifterList();

	/*
	* 开始 禁用 结束投放 菜单
	*/
	ui->action_3->setDisabled(true);
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

	QMap<QString, QLabel*>::iterator begin = m_button_ID_to_ctl.begin(), end = m_button_ID_to_ctl.end(); //DI设备ID 映射 按钮
	QMap<QString, QLabel*>::iterator beginaqcd = m_label_ID_to_aqcd.begin(), endaqcd = m_label_ID_to_aqcd.end(); //DI设备DI映射 安全触点

	for (; begin != end; ++begin)
		begin.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd != endaqcd; ++beginaqcd)
		beginaqcd.value()->setPixmap(m_label_bitmap_1);

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
void Lifter_client_mscv::show_ui_CgqData(QStringList const& strList)
{
	if (strList.size()<6)
		return;
	//  ui->label_jsd_x->setText(QString("%1g").arg(strList[0]));

	//   ui->label_jsd_y->setText(QString("%1g").arg(strList[1]));

	ui->label_jsd_z->setText(QString("%1g").arg(strList[2]));

	ui->label_spd_x->setText(QString("%1度").arg(strList[3]));

	ui->label_spd_y->setText(QString("%1度").arg(strList[4]));

	//ui->label_spd_z->setText(strList[5]);

}

//编码器  速度 位置 制动距离
void Lifter_client_mscv::show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl)//方向 速度 位置
{
	ui->label_bmq_fx->setText(strDir);
	ui->label_bmq_sd->setText(QString::number(iSd, 'f', 2) + "cm/s");
	ui->label_bmq_wz->setText(QString::number(iWz, 'f', 2) + "cm");
	if (zdjl != 0)
		ui->label_zdjl->setText(QString::number(zdjl, 'f', 2) + "cm");

}

//继电器 开关 控制 显示
void Lifter_client_mscv::show_ui_JdqData(QMap<QString, int> map)
{
	QMap<QString, int>::iterator begin = map.begin(), end = map.end();
	int iValue = -1;
	QString strKey;
	for (; begin != end; ++begin)
	{
		strKey = begin.key();
		iValue = begin.value();
		// if(begin.key() < QString("20010021")) //表示不是安全触点
		{

			if (m_button_ID_to_ctl.contains(strKey))
			{
				if (0 == iValue)
					// m_button_ID_to_ctl[strKey]->setText(QString("关"));
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_2);
				else if (1 == iValue)
					//m_button_ID_to_ctl[strKey]->setText(QString("开"));
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_3);
				else
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_1);
				m_button_ID_to_ctl[strKey]->setVisible(true);

				//20010021 - 20010026 表示平层开关
				if (m_lifter_floor.contains(strKey) && 1 == iValue )
				{
					m_lifter_floor[strKey]->setStyleSheet(m_button_state_old_style);
				}


			}

		}
		//安全触点
		if (m_label_ID_to_aqcd.contains(strKey))
		{
			if (0 == iValue)
				// m_button_ID_to_ctl[strKey]->setText(QString("关"));
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_2);
			else if (1 == iValue)
				//m_button_ID_to_ctl[strKey]->setText(QString("开"));
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_3);
			else
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_1);
			m_label_ID_to_aqcd[strKey]->setVisible(true);
		}

	}

	/*
	* 没有第二个继电器
	* 所以 先显示 死状态
	*/
	//    m_button_ID_to_ctl[QString("20010017")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010017")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010018")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[QString("20010019")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010020")]->setPixmap(m_label_bitmap_2);


}
//应力
void    Lifter_client_mscv::show_ui_YlData(QString const& strTd, QString const& strData)
{
	// m_yl_tableV_mode->setItem(0,2,new QStandardItem(QString("通道-1")));

	m_yl_tableV_mode->setItem(strTd.toInt(), 2, new QStandardItem(strData));
}
//应力对应的数据表
void  Lifter_client_mscv::initYlTableView()
{


	m_yl_tableV_mode = new QStandardItemModel();
	m_yl_tableV_mode->setColumnCount(4);
	m_yl_tableV_mode->setHeaderData(0, Qt::Horizontal, QString("检测部位"));
	m_yl_tableV_mode->setHeaderData(1, Qt::Horizontal, QString("正常值"));
	m_yl_tableV_mode->setHeaderData(2, Qt::Horizontal, QString("实际值"));
	m_yl_tableV_mode->setHeaderData(3, Qt::Horizontal, QString("状态"));
	ui->tableView_yl->setModel(m_yl_tableV_mode);
	ui->tableView_yl->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可编辑
																		  //表头信息显示居左
	ui->tableView_yl->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//设置列宽不可变
	//    ui->tableView_yl->horizontalHeader()->setmo(0,QHeaderView::Fixed);
	//    ui->tableView_yl->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
	ui->tableView_yl->verticalHeader()->hide();
	ui->tableView_yl->horizontalScrollBar()->setVisible(false);
	ui->tableView_yl->verticalScrollBar()->setVisible(false);
	int nWidth = ui->tableView_yl->width() / 4;
	for (int index = 0; index < 4; ++index)
		ui->tableView_yl->setColumnWidth(index, nWidth);



	for (int i = 0; i < 32; i++)
	{
		m_yl_tableV_mode->setItem(i, 0, new QStandardItem(QString("通道%1").arg(i + 1)));
		//设置字符颜色
		m_yl_tableV_mode->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置
		m_yl_tableV_mode->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode->setItem(i, 3, new QStandardItem(QString("xx")));
	}



}

//钢丝绳对应的数据表
void  Lifter_client_mscv::initGssTableView()
{
	m_gss_tableV_mode = new QStandardItemModel();
	m_gss_tableV_mode->setColumnCount(4);
	m_gss_tableV_mode->setHeaderData(0, Qt::Horizontal, QString("检测部位"));
	m_gss_tableV_mode->setHeaderData(1, Qt::Horizontal, QString("正常值"));
	m_gss_tableV_mode->setHeaderData(2, Qt::Horizontal, QString("实际值"));
	m_gss_tableV_mode->setHeaderData(3, Qt::Horizontal, QString("状态"));


	ui->tableView_gss->setModel(m_gss_tableV_mode);

	ui->tableView_gss->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

																		  //表头信息显示居左
	ui->tableView_gss->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableView_gss->verticalHeader()->hide();
	//    ui->tableView_gss->resizeColumnsToContents(); //根据内容自动调整列宽

	//    ui->tableView_gss->resizeRowsToContents(); //根据内容自动调整行高

	ui->tableView_gss->horizontalScrollBar()->setVisible(false);


	//设置列宽不可变
	//    ui->tableView_gss->horizontalHeader()->setmo(0,QHeaderView::Fixed);
	//    ui->tableView_gss->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
	int nWidth = ui->tableView_gss->width() / 4;
	for (int index = 0; index < 4; ++index)
		ui->tableView_gss->setColumnWidth(index, nWidth);
	////    ui->tableView_gss->verticalHeader()->setDefaultSectionSize(nWidth);
	////    ui->tableView_gss->verticalHeader()->setDefaultSectionSize(nWidth);
	//    ui->tableView_gss->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	for (int i = 0; i < 20; i++)
	{
		m_gss_tableV_mode->setItem(i, 0, new QStandardItem(QString("xx")));
		//设置字符颜色
		m_gss_tableV_mode->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置
		m_gss_tableV_mode->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_gss_tableV_mode->setItem(i, 1, new QStandardItem(QString("xx")));
		m_gss_tableV_mode->setItem(i, 2, new QStandardItem(QString("xx")));
		m_gss_tableV_mode->setItem(i, 3, new QStandardItem(QString("xx")));
	}

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

					emit showCgqData(strList);
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
					emit showJdqData(device_map);
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
						emit showYlData(root.attribute(QString("ID")), root.text());
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
					dZs = dZs*gSizeD * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //找jsz子节点 计数值
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					dWz = dWz * 2 * 3.14*gSizeD / (2 * gSizeP);
				}
				root = rootFist.firstChildElement(QString("zdjl")); //找zdjl子节点 制动距离

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}

				emit showBmpData(strDir, dZs, dWz, dZdjl);
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
				emit showJdqData(device_map);
				
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

	//keep state button map


	m_button_keep_state[ui->pushButton_xc_1] = 0;
	m_button_keep_state[ui->pushButton_xc_2] = 0;
	m_button_keep_state[ui->pushButton_xc_3] = 0;
	m_button_keep_state[ui->pushButton_xc_4] = 0;
	m_button_keep_state[ui->pushButton_xc_5] = 0;
	m_button_keep_state[ui->pushButton_xc_6] = 0;
	m_button_keep_state[ui->pushButton_xc_sx] = 0;
	m_button_keep_state[ui->pushButton_xc_xx] = 0;
	//DO
	m_button_ID[ui->pushButton_xc_1] = QString("20020001");
	m_button_ID[ui->pushButton_xc_2] = QString("20020002");
	m_button_ID[ui->pushButton_xc_3] = QString("20020003");
	m_button_ID[ui->pushButton_xc_4] = QString("20020004");
	m_button_ID[ui->pushButton_xc_5] = QString("20020005");
	m_button_ID[ui->pushButton_xc_6] = QString("20020006");
	m_button_ID[ui->pushButton_xc_sx] = QString("20020007");
	m_button_ID[ui->pushButton_xc_xx] = QString("20020008");
	m_button_ID[ui->pushButton_zd] = QString("20020009");
	m_button_ID[ui->pushButton_kgm] = QString("20020010");
	m_button_ID[ui->pushButton_gm] = QString("20020011");

	m_button_ID[ui->pushButton_jt] = QString("20020012");
	m_button_ID[ui->pushButton_jx] = QString("20020013");
	m_button_ID[ui->pushButton_czbh] = QString("20020014");
	m_button_ID[ui->pushButton_xwkg_sxw] = QString("20020015");
	m_button_ID[ui->pushButton_xwkg_xxw] = QString("20020016");
	m_button_ID[ui->pushButton_jxkg_sjx] = QString("20020017");
	m_button_ID[ui->pushButton_jxkg_xjx] = QString("20020018");
	m_button_ID[ui->pushButton_jtkg] = QString("20020019");
	m_button_ID[ui->pushButton_mls_jx] = QString("20020020");
	m_button_ID[ui->pushButton_mls_1] = QString("20020021");
	m_button_ID[ui->pushButton_mls_2] = QString("20020022");
	m_button_ID[ui->pushButton_mls_3] = QString("20020023");
	m_button_ID[ui->pushButton_mls_4] = QString("20020024");
	m_button_ID[ui->pushButton_mls_5] = QString("20020025");
	m_button_ID[ui->pushButton_mls_6] = QString("20020026");


	//DI
	m_button_ID_to_ctl[QString("20010001")] = ui->label_sx; //上行
	m_button_ID_to_ctl[QString("20010002")] = ui->label_xx; //下行
	m_button_ID_to_ctl[QString("20010003")] = ui->label_zd; //制动
	m_button_ID_to_ctl[QString("20010004")] = ui->label_km; //开门
	m_button_ID_to_ctl[QString("20010005")] = ui->label_gm;  //关门
	m_button_ID_to_ctl[QString("20010006")] = ui->label_jt; //急停
	m_button_ID_to_ctl[QString("20010007")] = ui->label_jx; //检修

															
															//1-6平层开关
	m_button_ID_to_ctl[QString("20010021")] = ui->label_mls_1;
	m_button_ID_to_ctl[QString("20010022")] = ui->label_mls_2;
	m_button_ID_to_ctl[QString("20010023")] = ui->label_mls_3;
	m_button_ID_to_ctl[QString("20010024")] = ui->label_mls_4;
	m_button_ID_to_ctl[QString("20010025")] = ui->label_mls_5;
	m_button_ID_to_ctl[QString("20010026")] = ui->label_mls_6;

	//安全触点
	m_label_ID_to_aqcd[QString("20010008")] = ui->label_aqcd1;
	m_label_ID_to_aqcd[QString("20010009")] = ui->label_aqcd2;
	m_label_ID_to_aqcd[QString("20010010")] = ui->label_aqcd3;
	m_label_ID_to_aqcd[QString("20010011")] = ui->label_aqcd4;
	m_label_ID_to_aqcd[QString("20010012")] = ui->label_aqcd5;
	m_label_ID_to_aqcd[QString("20010013")] = ui->label_aqcd6;
	m_label_ID_to_aqcd[QString("20010014")] = ui->label_aqcd7;
	m_label_ID_to_aqcd[QString("20010015")] = ui->label_aqcd8;
	m_label_ID_to_aqcd[QString("20010016")] = ui->label_aqcd9;
	m_label_ID_to_aqcd[QString("20010017")] = ui->label_aqcd10;
	m_label_ID_to_aqcd[QString("20010018")] = ui->label_aqcd11;
	m_label_ID_to_aqcd[QString("20010019")] = ui->label_aqcd12;
	m_label_ID_to_aqcd[QString("20010020")] = ui->label_aqcd13;


	ui->label_aqcd14->setVisible(false);
	ui->label_aqcd15->setVisible(false);
	ui->label_aqcd16->setVisible(false);



	//控制按钮
	QMap<QPushButton*, QString>::iterator begin = m_button_ID.begin(), end = m_button_ID.end();
	QPushButton* pTemp = nullptr;
	for (; begin != end; ++begin)
	{
		pTemp = nullptr;
		pTemp = begin.key();
		if (nullptr == pTemp)
		{
			continue;
		}
		pTemp->installEventFilter(this);
		pTemp->setStyleSheet(m_button_state_old_style);
	}

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

		if (m_button_ID.contains((QPushButton*)watched))
		{

			//((QPushButton*)watched)->setStyleSheet(m_button_state_new_style);
			m_changeState_button = ((QPushButton*)watched);
			//遍历 选层开关 
			for each (QPushButton* var in m_lifter_floor)
			{
				if (var == (QPushButton*)watched)
				{
					m_changeState_button->setStyleSheet(m_button_state_new_style);
				}
			}

		}
	}
	

	//按钮释放
	if (event->type() == QEvent::MouseButtonRelease)
	{

		if (m_button_ID.contains((QPushButton*)watched))
		{

			QString strID = m_button_ID[(QPushButton*)watched];


			QStringList strSend;
			strSend.append(QString("<dev ID = '%1'>%2</dev><lifter>%3</lifter>").arg(strID).arg(i).arg(m_choose_lifter));
			bool retB = client_manager::GetInstance()->send_to_server(QString("4001"), strSend);
			i = 1 - i;
			if (retB)
				statusBar()->showMessage("发送控制命令成功,等待回复!", 3000); // 显示临时信息，时间3秒钟.
			else
				statusBar()->showMessage("发送控制命令失败,网络错误!", 3000); // 显示临时信息，时间3秒钟.
			QThread thread;
			
		}
	}

	//双击 选择按钮时 取消 选择
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		if (m_button_ID.contains((QPushButton*)watched))
		{

			QString strID = m_button_ID[(QPushButton*)watched];


			QStringList strSend;
			strSend.append(QString("<dev ID = '%1'>%2</dev><lifter>%3</lifter>").arg(strID).arg(0).arg(m_choose_lifter));
			client_manager::GetInstance()->send_to_server(QString("4001"), strSend);


			for each (QPushButton* var in m_lifter_floor)
			{
				if (var == (QPushButton*)watched)
				{
					m_changeState_button->setStyleSheet(m_button_state_old_style);
				}
			}
		}
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
	ConfigDialog dlg(gSizeD, gSizeP);
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
	gSizeD = d;
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

		/*
		* 不同电梯 显示不同的界面信息:
		* 暂时实现:通过 按钮等控件的 禁用 与 开启 实现 不同 电梯界面的 显示
		* LIFTER_TYPE_YLS "0001" //曳引式简易升降机
		* LIFTER_TYPE_QZS "0002" //强制式简易升降机
		* LIFTER_TYPE_SCSL "0003" //sc双笼施工升降机
		*/

		//控制按钮DO
		//QMap<QPushButton*, QString>::iterator begin = m_button_ID.begin(), end = m_button_ID.end();

		//普通状态按钮DI
		//QMap<QString, QLabel*>::iterator beginID = m_button_ID_to_ctl.begin(), endID = m_button_ID_to_ctl.end();

		//安全触点按钮DI
		//QMap<QString, QLabel*>::iterator beginID_aqcd = m_label_ID_to_aqcd.begin(), endID_aqcd = m_label_ID_to_aqcd.end();

		QString strLifterID = m_choose_lifter.left(4);
		if (strLifterID == LIFTER_TYPE_YLS) //曳引式 升降机
		{
			//平层开关 ID 对应 控制选层开关的 按钮
			m_lifter_floor[QString("20010021")] = ui->pushButton_xc_1;
			m_lifter_floor[QString("20010022")] = ui->pushButton_xc_2;
			m_lifter_floor[QString("20010023")] = ui->pushButton_xc_3;
			m_lifter_floor[QString("20010024")] = ui->pushButton_xc_4;
			m_lifter_floor[QString("20010025")] = ui->pushButton_xc_5;
			m_lifter_floor[QString("20010026")] = ui->pushButton_xc_6;
		}
		if (strLifterID == LIFTER_TYPE_QZS) //强制式简易升降机
		{
			QMap<QPushButton*, QString>::iterator begin = m_button_ID.begin(), end = m_button_ID.end();
			QPushButton* pTempB = nullptr;
			QString strTempID;
			for (;begin != end; ++begin)
			{
				pTempB = nullptr;
				strTempID = begin.value();
				pTempB = begin.key();
				if (nullptr == pTempB)
				{
					continue;
				}
				//DO 6层控制按钮
				if (QString("20020006") == strTempID)
				{
					pTempB->setEnabled(false);
				}
				//DO 6层门连锁
				if (QString("20020026") == strTempID)
				{
					pTempB->setEnabled(false);
				}
			}

			//DI 显示控件

			//安全出点6层门联锁
			QLabel* pLabel = m_label_ID_to_aqcd[QString("20010020")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(false);
			}
			//普通DI 6层平层开关
			pLabel = m_button_ID_to_ctl[QString("20010026")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(false);
			}

			//平层开关 ID 对应 控制选层开关的 按钮
			m_lifter_floor[QString("20010021")] = ui->pushButton_xc_1;
			m_lifter_floor[QString("20010022")] = ui->pushButton_xc_2;
			m_lifter_floor[QString("20010023")] = ui->pushButton_xc_3;
			m_lifter_floor[QString("20010024")] = ui->pushButton_xc_4;
			m_lifter_floor[QString("20010025")] = ui->pushButton_xc_5;
		}
		if (strLifterID == LIFTER_TYPE_SCSL) //sc双笼施工升降机
		{

			//控制按钮DO
			QMap<QPushButton*, QString>::iterator begin = m_button_ID.begin(), end = m_button_ID.end();
			QPushButton* pTempB = nullptr;
			for (;begin != end; ++begin)
			{
				pTempB = begin.key();
				if (nullptr != pTempB)
				{
					pTempB->setEnabled(false);
				}
				
			}
			//普通状态按钮DI
			QMap<QString, QLabel*>::iterator beginID = m_button_ID_to_ctl.begin(), endID = m_button_ID_to_ctl.end();
			QLabel* pTempID = nullptr;
			for (; beginID != endID; ++beginID)
			{
				pTempID = beginID.value();
				if (nullptr != pTempID)
				{
					pTempID->setEnabled(false);
				}

			}
			//安全触点按钮DI
			QMap<QString, QLabel*>::iterator beginID_aqcd = m_label_ID_to_aqcd.begin(), endID_aqcd = m_label_ID_to_aqcd.end();
			QLabel* pTempID_aqcd = nullptr;
			for (; beginID_aqcd != endID_aqcd; ++beginID_aqcd)
			{
				pTempID_aqcd = beginID_aqcd.value();
				if (nullptr != pTempID_aqcd)
				{
					pTempID_aqcd->setEnabled(false);
				}

			}

			//启用控件

			QMap<QPushButton*, QString>::iterator beginQY = m_button_ID.begin(), endQY = m_button_ID.end();
			QPushButton* pTempBQY = nullptr;
			QString strTempIDQY;
			for (; beginQY != endQY; ++beginQY)
			{
				pTempBQY = nullptr;
				strTempIDQY = beginQY.value();
				pTempBQY = beginQY.key();
				if (nullptr == pTempBQY)
				{
					continue;
				}
				//DO 上行
				if (QString("20020007") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO 下行
				if (QString("20020008") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO 制动
				if (QString("20020009") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO 急停 - 急停开关
				int bID = 12;
				for (;bID <=19; ++bID)
				{
					if (QString("200200%1").arg(bID) == strTempIDQY)
					{
						pTempBQY->setEnabled(true);
					}
				}
			}

			//DI 显示控件

			//安全触6层门联锁
			QLabel* pLabel = nullptr;
			int bID = 6;
			for (; bID <= 13; ++bID)
			{
				pLabel = m_label_ID_to_aqcd[QString("200100%1").arg(bID,2, 10, QLatin1Char('0'))];
				if (nullptr != pLabel)
				{
					pLabel->setEnabled(true);
				}
				pLabel = m_button_ID_to_ctl[QString("200100%1").arg(bID, 2, 10, QLatin1Char('0'))];
				if (nullptr != pLabel)
				{
					pLabel->setEnabled(true);
				}
			}
			//普通DI 6层平层开关
			
			pLabel = m_button_ID_to_ctl[QString("20010001")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(true);
			}
			pLabel = m_button_ID_to_ctl[QString("20010002")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(true);
			}
			pLabel = m_button_ID_to_ctl[QString("20010003")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(true);
			}

			//创建 两个 菜单  默认选择 按配置来
			create_choose_acction();

		}
	}



}
//双笼 电梯 创建两个切换 菜单
void Lifter_client_mscv::create_choose_acction()
{
	QMenuBar* mainMenu = menuBar();
	QMenu *fileMenu = menuBar()->addMenu(QString("选择控制项"));

	fileMenu->addAction(m_choose_one);

	fileMenu->addSeparator();

	fileMenu->addAction(m_choose_two);

	menuBar()->addSeparator();

	
}
void Lifter_client_mscv::on_choose_one()
{
	QString strTemp = m_choose_lifter; //电梯标识
	strTemp = strTemp.left(strTemp.size() - 1) + QString("1");
	m_choose_lifter = strTemp;
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	iniRead.setValue("lifter/lifter_choose", m_choose_lifter);
	//更改标题
	ConfigDialog dlg;
	setWindowTitle((DEFTITLE + QString("-") + dlg.GetLifterName(m_choose_lifter)));
}
void Lifter_client_mscv::on_choose_two()
{
	QString strTemp = m_choose_lifter; //电梯标识
	strTemp = strTemp.left(strTemp.size() - 1) + QString("2");
	m_choose_lifter = strTemp;
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	iniRead.setValue("lifter/lifter_choose", m_choose_lifter);
	//更改标题
	ConfigDialog dlg;
	setWindowTitle((DEFTITLE + QString("-") + dlg.GetLifterName(m_choose_lifter)));
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