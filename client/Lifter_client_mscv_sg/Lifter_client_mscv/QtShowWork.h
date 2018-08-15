#pragma once

#include <QLabel>
#include <QObject>
#include "Lifter_client_mscv.h"



class Lifter_client_mscv;
class QStandardItemModel;
class QtShowWork : public QObject
{
	Q_OBJECT

public:
	QtShowWork(QObject *parent);
	~QtShowWork();
public:
	QtShowWork(Lifter_client_mscv* pMainwindow)
	{
		m_pMainwindow = pMainwindow;
	}
	Lifter_client_mscv* m_pMainwindow; //��ui���ڵ�ָ��
									   //void start(void);

	public slots:

	//!�������� ������ ��
	// void producer(const QString &parameter) ;
	/*
	* �������е����ݰ�
	*/
	void analyseAllData();
	//!�ӻ�����ȡ���� ���� ��ʾ
	//�������� ����ʾ����

	void consumer();

	void analyseBmqData();


Q_SIGNALS:
	//������
	void showCgqData(QStringList const strList
		, QMap<int, QLabel*>* map);

	//������
	void showBmpData(BmqDataS* bmqData
		, QMap<int, QLabel*>* map); //���� �ٶ� λ�� �ƶ�����

									//�̵���
	void showJdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>* map_id
		, QMap<QString, QLabel*>* map_id_aqcd
		, QPushButton*);

	void showJdqDataDO(QMap<QString, int> map
		, QMap<QPushButton*,QString>* map_id, QMap<QPushButton*, BtAttribute*>*);

	//Ӧ��
	void    showYlData(QString const strTd, QString const strData, QStandardItemModel*);

	//���� ������� 0 ʧ�� 1�ɹ�
	void    showControlRes(int ret);

	//��Դ ������ʾ
	void showdyData(int, QString , QString, QString, QLabel***);

	//����
	void showTest(QMap<QString, int> map
		, QMap<QString, QLabel*> map_id
		, QMap<QString, QLabel*> map_id_aqcd);
	void showTest2(
		 QMap<QString, QLabel*>* map_id_aqcd);
	void showTest3(QMap<QString, int>);
	void showTest4();
public:
	void showSignal()
	{
		QStringList strList;
		
		emit showCgqData(strList, &(m_pMainwindow->m_cgq_to_label_left));
		return;
		BmqDataS data;
		data.m_iWz = 1;
		data.m_strDir = "2";
		emit showBmpData(&data, &(m_pMainwindow->m_bmq_to_label_left));
		return;

		QMap<QString, int> m;
		QMap<QString, QLabel*> mi;
		QMap<QString, QLabel*> ma;
		emit showTest(m, mi, ma);

		emit showTest3(m);
		emit showTest4();
		
	}
};
