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
* �Զ������� �����߳�
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
	* ѡ��ĵ���
	* ���ݵ�ID
	*/
	QString       m_choose_lifter;

	/*
	* �Զ��崦���߳�
	*/
	TaskThread      m_taskThread;

	
	QStandardItemModel*     m_gss_tableV_mode;


	QStandardItemModel*     m_yl_tableV_mode;

	QList<QGraphicsPixmapItem*> m_item_list;

	QThread m_consumerThread; //�������ݵ��߳�
	QThread m_project;
	QThread m_thread_pool[MAX_THREAD_POOL];
	QTimer   m_timer;
	QTimer   m_timer2;
	QTimer   m_timer_pool[MAX_THREAD_POOL];

	QMap<QString, int> m_device_map; //�豸ID��Ӧ�Ŀؼ���
	QMap<int, QString> m_control_map; //�ؼ����豸ID
	QMap<QPushButton*, QString> m_button_ID; //��ť ӳ��DO�豸ID
	QMap<QPushButton*, int>      m_button_keep_state; //����һЩ����İ�ť �����ť�� ״̬�ı� һֱ���� ֱ�� ����
	QMap<QString, QLabel*> m_button_ID_to_ctl; //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*> m_label_ID_to_aqcd; //DI�豸DIӳ�� ��ȫ����
	
	//ƽ�㿪�ص�״̬ID ��Ӧ����ѡ�񿪹� ��ť ָ��
	QMap<QString, QPushButton*> m_lifter_floor;


   /*
	* ���� �ı�״̬��button
	*/
	QPushButton* m_changeState_button;
	QTimer m_changeState_timer;

	//�������button �¾ɷ��
	QString m_button_state_old_style; 
	QString m_button_state_new_style;

	// gray   �źŵ� ����״̬��ɫ
	// red
	// green
	QString m_label_bitmap_1; 
	QString m_label_bitmap_2; 
	QString m_label_bitmap_3; 

	//˫�� ����ѡ��ť
	QAction*  m_choose_one;
	QAction*  m_choose_two;

	QStringList     m_lifter_list;

	/***********************************************************/

	/*
	* �����ļ��еĵ���ѡ��
	* ���� �����ļ��� ���г����ĵ��� ID
	*/

	/*
	* ��ʼ�� m_lifter_list
	*/
	void    InitLifterList(void);

	//��ʼ����������map
	void  InitButtonIDMap(void); 

	// ��һЩ����ĳ�ʼ������
	void init(void);
	
					//Ӧ��tableView
	void  initYlTableView();

	//��˿��tableView
	void  initGssTableView();
	public slots:

	/*
	* ����������
	*/
	void  SetBmqConfig(double d, double p, const QString &strLifter, const QString &strBmqID);

	/*
	* ���ô�������
	*/
	void  SetTitle(QString const& strName);

	/*
	* ���ư�ť ��� �ı� ����ɫ
	*/
	void ChangeButtonState();

	/*
	* �Զ���������
	*/
	void show_ui_button_zdy(void);
	void get_task_result(QStringList  list, int times);
	void show_ui_CgqData(QStringList const& strList);
	//������
	void show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl); //���� �ٶ� λ��

																			   //�̵���
	void show_ui_JdqData(QMap<QString, int> map);

	//Ӧ��
	void    show_ui_YlData(QString const& strTd, QString const& strData);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    show_ui_ControlRes(int ret);

	//�Զ��� ��������
	void on_pushButton_zdy_clicked();
	void on_pushButton_sz_clicked(bool checked);

	//˫�� ���� ���������л� �˵�
	void create_choose_acction();
	private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_CAN_triggered();
	void on_choose_one();
	void on_choose_two();
};
//�������ݻ������е�����,�����µ�������
/*
* ��������� ���ݰ�
*/

class ShowWorker : public QObject
{

	Q_OBJECT

public:
	ShowWorker()
	{

	}

	Lifter_client_mscv* m_pMainwindow; //��ui���ڵ�ָ��
	void start(void);


private:



public slots :

		//!�������� ������ ��
		// void producer(const QString &parameter) ;

		//!�ӻ�����ȡ���� ���� ��ʾ
		//�������� ����ʾ����
		void consumer();

	void analyseBmqData();

	/*
	* �������е����ݰ�
	*/
	void analyseAllData();
signals:

	//������
	void showCgqData(QStringList const& strList);

	//������
	void showBmpData(QString strDir, double iSd, double iWz, double zdjl); //���� �ٶ� λ�� �ƶ�����

																		   //�̵���
	void showJdqData(QMap<QString, int> map);

	//Ӧ��
	void    showYlData(QString const& strTd, QString const& strData);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    showControlRes(int ret);
};