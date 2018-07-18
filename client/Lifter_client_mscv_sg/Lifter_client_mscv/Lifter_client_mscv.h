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
�������������
*/
enum CGQ_DATA
{
	cgq_jsd_y, //���ٶ� y��
	cgq_spd_x, //ˮƽ�� x��
	cgq_spd_z, //ˮƽ�� z��
	cgq_max
};

/*
������������ݼ��
*/
enum BMQ_DATA
{
	bmq_fx, //�������з���
	bmq_sd, //�ٶ�
	bmq_wz, //λ��
	bmq_zdjl, //�ƶ�����
	bmq_max
};


/*
��Դ 3�� ��ѹ ��������Ƶ��
*/
enum DYDATA_SHOW
{
	DYDATA_SHOW_DY, //��Դ ��ѹ
	DYDATA_SHOW_DL, //��Դ ����
	DYDATA_SHOW_PL, //��Դ Ƶ��
	DYDATA_SHOW_max
};
enum DYDATA_DYDLPL
{
	DYDATA_DYDLPL_A,  //3���  A��
	DYDATA_DYDLPL_B,  //3���  B��
	DYDATA_DYDLPL_C,  //3���  C��
	DYDATA_DYDLPL_max
};


//���ư�ť ���� �� ������� �ؼ�
typedef struct BtAttribute
{
	bool  m_flag; //�������ı�־ 
	QString  m_strName; //��ť������
	QPushButton* m_bt_renLSB;//����ʶ�� ����
	QPushButton* m_bt_yaoS; //Կ�׿���
	QPushButton* m_bt_qiD; //���� ����
	QPushButton* m_bt_dir; //1.���� ǰ��(�ϡ��� ����) 
	QPushButton* m_bt_jiaS; //���ٰ�ť
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
public:
	/*
	��A
	*/
	QMap<QString, QLabel*> m_button_ID_to_ctl_left; //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*> m_label_ID_to_aqcd_left; //DI�豸DIӳ�� ��ȫ����
	QMap<int, QLabel*>  m_cgq_to_label_left; //��������� label
	QMap<int, QLabel*>  m_bmq_to_label_left; //��������� label
	QTableView*  m_tableView_left;
	QStandardItemModel*     m_yl_tableV_mode_left;
	QLabel*		m_label_dydl_show_left[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_left[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_left_set;//���ð�ť
	/*
	��B
	*/
	QMap<QString, QLabel*> m_button_ID_to_ctl_right; //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*> m_label_ID_to_aqcd_right; //DI�豸DIӳ�� ��ȫ����
	QMap<int, QLabel*>  m_cgq_to_label_right; //��������� label
	QMap<int, QLabel*>  m_bmq_to_label_right; //��������� label
	QTableView* m_tableView_right;
	QStandardItemModel*     m_yl_tableV_mode_right;
	QLabel*		m_label_dydl_show_right[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_right[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_right_set;//���ð�ť
private:
	/*
	��ʼ������
	��������ʾʩ��������2������ ����
	*/
	void InitMainWnd();



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

	
	

	QList<QGraphicsPixmapItem*> m_item_list;

	QThread m_consumerThread; //�������ݵ��߳�
	QThread m_project;
	QThread m_thread_pool[MAX_THREAD_POOL];
	QTimer   m_timer;
	QTimer   m_timer2;
	QTimer   m_timer_pool[MAX_THREAD_POOL];

	/*
	��A ��Ӧ������ ��� ��ʾ
	*/
	QMap<QPushButton*, QString> m_button_ID_left; //��ť ӳ��DO�豸ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_left; //��ť ӳ�䰴ť������

	/***************************************************/

	/*
	��B ��Ӧ������ �ұ� ��ʾ
	*/
	QMap<QPushButton*, QString> m_button_ID_right; //��ť ӳ��DO�豸ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_right; //��ť ӳ�䰴ť������
  /***************************************************/

	//ƽ�㿪�ص�״̬ID ��Ӧ����ѡ�񿪹� ��ť ָ��
	QMap<QString, QPushButton*> m_lifter_floor;


   /*
	* ���� �ı�״̬��button
	*/
	QPushButton* m_changeState_button;
	QTimer m_changeState_timer;
	QTimer m_onlineStatus_timer;
	QString m_baseTitle; //�����ı���

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
	* �������������״̬
	*/
	void ShowOnlineStatus();

	/*
	* �Զ���������
	*/
	void show_ui_button_zdy(void);
	void get_task_result(QStringList  list, int times);
	void show_ui_CgqData(QStringList const& strList
		, QMap<int, QLabel*>& map);
	//������
	void show_ui_BmpData(QString strDir, double iSd, double iWz, double zdjl
		, QMap<int, QLabel*>&); //���� �ٶ� λ��

																			   //�̵���
	void show_ui_JdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>& map_id
		, QMap<QString, QLabel*>& map_id_aqcd);

	//Ӧ��
	void    show_ui_YlData(QString const& strTd, QString const& strData, QStandardItemModel*);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    show_ui_ControlRes(int ret);

	//�Զ��� ��������
	void on_pushButton_zdy_clicked();
	void on_pushButton_sz_clicked(bool checked);

	//��Դ ���ݵ� ��ʾ 
	void show_ui_dyData(int, QString &, QString&, QString&, QLabel***);
	//��Դ ���� ��ѹ ���� Ƶ��
	void on_pushButton_dySet_right(bool flag);
	void on_pushButton_dySet_left(bool flag);
	private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_CAN_triggered();


};
//�������ݻ������е�����,�����µ�������
/*
* ��������� ���ݰ�
*/

class ShowWorker : public QObject
{

	Q_OBJECT

public:
	ShowWorker(Lifter_client_mscv* pMainwindow)
	{
		m_pMainwindow = pMainwindow;
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
	void showCgqData(QStringList const& strList
		, QMap<int, QLabel*>& map);

	//������
	void showBmpData(QString strDir, double iSd, double iWz, double zdjl
		, QMap<int, QLabel*>& map); //���� �ٶ� λ�� �ƶ�����

																		   //�̵���
	void showJdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>& map_id
		, QMap<QString, QLabel*>& map_id_aqcd);

	//Ӧ��
	void    showYlData(QString const& strTd, QString const& strData, QStandardItemModel*);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    showControlRes(int ret);

	//��Դ ������ʾ
	void showdyData(int, QString &, QString&, QString&, QLabel***);
	
};


