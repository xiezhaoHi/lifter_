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

//��λ����cm
static double gSizeR = 10;//Ȧ�İ뾶
static double gSizeP = 1000; //���� �̶�
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

	//��ʼ������
	InitMainWnd();

	/*���� �ͻ��� ����*/
	client_manager::GetInstance()->start_server();

	//��ʵ��һ��Ч�� �������Ż�  ���Ż�
	connect(&m_changeState_timer, &QTimer::timeout, this, &Lifter_client_mscv::ChangeButtonState);

	/*****************************************************/
	
	//ui->pushButton_zdy->setStyleSheet(m_button_state_old_style);
	//ui->pushButton_sz->setStyleSheet(m_button_state_old_style);



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
		gSizeR = configd;
	if (0 != configp)
		gSizeP = configp;
	/*****************************************************/
	/*
	Ӧ�� ��ʼ��
	��˿�����ݱ� ��ʾ
	��ʼ���̵���DI DO �豸ӳ��� 
	*/
	 
	//initYlTableView(); //��ʼ��Ӧ�����ݱ�
	//initGssTableView();//��˿�����ݱ���ʾ
	//InitButtonIDMap();//��ʼ���̵���DI DO �豸ӳ���
					 



	//init(); //�����ڽ����ʼ������
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
	connect(&m_timer_pool[0], &QTimer::timeout, worker, &ShowWorker::analyseAllData); //�������ݰ�
	m_timer_pool[0].start(10);
	m_thread_pool[0].start();


	/*�Զ�������
	*/
	connect(&m_taskThread, &TaskThread::show_button_zdy, this, &Lifter_client_mscv::show_ui_button_zdy);

	/*��������*/
	//connect(&client_manager::GetInstance())


	//��ʼ�� ������ʾ,��ͬ������� ��ʾ��ͬ�Ŀؼ�
	InitLifterList();

	/*
	* ��ʼ ���� ����Ͷ�� �˵�
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
		map[cgq_spd_x]->setText(QString("%1��").arg(strList[3]));
	}
	if (map.contains(cgq_spd_z) && nullptr != map[cgq_spd_z])
	{
		map[cgq_spd_z]->setText(QString("%1��").arg(strList[4]));
	}
	
	//ui->label_spd_z->setText(strList[5]);

}

//������  �ٶ� λ�� �ƶ�����
void Lifter_client_mscv::show_ui_BmpData(QString strDir, double iSd, double iWz
	, double zdjl, QMap<int, QLabel*>& map)//���� �ٶ� λ��
{
	map[bmq_fx]->setText(strDir);
	map[bmq_sd]->setText(QString::number(iSd, 'f', 2) + "cm/s");
	map[bmq_wz]->setText(QString::number(iWz, 'f', 2) + "cm");
	if (zdjl != 0)
		map[bmq_zdjl]->setText(QString::number(zdjl, 'f', 2) + "cm");

}

//�̵��� ���� ���� ��ʾ
/*
map �̵���ֵ key Ϊ DI ��ʶ 2001...  value  Ϊ 0 �� 1
map_id  ��ͨ�̵��� DI ��ʶ 2001...��Ӧ label
map_id_aqcd ��ȫ���� DI ��ʶ ��Ӧ  label
*/
void Lifter_client_mscv::show_ui_JdqData(QMap<QString, int> map
	, QMap<QString, QLabel*>& map_id, QMap<QString, QLabel*>& map_id_aqcd)
{
	QMap<QString, int>::iterator begin = map.begin(), end = map.end();
	int iValue = -1;
	QString strKey;
	for (; begin != end; ++begin)
	{
		strKey = begin.key(); //DI ��ʶ 2001...
		iValue = begin.value(); //״̬ 0 ��   1 ��
		// if(begin.key() < QString("20010021")) //��ʾ���ǰ�ȫ����
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

				//20010021 - 20010026 ��ʾƽ�㿪��
				if (m_lifter_floor.contains(strKey) && 1 == iValue )
				{
					m_lifter_floor[strKey]->setStyleSheet(m_button_state_old_style);
				}


			}

		}
		//��ȫ����
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
//Ӧ��
void    Lifter_client_mscv::show_ui_YlData(QString const& strTd, QString const& strData, QStandardItemModel* pModel)
{
	// m_yl_tableV_mode->setItem(0,2,new QStandardItem(QString("ͨ��-1")));
	if (nullptr != pModel)
	{
		pModel->setItem(strTd.toInt(), 2, new QStandardItem(strData));
		return;
	}
	qDebug() << "[show_ui_YlData] # Ӧ����ʾ ����,����Ĳ�����Ч!";
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
		QString strBelongs = root.attribute(QString("belongs")); //������
		/*
		��������
		0001...(ҷ��ʽ����������)
		0002...(ǿ��ʽ����������)
		0003...(sc˫��ʩ��������)
		*/
		QMap<QString, QLabel*>* map_id = nullptr/*��ͨ������*/
		, *map_id_aqcd = nullptr;/*��ȫ���㿪����*/;
		QMap<int, QLabel*> *map_cgq = nullptr; /*������ �������� ��Ӧ �ؼ�*/;
		QMap<int, QLabel*> *map_bmq = nullptr; /*������ �������� ��Ӧ �ؼ�*/;
		QStandardItemModel* pModel = nullptr;
		
		if ("1" == strBelongs.right(1)) //��ʾ ��A
		{
			map_id = &(m_pMainwindow->m_button_ID_to_ctl_left);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_left);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_left); 
			map_bmq = &(m_pMainwindow->m_bmq_to_label_left);
			pModel = m_pMainwindow->m_yl_tableV_mode_left;
		}
		if ("2" == strBelongs.right(1))//��ʾ ��B
		{
			map_id = &(m_pMainwindow->m_button_ID_to_ctl_right);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_right);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_right);
			map_bmq = &(m_pMainwindow->m_bmq_to_label_right);
			pModel = m_pMainwindow->m_yl_tableV_mode_right;
		}
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

					emit showCgqData(strList, *map_cgq);
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
					emit showJdqData(device_map, *map_id, *map_id_aqcd);
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
						emit showYlData(root.attribute(QString("ID")), root.text(), pModel);
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
					dZs = dZs*gSizeR * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //��jsz�ӽڵ� ����ֵ
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					dWz = dWz * 2 * 3.14*gSizeR / (2 * gSizeP);
				}
				root = rootFist.firstChildElement(QString("zdjl")); //��zdjl�ӽڵ� �ƶ�����

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}

				emit showBmpData(strDir, dZs, dWz, dZdjl, *map_bmq);
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
				emit showJdqData(device_map, *map_id, *map_id_aqcd);
				
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

		
	}
	

	//��ť�ͷ�
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
			statusBar()->showMessage("���Ϳ�������ɹ�,�ȴ��ظ�!", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.
		else
			statusBar()->showMessage("���Ϳ�������ʧ��,�������!", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.

	}

	//˫�� ѡ��ťʱ ȡ�� ѡ��
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
* ����������
*/
void  Lifter_client_mscv::SetBmqConfig(double d, double p, QString const& strLifter, QString const& strBmqID)
{
	if (strLifter.isEmpty())
		return;

	m_choose_lifter = strLifter; //����ѡ��ĵ���ID
	gSizeR = d;
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
	}



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



/*
��ʼ������
��������ʾʩ��������2������ ����
*/
void Lifter_client_mscv::InitMainWnd()
{
	int iWidth = 1366, iHeight = 768;
	QWidget* pCenterWnd = new QWidget;
	pCenterWnd->setMinimumSize(iWidth, iHeight);
	setCentralWidget(pCenterWnd);
	QGroupBox* pGbLeftA = new QGroupBox();
	pGbLeftA->setTitle("  ��A  ");
	//pGbLeftA->setMinimumWidth(iWidth / 2);
	pGbLeftA->setMinimumHeight(iHeight / 2);

	QGroupBox* pGbRightB = new QGroupBox();
	pGbRightB->setTitle("  ��B  ");
	pGbRightB->setMinimumHeight(iHeight / 2);
/*
�����A�����ʼ��
*/
	//1.1Զ�̿���
#define  UIYCKZ_LEFT
#ifdef UIYCKZ_LEFT
	QGroupBox* pGbLeftYckz_left = new QGroupBox; //Զ�̿���
	pGbLeftYckz_left->setTitle("Զ�̿���");
	QPushButton* pButtonSx_left = new QPushButton("����");
	QPushButton* pButtonXx_left = new QPushButton("����");
	QPushButton* pButtonJx_left = new QPushButton("����");
	QPushButton* pButtonZd_left = new QPushButton("�ƶ�");
	QPushButton* pButtonJt_left = new QPushButton("��ͣ");

	QPushButton* pButtonCzbh_left = new QPushButton("���ر���");
	QPushButton* pButtonSxw_left = new QPushButton("����λ");
	QPushButton* pButtonXxw_left = new QPushButton("����λ");
	QPushButton* pButtonSjx_left = new QPushButton("�ϼ���");
	QPushButton* pButtonXjx_left = new QPushButton("�¼���");
	QPushButton* pButtonJtkg_left = new QPushButton("��ͣ����");


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

	//1.1.1���ÿؼ�������
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

	//1.1.2 Զ�̿��� ģ�� ���񲼾�
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
	//1.2 ����״̬
#define  UISDZZJK_LEFT
#ifdef UISDZZJK_LEFT
	//1.2.1 �����ؼ�
	QGroupBox* pGbLeftYxzt_left = new QGroupBox; //����״̬
	pGbLeftYxzt_left->setTitle("����״̬");

	QLabel* pLabelSx_text_left = new QLabel("����");
	QLabel* pLabelXx_text_left = new QLabel("����");
	QLabel* pLabelJx_text_left = new QLabel("����");
	QLabel* pLabelZd_text_left = new QLabel("�ƶ�");
	QLabel* pLabelJt_text_left = new QLabel("��ͣ");
	QLabel* pLabelCzbh_text_left = new QLabel("���ر���");
	QLabel* pLabelSxw_text_left = new QLabel("����λ");
	QLabel* pLabelXxw_text_left = new QLabel("����λ");
	QLabel* pLabelSjx_text_left = new QLabel("�ϼ���");
	QLabel* pLabelXjx_text_left = new QLabel("�¼���");
	QLabel* pLabelJtkg_text_left = new QLabel("��ͣ����");

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

	//1.2.2 �洢 DI ��Ӧ�� ��ͨ����״̬ �ؼ�����
	m_button_ID_to_ctl_left[QString("20010001")] = pLabelSx_left; //����
	m_button_ID_to_ctl_left[QString("20010002")] = pLabelXx_left; //����
	m_button_ID_to_ctl_left[QString("20010003")] = pLabelZd_left; //�ƶ�
	m_button_ID_to_ctl_left[QString("20010006")] = pLabelJt_left; //��ͣ
	m_button_ID_to_ctl_left[QString("20010007")] = pLabelJx_left; //����

	//��ȫ���� ����
	m_label_ID_to_aqcd_left[QString("20010008")] = pLabelCzbh_left;
	m_label_ID_to_aqcd_left[QString("20010009")] = pLabelSxw_left;
	m_label_ID_to_aqcd_left[QString("20010010")] = pLabelXxw_left;
	m_label_ID_to_aqcd_left[QString("20010011")] = pLabelSjx_left;
	m_label_ID_to_aqcd_left[QString("20010012")] = pLabelXjx_left;
	m_label_ID_to_aqcd_left[QString("20010013")] = pLabelJtkg_left;

	//1.2.3 ���ÿ���label ������źŵ�
	QMap<QString, QLabel*>::iterator beginID_left = m_button_ID_to_ctl_left.begin()
		, endID_left = m_button_ID_to_ctl_left.end(); //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*>::iterator beginaqcd_left = m_label_ID_to_aqcd_left.begin()
		, endaqcd_left = m_label_ID_to_aqcd_left.end(); //DI�豸DIӳ�� ��ȫ����

	for (; beginID_left != endID_left; ++beginID_left)
		beginID_left.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd_left != endaqcd_left; ++beginaqcd_left)
		beginaqcd_left.value()->setPixmap(m_label_bitmap_1);

	//ʵʱλ�� ʵʱ�ٶ�
// 	QLabel* pLabelWz_text_left = new QLabel("ʵʱλ��");
// 	QLabel* pLabelSd_text_left = new QLabel("ʵʱ�ٶ�");
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
	
	//1.3 ��ѹ ���� Ƶ�� ���״̬
#define  UIDYDL_LEFT
#ifdef  UIDYDL_LEFT
	QGroupBox* pGbLeftDydljczt_left = new QGroupBox; //��ѹ ���� Ƶ�� ���״̬
	pGbLeftDydljczt_left->setTitle("��ѹ ���� Ƶ�� ���״̬");

	/*ͼ����ʾ*/
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
	/*˵��*/
	//��ѹ˵��
	QLabel* pLabelDySm_left = new QLabel("�ֱ���0.1v������0.2%rdg+0.2%FS");
	//����˵��
	QLabel* pLabelDlSm_left = new QLabel("�ֱ���0.1A/1A������0.3%rdg+0.3%FS");

	//Ƶ��˵��
	QLabel* pLabelPlSm_left = new QLabel("�ֱ���0.1Hz������0.05%");

	QLabel* pLabelDy_text_left = new QLabel("��ѹ:");
	QLabel* pLabelDl_text_left = new QLabel("����:");
	QLabel* pLabelPl_text_left = new QLabel("Ƶ��:");
	//��ѹ
	QLineEdit* pLabelDy_left = new QLineEdit();
	//����
	QLineEdit* pLabelDl_left = new QLineEdit();

	//Ƶ��
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
	//1.4 Ӧ�� ���  ������ ���
#define  UIYLJC_LEFT
#ifdef UIYLJC_LEFT
	QGroupBox* pGbLeftYljc_left = new QGroupBox;//Ӧ�� ���
	pGbLeftYljc_left->setTitle("Ӧ�� ���");
	QTableView* pTableView_left = new QTableView();

	//������a Ӧ������
	m_tableView_left = pTableView_left;

	m_yl_tableV_mode_left = new QStandardItemModel();
	m_yl_tableV_mode_left->setColumnCount(4);
	m_yl_tableV_mode_left->setHeaderData(0, Qt::Horizontal, QString("��ⲿλ"));
	m_yl_tableV_mode_left->setHeaderData(1, Qt::Horizontal, QString("����ֵ"));
	m_yl_tableV_mode_left->setHeaderData(2, Qt::Horizontal, QString("ʵ��ֵ"));
	m_yl_tableV_mode_left->setHeaderData(3, Qt::Horizontal, QString("״̬"));
	pTableView_left->setModel(m_yl_tableV_mode_left);
	pTableView_left->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ɱ༭
																	//��ͷ��Ϣ��ʾ����
	pTableView_left->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//�����п��ɱ�
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
		m_yl_tableV_mode_left->setItem(i, 0, new QStandardItem(QString("ͨ��%1").arg(i + 1)));
		//�����ַ���ɫ
		m_yl_tableV_mode_left->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//�����ַ�λ��
		m_yl_tableV_mode_left->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode_left->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_left->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_left->setItem(i, 3, new QStandardItem(QString("xx")));
	}
	/**********************************************/
	//�������������
	QLabel* pLabelSpdx_text_left = new QLabel("x��ˮƽ��");
	QLabel* pLabelSpdz_text_left = new QLabel("z��ˮƽ��");
	QLabel* pLabelJsdy_text_left = new QLabel("y����ٶ�");


	QLabel* pLabelSpdx_left = new QLabel();
	QLabel* pLabelSpdz_left = new QLabel();
	QLabel* pLabelJsdy_left = new QLabel();

	m_cgq_to_label_left[cgq_spd_x] = pLabelSpdx_left;
	m_cgq_to_label_left[cgq_spd_z] = pLabelSpdz_left;
	m_cgq_to_label_left[cgq_jsd_y] = pLabelJsdy_left;
	/**********************************************/

	/**********************************************/
	// ��������ؼ�� ����
	QLabel* pLabelFx_text_left = new QLabel("�������з���");
	
	QLabel* pLabelZdjl_text_left = new QLabel("�ƶ�����");
	QLabel* pLabelWz_text_left = new QLabel("λ��");
	QLabel* pLabelJsz_text_left = new QLabel("�ٶ�");

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
	//������
	pLayoutYl_left->addWidget(pTableView_left,0,0, iRow_left,4);
	pLayoutYl_left->addWidget(pLabelSpdx_text_left, iRow_left +1, 0);
	pLayoutYl_left->addWidget(pLabelSpdx_left, iRow_left +1, 1);
	pLayoutYl_left->addWidget(pLabelSpdz_text_left, iRow_left +2, 0);
	pLayoutYl_left->addWidget(pLabelSpdz_left, iRow_left +2, 1);
	pLayoutYl_left->addWidget(pLabelJsdy_text_left, iRow_left +3, 0);
	pLayoutYl_left->addWidget(pLabelJsdy_left, iRow_left +3, 1);

	//������
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
	//��� ���岼��
	QGridLayout* pGridLeft_left = new QGridLayout;
	pGridLeft_left->addWidget(pGbLeftYckz_left, 0, 0);
	pGridLeft_left->addWidget(pGbLeftYxzt_left, 1, 0);
	pGridLeft_left->addWidget(pGbLeftDydljczt_left, 2, 0);
	pGridLeft_left->addWidget(pGbLeftYljc_left, 0, 1, 3, 1);

	//���� �б�
	pGridLeft_left->setRowStretch(0, 1);
	pGridLeft_left->setRowStretch(1, 1);
	pGridLeft_left->setRowStretch(2, 1);

	pGridLeft_left->setColumnStretch(0, 1);
	pGridLeft_left->setColumnStretch(1, 3);

	pGbLeftA->setLayout(pGridLeft_left);



/*
�ұ� ��B ���ڽ����ʼ��
*/
//2.1Զ�̿���
#define  UIYCKZ_RIGHT
#ifdef UIYCKZ_RIGHT
	QGroupBox* pGbLeftYckz_right = new QGroupBox; //Զ�̿���
	pGbLeftYckz_right->setTitle("Զ�̿���");
	QPushButton* pButtonSx_right = new QPushButton("����");
	QPushButton* pButtonXx_right = new QPushButton("����");
	QPushButton* pButtonJx_right = new QPushButton("����");
	QPushButton* pButtonZd_right = new QPushButton("�ƶ�");
	QPushButton* pButtonJt_right = new QPushButton("��ͣ");

	QPushButton* pButtonCzbh_right = new QPushButton("���ر���");
	QPushButton* pButtonSxw_right = new QPushButton("����λ");
	QPushButton* pButtonXxw_right = new QPushButton("����λ");
	QPushButton* pButtonSjx_right = new QPushButton("�ϼ���");
	QPushButton* pButtonXjx_right = new QPushButton("�¼���");
	QPushButton* pButtonJtkg_right = new QPushButton("��ͣ����");


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

	//2.1.1���ÿؼ�������
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

	//2.1.2 Զ�̿��� ģ�� ���񲼾�
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
	//2.2 ����״̬
#define  UISDZZJK_RIGHT
#ifdef UISDZZJK_RIGHT
	//2.2.1 �����ؼ�
	QGroupBox* pGbLeftYxzt_right = new QGroupBox; //����״̬
	pGbLeftYxzt_right->setTitle("����״̬");

	QLabel* pLabelSx_text_right = new QLabel("����");
	QLabel* pLabelXx_text_right = new QLabel("����");
	QLabel* pLabelJx_text_right = new QLabel("����");
	QLabel* pLabelZd_text_right = new QLabel("�ƶ�");
	QLabel* pLabelJt_text_right = new QLabel("��ͣ");
	QLabel* pLabelCzbh_text_right = new QLabel("���ر���");
	QLabel* pLabelSxw_text_right = new QLabel("����λ");
	QLabel* pLabelXxw_text_right = new QLabel("����λ");
	QLabel* pLabelSjx_text_right = new QLabel("�ϼ���");
	QLabel* pLabelXjx_text_right = new QLabel("�¼���");
	QLabel* pLabelJtkg_text_right = new QLabel("��ͣ����");

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

	//2.2.2 �洢 DI ��Ӧ�� ��ͨ����״̬ �ؼ�����
	m_button_ID_to_ctl_right[QString("20010001")] = pLabelSx_right; //����
	m_button_ID_to_ctl_right[QString("20010002")] = pLabelXx_right; //����
	m_button_ID_to_ctl_right[QString("20010003")] = pLabelZd_right; //�ƶ�
	m_button_ID_to_ctl_right[QString("20010006")] = pLabelJt_right; //��ͣ
	m_button_ID_to_ctl_right[QString("20010007")] = pLabelJx_right; //����

																	//��ȫ���� ����
	m_label_ID_to_aqcd_right[QString("20010008")] = pLabelCzbh_right;
	m_label_ID_to_aqcd_right[QString("20010009")] = pLabelSxw_right;
	m_label_ID_to_aqcd_right[QString("20010010")] = pLabelXxw_right;
	m_label_ID_to_aqcd_right[QString("20010011")] = pLabelSjx_right;
	m_label_ID_to_aqcd_right[QString("20010012")] = pLabelXjx_right;
	m_label_ID_to_aqcd_right[QString("20010013")] = pLabelJtkg_right;

	//2.2.3 ���ÿ���label ������źŵ�
	QMap<QString, QLabel*>::iterator beginID_right = m_button_ID_to_ctl_right.begin()
		, endID_right = m_button_ID_to_ctl_right.end(); //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*>::iterator beginaqcd_right = m_label_ID_to_aqcd_right.begin()
		, endaqcd_right = m_label_ID_to_aqcd_right.end(); //DI�豸DIӳ�� ��ȫ����

	for (; beginID_right != endID_right; ++beginID_right)
		beginID_right.value()->setPixmap(m_label_bitmap_1);
	for (; beginaqcd_right != endaqcd_right; ++beginaqcd_right)
		beginaqcd_right.value()->setPixmap(m_label_bitmap_1);

	//ʵʱλ�� ʵʱ�ٶ�
// 	QLabel* pLabelWz_text_right = new QLabel("ʵʱλ��");
// 	QLabel* pLabelSd_text_right = new QLabel("ʵʱ�ٶ�");
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

	//2.3 ��ѹ ���� Ƶ�� ���״̬
#define  UIDYDL_RIGHT
#ifdef  UIDYDL_RIGHT
	QGroupBox* pGbLeftDydljczt_right = new QGroupBox; //��ѹ ���� Ƶ�� ���״̬
	pGbLeftDydljczt_right->setTitle("��ѹ ���� Ƶ�� ���״̬");

	/*ͼ����ʾ*/
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
	/*˵��*/
	//��ѹ˵��
	QLabel* pLabelDySm_right = new QLabel("�ֱ���0.1v������0.2%rdg+0.2%FS");
	//����˵��
	QLabel* pLabelDlSm_right = new QLabel("�ֱ���0.1A/1A������0.3%rdg+0.3%FS");

	//Ƶ��˵��
	QLabel* pLabelPlSm_right = new QLabel("�ֱ���0.1Hz������0.05%");

	QLabel* pLabelDy_text_right = new QLabel("��ѹ:");
	QLabel* pLabelDl_text_right = new QLabel("����:");
	QLabel* pLabelPl_text_right = new QLabel("Ƶ��:");
	//��ѹ
	QLineEdit* pLabelDy_right = new QLineEdit();
	//����
	QLineEdit* pLabelDl_right = new QLineEdit();

	//Ƶ��
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
	//2.4 Ӧ�� ���  ������ ���
#define  UIYLJC_RIGHT
#ifdef UIYLJC_RIGHT
	QGroupBox* pGbLeftYljc_right = new QGroupBox;//Ӧ�� ���
	pGbLeftYljc_right->setTitle("Ӧ�� ���");
	QTableView* pTableView_right = new QTableView();
	//���� ��B Ӧ���ؼ�����
	m_tableView_right = pTableView_right;

	m_yl_tableV_mode_right = new QStandardItemModel();
	m_yl_tableV_mode_right->setColumnCount(4);
	m_yl_tableV_mode_right->setHeaderData(0, Qt::Horizontal, QString("��ⲿλ"));
	m_yl_tableV_mode_right->setHeaderData(1, Qt::Horizontal, QString("����ֵ"));
	m_yl_tableV_mode_right->setHeaderData(2, Qt::Horizontal, QString("ʵ��ֵ"));
	m_yl_tableV_mode_right->setHeaderData(3, Qt::Horizontal, QString("״̬"));
	pTableView_right->setModel(m_yl_tableV_mode_right);
	pTableView_right->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ɱ༭
																		  //��ͷ��Ϣ��ʾ����
	pTableView_right->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//�����п��ɱ�
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
			, new QStandardItem(QString("ͨ��%1").arg(i + 1)));
		//�����ַ���ɫ
		m_yl_tableV_mode_right->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//�����ַ�λ��
		m_yl_tableV_mode_right->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		m_yl_tableV_mode_right->setItem(i, 1, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_right->setItem(i, 2, new QStandardItem(QString("xx")));
		m_yl_tableV_mode_right->setItem(i, 3, new QStandardItem(QString("xx")));
	}

	/**********************************************/
	// ��������� �������
	QLabel* pLabelSpdx_text_right = new QLabel("x��ˮƽ��");
	QLabel* pLabelSpdz_text_right = new QLabel("z��ˮƽ��");
	QLabel* pLabelJsdy_text_right = new QLabel("y����ٶ�");


	QLabel* pLabelSpdx_right = new QLabel();
	QLabel* pLabelSpdz_right = new QLabel();
	QLabel* pLabelJsdy_right = new QLabel();

	m_cgq_to_label_right[cgq_spd_x] = pLabelSpdx_right;
	m_cgq_to_label_right[cgq_spd_z] = pLabelSpdz_right;
	m_cgq_to_label_right[cgq_jsd_y] = pLabelJsdy_right;
	/**********************************************/

	/**********************************************/
	// ��������ؼ�� ����
	QLabel* pLabelFx_text_right = new QLabel("�������з���");
	QLabel* pLabelJsz_text_right = new QLabel("�ٶ�");
	QLabel* pLabelZdjl_text_right = new QLabel("�ƶ�����");
	QLabel* pLabelWz_text_right = new QLabel("λ��");

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
	//������
	pLayoutYl_right->addWidget(pLabelSpdx_text_right, iRow_right + 1, 0);
	pLayoutYl_right->addWidget(pLabelSpdx_right, iRow_right + 1, 1);
	pLayoutYl_right->addWidget(pLabelSpdz_text_right, iRow_right + 2, 0);
	pLayoutYl_right->addWidget(pLabelSpdz_right, iRow_right + 2, 1);
	pLayoutYl_right->addWidget(pLabelJsdy_text_right, iRow_right + 3, 0);
	pLayoutYl_right->addWidget(pLabelJsdy_right, iRow_right + 3, 1);
	//������
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
	//��� ���岼��
	QGridLayout* pGridLeft_right = new QGridLayout;
	pGridLeft_right->addWidget(pGbLeftYckz_right, 0, 0);
	pGridLeft_right->addWidget(pGbLeftYxzt_right, 1, 0);
	pGridLeft_right->addWidget(pGbLeftDydljczt_right, 2, 0);
	pGridLeft_right->addWidget(pGbLeftYljc_right, 0, 1, 3, 1);

	//���� �б�
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