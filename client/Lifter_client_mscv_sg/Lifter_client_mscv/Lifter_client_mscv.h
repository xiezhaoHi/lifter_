#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lifter_client_mscv.h"

#include<QList>
#include<QGraphicsPixmapItem>
#include<QThread>
#include<QTimer>
#include"client_manager.h"
#include<QStandardItemModel>
#include<QDebug>

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

//����������
typedef struct BmqDataS
{
	QString	m_strDir; //���з���
	double	m_iSd;  //�ٶ�
	double	m_iWz;  //λ��-����������ֵ
	double	m_zdjl; //�ƶ�����
}BmqDataS;

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
	QMap<QString, QLabel*> m_label_ID_to_ctl_left; //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*> m_label_ID_to_aqcd_left; //DI�豸DIӳ�� ��ȫ����
	QMap<int, QLabel*>  m_cgq_to_label_left; //��������� label
	QMap<int, QLabel*>  m_bmq_to_label_left; //��������� label
	QTableView*  m_tableView_left;
	QStandardItemModel*     m_yl_tableV_mode_left;
	QLabel*		m_label_dydl_show_left[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_left[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_left_set;//���ð�ť

	QMap<QPushButton*, QString> m_button_ID_left; //��ť ӳ��DO�豸ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_left; //��ť ӳ�䰴ť������
	QMap<QString, QPushButton*> m_ID_button_left; //DO�豸ID ӳ�� ��ť

	BmqDataS m_dataBmq_left; //����bmq���������
	/*
	��B
	*/
	QMap<QString, QLabel*> m_label_ID_to_ctl_right; //DI�豸ID ӳ�� ��ť
	QMap<QString, QLabel*> m_label_ID_to_aqcd_right; //DI�豸DIӳ�� ��ȫ����
	QMap<int, QLabel*>  m_cgq_to_label_right; //��������� label
	QMap<int, QLabel*>  m_bmq_to_label_right; //��������� label
	QTableView* m_tableView_right;
	QStandardItemModel*     m_yl_tableV_mode_right;
	QLabel*		m_label_dydl_show_right[DYDATA_SHOW_max][DYDATA_DYDLPL_max];
	QLineEdit*	m_edit_dydl_set_right[DYDATA_DYDLPL_max];
	QPushButton* m_pButtonSet_right_set;//���ð�ť

	QMap<QPushButton*, QString> m_button_ID_right; //��ť ӳ��DO�豸ID
	QMap<QPushButton*, BtAttribute*> m_BtAttribute_right; //��ť ӳ�䰴ť������
	QMap<QString, QPushButton*> m_ID_button_right; //DO�豸ID ӳ�� ��ť
	BmqDataS m_dataBmq_right; //����bmq���������													 
private:
	/*
	��ʼ������
	��������ʾʩ��������2������ ����
	*/
	void InitMainWnd();

	/*
	��ʼ�����ص�״̬
	*/
	void InitWndStatues();

	/*
	��ֹ����:������ �й�����Ա����ʱ,��ֹ�������
	bool:true ���Բ��� false ��ֹ����
	*/
	void OperationNot(BOOL);

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



	//ƽ�㿪�ص�״̬ID ��Ӧ����ѡ�񿪹� ��ť ָ��
	QMap<QString, QPushButton*> m_lifter_floor;


	/*
	 * ���� �ı�״̬��button
	 */
	QPushButton* m_changeState_button;
	QTimer m_changeState_timer;
	QTimer m_onlineStatus_timer;
	QTimer m_jdqDOStatus_timer; //�̵���DO״̬  ģ�鷵�ص�ǰ�̵���״̬���ݣ�
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

	/*
	����:����˳�.����Ӧ��������.
	1.�̵��� ���� ��λ
	*/
	void ClearWork();

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
	* �̵���-ģ�鷵�ص�ǰ�̵���״̬����
	*/
	void ChangeButtonStateJDQ();

	/*
	* �Զ���������
	*/
	void show_ui_button_zdy(void);
	void get_task_result(QStringList  , int );
	void show_ui_CgqData(QStringList const 
		, QMap<int, QLabel*> );
	//������
	void show_ui_BmpData(BmqDataS* bmqData
		, QMap<int, QLabel*>*); //���� �ٶ� λ��

																			   //�̵���
	void show_ui_JdqData(QMap<QString, int> 
		, QMap<QString, QLabel*>* 
		, QMap<QString, QLabel*>* 
		, QPushButton*);

	void show_ui_JdqDataDO(QMap<QString, int> 
		, QMap<QPushButton*,QString>* 
	, QMap<QPushButton*, BtAttribute*>*);
	
	//Ӧ��
	void    show_ui_YlData(QString const , QString const , QStandardItemModel*);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    show_ui_ControlRes(int );

	//�Զ��� ��������
	void on_pushButton_zdy_clicked();

	//��Դ ���ݵ� ��ʾ 
	void show_ui_dyData(int, QString , QString, QString, QLabel***);
	//��Դ ���� ��ѹ ���� Ƶ��
	void on_pushButton_dySet_right(bool );
	void on_pushButton_dySet_left(bool );
	private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_CAN_triggered();

	//����
	void show_ui_test(QMap<QString, int> map
		, QMap<QString, QLabel*> map_id
		, QMap<QString, QLabel*> map_id_aqcd)
	{
		qDebug() << "test";
	}
	void show_ui_test2(QMap<QString, QLabel*>* map_id_aqcd)
	{
		qDebug() << "test2";
	}
	void show_ui_test3(QMap<QString, int> map)
	{
		qDebug() << "test3";
	}
	void show_ui_test4()
	{
		qDebug() << "test4";
	}
};
