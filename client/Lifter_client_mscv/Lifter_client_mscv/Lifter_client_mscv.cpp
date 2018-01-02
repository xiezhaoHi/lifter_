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
#include"usebuffer.h" //��������
#include"enum.h"
#include"configdialog.h" //���ô�����

#include<QStatusBar>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif


//Ĭ�ϵĴ��ڱ���
#define DEFTITLE "���ݲ��ϵͳ(�ͻ���)"


//server* myserver = NULL;
QThread thread;
QT_CHARTS_USE_NAMESPACE
//QString t[] = {QString::fromLocal8Bit("��ѹ")};
QString  str[] = { "��ѹ����","����","��ѹ"
,"Ƶ��",NULL };

static double gSizeD = 10;//Ȧ�İ뾶
static double gSizeP = 1000; //���� �̶�
void Lifter_client_mscv::closeEvent(QCloseEvent *event)
{

}

Lifter_client_mscv::Lifter_client_mscv(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/*���� �ͻ��� ����*/
	client_manager::GetInstance()->start_server();

	//��ʵ��һ��Ч�� �������Ż�
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
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // ���ò���ʾlabel�ı߿�

	statusBar()->setSizeGripEnabled(false); //�����Ƿ���ʾ�ұߵĴ�С���Ƶ�
	statusBar()->showMessage("����ɹ�����!", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.
	/*****************************************************/

	/*
	* ��ʼ����ת��������һЩ������
	*/
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);
	double configd = iniRead.value("bmqConfig/configR").toDouble();//Ȧ�İ뾶
	double configp = iniRead.value("bmqConfig/configP").toDouble();//���� �̶�
	if (0 != configd)
		gSizeD = configd;
	if (0 != configp)
		gSizeP = configp;
	/*****************************************************/


	initYlTableView(); //��ʼ��Ӧ�����ݱ�
	initGssTableView();//��˿�����ݱ���ʾ
	InitButtonIDMap();//��ʼ���̵���DI DO �豸ӳ���
					  // ui->lineEdit_mz_size->setPlaceholderText(QString("Ȧ�뾶(cm)"));
					  // ui->lineEdit_mz_kd->setPlaceholderText(QString("�̶�(ÿת������)"));
					  //     ui->lineEdit_mz_size->setPlaceholderText(QString("%1").arg(gSizeD));
					  //     ui->lineEdit_mz_kd->setPlaceholderText(QString("%1").arg(gSizeP));


					  //    ui->label_spd_z->setVisible(false);



	init(); //�����ڽ����ʼ������
	qRegisterMetaType<QMap<int, QString> >("QMap<int,QString>");
	qRegisterMetaType<QMap<QString, int> >("QMap<QString, int>");
	//!
	//!
	//!
	//! ���� ���ݽ������ʼ��
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
	connect(&m_timer_pool[0], &QTimer::timeout, worker, &ShowWorker::analyseAllData); //�������ݰ�
	m_timer_pool[0].start(10);
	m_thread_pool[0].start();


	/*�Զ�������
	*/
	connect(&m_taskThread, &TaskThread::show_button_zdy, this, &Lifter_client_mscv::show_ui_button_zdy);

	/*��������*/
	//connect(&client_manager::GetInstance())

	/*
	* ��ʼ�������б� ѡ������ĵ��� ���ô��ڱ���
	* ��ͬ���� ��ʾ��ͬ�Ľ��� 
	*/

	m_choose_one = new QAction("ѡ����A", this);
	m_choose_two = new QAction("ѡ����B", this);
	connect(m_choose_one, &QAction::triggered, this, &Lifter_client_mscv::on_choose_one);
	connect(m_choose_two, &QAction::triggered, this, &Lifter_client_mscv::on_choose_two);
	
	//��ʼ�� ������ʾ,��ͬ������� ��ʾ��ͬ�Ŀؼ�
	InitLifterList();

	/*
	* ��ʼ ���� ����Ͷ�� �˵�
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
* �Զ���������ҵ ���̲���
*/
void Lifter_client_mscv::on_pushButton_zdy_clicked()
{

	custom m;
	connect(&m, &custom::sendresult, this, &Lifter_client_mscv::get_task_result);
	m.exec();

}

/*
* �����Զ��� ����
*/
void    TaskThread::run()
{
	/*list Ϊ    ������  �յ����  ����ʱ��
	* ����ʱ�� 1��ʾ ���� ,0 ��ʾ ����
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
		if (!m_stop) //�˳��߳�
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



/*���õ�ѹ*/
void Lifter_client_mscv::on_pushButton_sz_clicked(bool checked)
{

}

/*��ʼ��ͼ�� ��ѹ ����*/
void Lifter_client_mscv::init(void)
{
	//    QMap<QString,QLabel*> m_button_ID_to_ctl; //DI�豸ID ӳ�� ��ť
	//    QMap<QString,QLabel*> m_label_ID_to_aqcd; //DI�豸DIӳ�� ��ȫ����

	/*��ѹ����Ƶ��*/

	QMap<QString, QLabel*>::iterator begin = m_button_ID_to_ctl.begin(), end = m_button_ID_to_ctl.end(); //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*>::iterator beginaqcd = m_label_ID_to_aqcd.begin(), endaqcd = m_label_ID_to_aqcd.end(); //DI�豸DIӳ�� ��ȫ����

	for (; begin != end; ++begin)
		begin.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd != endaqcd; ++beginaqcd)
		beginaqcd.value()->setPixmap(m_label_bitmap_1);

	/*ͼ����ʾ*/
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

	/*˵��*/
	//��ѹ˵��
	ui->label_dy_sm->setText("�ֱ���0.1v������0.2%rdg+0.2%FS");

	//����˵��
	ui->label_dl_sm->setText("�ֱ���0.1A/1A������0.3%rdg+0.3%FS");

	//Ƶ��˵��
	ui->label_pl_sm->setText("�ֱ���0.1Hz������0.05%");
}

//������ ���ٶ� ˮƽ��
void Lifter_client_mscv::show_ui_CgqData(QStringList const& strList)
{
	if (strList.size()<6)
		return;
	//  ui->label_jsd_x->setText(QString("%1g").arg(strList[0]));

	//   ui->label_jsd_y->setText(QString("%1g").arg(strList[1]));

	ui->label_jsd_z->setText(QString("%1g").arg(strList[2]));

	ui->label_spd_x->setText(QString("%1��").arg(strList[3]));

	ui->label_spd_y->setText(QString("%1��").arg(strList[4]));

	//ui->label_spd_z->setText(strList[5]);

}

//������  �ٶ� λ�� �ƶ�����
void Lifter_client_mscv::show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl)//���� �ٶ� λ��
{
	ui->label_bmq_fx->setText(strDir);
	ui->label_bmq_sd->setText(QString::number(iSd, 'f', 2) + "cm/s");
	ui->label_bmq_wz->setText(QString::number(iWz, 'f', 2) + "cm");
	if (zdjl != 0)
		ui->label_zdjl->setText(QString::number(zdjl, 'f', 2) + "cm");

}

//�̵��� ���� ���� ��ʾ
void Lifter_client_mscv::show_ui_JdqData(QMap<QString, int> map)
{
	QMap<QString, int>::iterator begin = map.begin(), end = map.end();
	int iValue = -1;
	QString strKey;
	for (; begin != end; ++begin)
	{
		strKey = begin.key();
		iValue = begin.value();
		// if(begin.key() < QString("20010021")) //��ʾ���ǰ�ȫ����
		{

			if (m_button_ID_to_ctl.contains(strKey))
			{
				if (0 == iValue)
					// m_button_ID_to_ctl[strKey]->setText(QString("��"));
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_2);
				else if (1 == iValue)
					//m_button_ID_to_ctl[strKey]->setText(QString("��"));
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_3);
				else
					m_button_ID_to_ctl[strKey]->setPixmap(m_label_bitmap_1);
				m_button_ID_to_ctl[strKey]->setVisible(true);

				//20010021 - 20010026 ��ʾƽ�㿪��
				if (m_lifter_floor.contains(strKey) && 1 == iValue )
				{
					m_lifter_floor[strKey]->setStyleSheet(m_button_state_old_style);
				}


			}

		}
		//��ȫ����
		if (m_label_ID_to_aqcd.contains(strKey))
		{
			if (0 == iValue)
				// m_button_ID_to_ctl[strKey]->setText(QString("��"));
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_2);
			else if (1 == iValue)
				//m_button_ID_to_ctl[strKey]->setText(QString("��"));
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_3);
			else
				m_label_ID_to_aqcd[strKey]->setPixmap(m_label_bitmap_1);
			m_label_ID_to_aqcd[strKey]->setVisible(true);
		}

	}

	/*
	* û�еڶ����̵���
	* ���� ����ʾ ��״̬
	*/
	//    m_button_ID_to_ctl[QString("20010017")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010017")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010018")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[QString("20010019")]->setPixmap(m_label_bitmap_2);
	//    m_button_ID_to_ctl[ QString("20010020")]->setPixmap(m_label_bitmap_2);


}
//Ӧ��
void    Lifter_client_mscv::show_ui_YlData(QString const& strTd, QString const& strData)
{
	// m_yl_tableV_mode->setItem(0,2,new QStandardItem(QString("ͨ��-1")));

	m_yl_tableV_mode->setItem(strTd.toInt(), 2, new QStandardItem(strData));
}
//Ӧ����Ӧ�����ݱ�
void  Lifter_client_mscv::initYlTableView()
{


	m_yl_tableV_mode = new QStandardItemModel();
	m_yl_tableV_mode->setColumnCount(4);
	m_yl_tableV_mode->setHeaderData(0, Qt::Horizontal, QString("��ⲿλ"));
	m_yl_tableV_mode->setHeaderData(1, Qt::Horizontal, QString("����ֵ"));
	m_yl_tableV_mode->setHeaderData(2, Qt::Horizontal, QString("ʵ��ֵ"));
	m_yl_tableV_mode->setHeaderData(3, Qt::Horizontal, QString("״̬"));
	ui->tableView_yl->setModel(m_yl_tableV_mode);
	ui->tableView_yl->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ɱ༭
																		  //��ͷ��Ϣ��ʾ����
	ui->tableView_yl->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//�����п��ɱ�
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
		m_yl_tableV_mode->setItem(i, 0, new QStandardItem(QString("ͨ��%1").arg(i + 1)));
		//�����ַ���ɫ
		m_yl_tableV_mode->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//�����ַ�λ��
		m_yl_tableV_mode->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode->setItem(i, 3, new QStandardItem(QString("xx")));
	}



}

//��˿����Ӧ�����ݱ�
void  Lifter_client_mscv::initGssTableView()
{
	m_gss_tableV_mode = new QStandardItemModel();
	m_gss_tableV_mode->setColumnCount(4);
	m_gss_tableV_mode->setHeaderData(0, Qt::Horizontal, QString("��ⲿλ"));
	m_gss_tableV_mode->setHeaderData(1, Qt::Horizontal, QString("����ֵ"));
	m_gss_tableV_mode->setHeaderData(2, Qt::Horizontal, QString("ʵ��ֵ"));
	m_gss_tableV_mode->setHeaderData(3, Qt::Horizontal, QString("״̬"));


	ui->tableView_gss->setModel(m_gss_tableV_mode);

	ui->tableView_gss->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ɱ༭

																		  //��ͷ��Ϣ��ʾ����
	ui->tableView_gss->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableView_gss->verticalHeader()->hide();
	//    ui->tableView_gss->resizeColumnsToContents(); //���������Զ������п�

	//    ui->tableView_gss->resizeRowsToContents(); //���������Զ������и�

	ui->tableView_gss->horizontalScrollBar()->setVisible(false);


	//�����п��ɱ�
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
		//�����ַ���ɫ
		m_gss_tableV_mode->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//�����ַ�λ��
		m_gss_tableV_mode->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_gss_tableV_mode->setItem(i, 1, new QStandardItem(QString("xx")));
		m_gss_tableV_mode->setItem(i, 2, new QStandardItem(QString("xx")));
		m_gss_tableV_mode->setItem(i, 3, new QStandardItem(QString("xx")));
	}

}

//���� ���ݰ�����
void ShowWorker::analyseAllData()
{
	QDomDocument doc;
	while (UserBuffer::GetInstance()->ReturnClientServerQueueSize()>0)
	{
		QString strData = UserBuffer::GetInstance()->PopClientServerQueue();
		// qDebug() << strData;
		QString error;
		int row = 0, column = 0;
		QString strDir; //����������
		double  dZs, dWz, dZdjl; //������ת�� ��λ�� �ƶ�����
		if (!doc.setContent(strData, false, &error, &row, &column))
		{
			qDebug() << "�������ݴ���:" << strData;
			continue;
		}
		// qDebug() << "ui" << strData;
		QDomElement root = doc.firstChildElement();
		QDomElement child, rootFist;
		if (!root.hasAttribute(QString("type")))
			return;
		QString strRootID = root.attribute(QString("type")); //������
		rootFist = root;
		if (!root.hasAttribute(QString("state"))) //û��state ���� ��ʾ �����˳�
		{

			QStringList strList;
			if (QString("1001") == strRootID) //������
			{
				root = rootFist.firstChildElement(QString("jsd")); //���ٶ� �ڵ㿪ʼ
				if (!root.isNull()) //�ڵ���Ч
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

					root = rootFist.firstChildElement(QString("jd")); //�Ƕ� �ڵ㿪ʼ
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
			if (QString("1003") == strRootID) //�̵���
			{
				QMap<QString, int>   device_map; //DI �������豸 ���Ӧ��ֵ 0 1
				root = rootFist.firstChildElement(QString("DI")); //�ҵ�һ���ӽڵ�
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
			if (QString("1004") == strRootID) //Ӧ��ͨ������
			{
				root = root.firstChildElement(); //�ҵ�һ���ӽڵ�
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("ID")))
						emit showYlData(root.attribute(QString("ID")), root.text());
					root = root.nextSiblingElement();
				}
			}
			if (QString("1002") == strRootID) //������
			{

				root = rootFist.firstChildElement(QString("dir")); //��dir�ӽڵ� ����
				if (!root.isNull())
				{
					if ("1" == root.text()) //����
						strDir = "����";
					else
						strDir = "����";

				}
				root = rootFist.firstChildElement(QString("jd")); //��jd�ӽڵ� �Ƕ�
				if (!root.isNull())
				{

				}

				root = rootFist.firstChildElement(QString("zs")); //��zs�ӽڵ� ת��
				if (!root.isNull())
				{

					dZs = root.text().toDouble();
					dZs = dZs*gSizeD * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //��jsz�ӽڵ� ����ֵ
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					dWz = dWz * 2 * 3.14*gSizeD / (2 * gSizeP);
				}
				root = rootFist.firstChildElement(QString("zdjl")); //��zdjl�ӽڵ� �ƶ�����

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}

				emit showBmpData(strDir, dZs, dWz, dZdjl);
			}
			if (QString("4005") == strRootID) //��ʾ ���������ص� ��Ӧ��
			{
				root = rootFist.firstChildElement(QString("ret"));
				if (!root.isNull())
				{
					//��� ������ Ӧ�����
					UserBuffer::GetInstance()->PushServerResponse(root.text().toInt());
					
				}

			}

		}
		else //��ʾ �˳�
		{
			if (QString("1002") == strRootID) //�̵����˳�
			{
				QMap<QString, int>   device_map; //DI �������豸 ���Ӧ��ֵ 0 1
				root = rootFist.firstChildElement(); //�ҵ�һ���ӽڵ�
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

//�������� ����ʾ����
void ShowWorker::consumer()
{

}

void ShowWorker::start(void)
{

}


//��ʼ�� map 
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
	m_button_ID_to_ctl[QString("20010001")] = ui->label_sx; //����
	m_button_ID_to_ctl[QString("20010002")] = ui->label_xx; //����
	m_button_ID_to_ctl[QString("20010003")] = ui->label_zd; //�ƶ�
	m_button_ID_to_ctl[QString("20010004")] = ui->label_km; //����
	m_button_ID_to_ctl[QString("20010005")] = ui->label_gm;  //����
	m_button_ID_to_ctl[QString("20010006")] = ui->label_jt; //��ͣ
	m_button_ID_to_ctl[QString("20010007")] = ui->label_jx; //����

															
															//1-6ƽ�㿪��
	m_button_ID_to_ctl[QString("20010021")] = ui->label_mls_1;
	m_button_ID_to_ctl[QString("20010022")] = ui->label_mls_2;
	m_button_ID_to_ctl[QString("20010023")] = ui->label_mls_3;
	m_button_ID_to_ctl[QString("20010024")] = ui->label_mls_4;
	m_button_ID_to_ctl[QString("20010025")] = ui->label_mls_5;
	m_button_ID_to_ctl[QString("20010026")] = ui->label_mls_6;

	//��ȫ����
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



	//���ư�ť
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
* �������а�ť����¼�
*
*/
bool Lifter_client_mscv::eventFilter(QObject *watched, QEvent *event)
{
	//��ť��ѹ
	if (event->type() == QEvent::MouseButtonPress)
	{

		if (m_button_ID.contains((QPushButton*)watched))
		{

			//((QPushButton*)watched)->setStyleSheet(m_button_state_new_style);
			m_changeState_button = ((QPushButton*)watched);
			//���� ѡ�㿪�� 
			for each (QPushButton* var in m_lifter_floor)
			{
				if (var == (QPushButton*)watched)
				{
					m_changeState_button->setStyleSheet(m_button_state_new_style);
				}
			}

		}
	}
	

	//��ť�ͷ�
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
				statusBar()->showMessage("���Ϳ�������ɹ�,�ȴ��ظ�!", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.
			else
				statusBar()->showMessage("���Ϳ�������ʧ��,�������!", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.
			QThread thread;
			
		}
	}

	//˫�� ѡ��ťʱ ȡ�� ѡ��
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
* ���ư�ť ��� �ı� ����ɫ
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
* ����������
*/
void  Lifter_client_mscv::SetBmqConfig(double d, double p, QString const& strLifter, QString const& strBmqID)
{
	if (strLifter.isEmpty())
		return;

	m_choose_lifter = strLifter; //����ѡ��ĵ���ID
	gSizeD = d;
	gSizeP = p;

	//����������Ϣ
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
* ��ʼ�� m_lifter_list
* 
* ��ͬ���� ������ͬ�Ĳ�������
*
*/
void    Lifter_client_mscv::InitLifterList(void)
{
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	QString strRet;

	//�鿴�Ƿ�ѡ���� �����ĵ���
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
		* ��ͬ���� ��ʾ��ͬ�Ľ�����Ϣ:
		* ��ʱʵ��:ͨ�� ��ť�ȿؼ��� ���� �� ���� ʵ�� ��ͬ ���ݽ���� ��ʾ
		* LIFTER_TYPE_YLS "0001" //ҷ��ʽ����������
		* LIFTER_TYPE_QZS "0002" //ǿ��ʽ����������
		* LIFTER_TYPE_SCSL "0003" //sc˫��ʩ��������
		*/

		//���ư�ťDO
		//QMap<QPushButton*, QString>::iterator begin = m_button_ID.begin(), end = m_button_ID.end();

		//��ͨ״̬��ťDI
		//QMap<QString, QLabel*>::iterator beginID = m_button_ID_to_ctl.begin(), endID = m_button_ID_to_ctl.end();

		//��ȫ���㰴ťDI
		//QMap<QString, QLabel*>::iterator beginID_aqcd = m_label_ID_to_aqcd.begin(), endID_aqcd = m_label_ID_to_aqcd.end();

		QString strLifterID = m_choose_lifter.left(4);
		if (strLifterID == LIFTER_TYPE_YLS) //ҷ��ʽ ������
		{
			//ƽ�㿪�� ID ��Ӧ ����ѡ�㿪�ص� ��ť
			m_lifter_floor[QString("20010021")] = ui->pushButton_xc_1;
			m_lifter_floor[QString("20010022")] = ui->pushButton_xc_2;
			m_lifter_floor[QString("20010023")] = ui->pushButton_xc_3;
			m_lifter_floor[QString("20010024")] = ui->pushButton_xc_4;
			m_lifter_floor[QString("20010025")] = ui->pushButton_xc_5;
			m_lifter_floor[QString("20010026")] = ui->pushButton_xc_6;
		}
		if (strLifterID == LIFTER_TYPE_QZS) //ǿ��ʽ����������
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
				//DO 6����ư�ť
				if (QString("20020006") == strTempID)
				{
					pTempB->setEnabled(false);
				}
				//DO 6��������
				if (QString("20020026") == strTempID)
				{
					pTempB->setEnabled(false);
				}
			}

			//DI ��ʾ�ؼ�

			//��ȫ����6��������
			QLabel* pLabel = m_label_ID_to_aqcd[QString("20010020")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(false);
			}
			//��ͨDI 6��ƽ�㿪��
			pLabel = m_button_ID_to_ctl[QString("20010026")];
			if (nullptr != pLabel)
			{
				pLabel->setEnabled(false);
			}

			//ƽ�㿪�� ID ��Ӧ ����ѡ�㿪�ص� ��ť
			m_lifter_floor[QString("20010021")] = ui->pushButton_xc_1;
			m_lifter_floor[QString("20010022")] = ui->pushButton_xc_2;
			m_lifter_floor[QString("20010023")] = ui->pushButton_xc_3;
			m_lifter_floor[QString("20010024")] = ui->pushButton_xc_4;
			m_lifter_floor[QString("20010025")] = ui->pushButton_xc_5;
		}
		if (strLifterID == LIFTER_TYPE_SCSL) //sc˫��ʩ��������
		{

			//���ư�ťDO
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
			//��ͨ״̬��ťDI
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
			//��ȫ���㰴ťDI
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

			//���ÿؼ�

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
				//DO ����
				if (QString("20020007") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO ����
				if (QString("20020008") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO �ƶ�
				if (QString("20020009") == strTempIDQY)
				{
					pTempBQY->setEnabled(true);
				}
				//DO ��ͣ - ��ͣ����
				int bID = 12;
				for (;bID <=19; ++bID)
				{
					if (QString("200200%1").arg(bID) == strTempIDQY)
					{
						pTempBQY->setEnabled(true);
					}
				}
			}

			//DI ��ʾ�ؼ�

			//��ȫ��6��������
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
			//��ͨDI 6��ƽ�㿪��
			
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

			//���� ���� �˵�  Ĭ��ѡ�� ��������
			create_choose_acction();

		}
	}



}
//˫�� ���� ���������л� �˵�
void Lifter_client_mscv::create_choose_acction()
{
	QMenuBar* mainMenu = menuBar();
	QMenu *fileMenu = menuBar()->addMenu(QString("ѡ�������"));

	fileMenu->addAction(m_choose_one);

	fileMenu->addSeparator();

	fileMenu->addAction(m_choose_two);

	menuBar()->addSeparator();

	
}
void Lifter_client_mscv::on_choose_one()
{
	QString strTemp = m_choose_lifter; //���ݱ�ʶ
	strTemp = strTemp.left(strTemp.size() - 1) + QString("1");
	m_choose_lifter = strTemp;
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	iniRead.setValue("lifter/lifter_choose", m_choose_lifter);
	//���ı���
	ConfigDialog dlg;
	setWindowTitle((DEFTITLE + QString("-") + dlg.GetLifterName(m_choose_lifter)));
}
void Lifter_client_mscv::on_choose_two()
{
	QString strTemp = m_choose_lifter; //���ݱ�ʶ
	strTemp = strTemp.left(strTemp.size() - 1) + QString("2");
	m_choose_lifter = strTemp;
	QString strPath = qApp->applicationDirPath() + "\\config\\config.ini";
	QSettings iniRead(strPath, QSettings::IniFormat);

	iniRead.setValue("lifter/lifter_choose", m_choose_lifter);
	//���ı���
	ConfigDialog dlg;
	setWindowTitle((DEFTITLE + QString("-") + dlg.GetLifterName(m_choose_lifter)));
}
//ѡ�񶯻�
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
	QString text("������!");
	if (0 == response)
		text = "����ʧ��";
	else if (1 == response)
		text = "���óɹ�";
	else
		text = "���ó�ʱ";
	QMessageBox message(QMessageBox::Warning, "��ʾ", text, QMessageBox::Ok, NULL);
	message.exec();

	if (1 == response)
		ui->action_3->setDisabled(false);
	else
		ui->action_2->setDisabled(false);
}
//����Ͷ��
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
	QString text("������!");
	if (0 == response)
		text = "����ʧ��";
	else if (1 == response)
		text = "���óɹ�";
	else
		text = "���ó�ʱ";
	QMessageBox message(QMessageBox::Warning, "��ʾ", text, QMessageBox::Ok, NULL);
	message.exec();
	if (1 == response)
		ui->action_2->setDisabled(false);
	else
		ui->action_3->setDisabled(false);
}

void Lifter_client_mscv::on_action_CAN_triggered()
{

	//QString strCanIp = "192.168.1.118";
	//�ر�
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
			strTest = "��������ʧ��";
		}
		if (-1 == response)
		{
			strTest = "�������ó�ʱ,�����δ����!";
		}

		QMessageBox message(QMessageBox::Warning, "��ʾ", strTest, QMessageBox::Ok, NULL);
		message.exec();
		return;
	}
	/**********��������exe***************/
	QString strPath = qApp->applicationDirPath() + "//config_can.exe";

	QProcess process(this);
	process.execute(strPath);

	/*************************/
	//����
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


// ���� ������� 0 ʧ�� 1�ɹ�
void    Lifter_client_mscv::show_ui_ControlRes(int ret)
{
	if (0 == ret) //ʧ��
	{
		statusBar()->showMessage("����ʧ��,������!", 3000);
	}
	if (1 == ret) //�ɹ�
	{
		statusBar()->showMessage("�����ɹ�!", 3000);
	}
}