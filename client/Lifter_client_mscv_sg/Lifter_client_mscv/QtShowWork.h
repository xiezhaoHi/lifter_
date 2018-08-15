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
	Lifter_client_mscv* m_pMainwindow; //主ui窗口的指针
									   //void start(void);

	public slots:

	//!把数据望 缓冲区 存
	// void producer(const QString &parameter) ;
	/*
	* 解析所有的数据包
	*/
	void analyseAllData();
	//!从缓冲区取数据 计算 显示
	//解析数据 并显示数据

	void consumer();

	void analyseBmqData();


Q_SIGNALS:
	//传感器
	void showCgqData(QStringList const strList
		, QMap<int, QLabel*>* map);

	//编码器
	void showBmpData(BmqDataS* bmqData
		, QMap<int, QLabel*>* map); //方向 速度 位置 制动距离

									//继电器
	void showJdqData(QMap<QString, int> map
		, QMap<QString, QLabel*>* map_id
		, QMap<QString, QLabel*>* map_id_aqcd
		, QPushButton*);

	void showJdqDataDO(QMap<QString, int> map
		, QMap<QPushButton*,QString>* map_id, QMap<QPushButton*, BtAttribute*>*);

	//应力
	void    showYlData(QString const strTd, QString const strData, QStandardItemModel*);

	//返回 操作结果 0 失败 1成功
	void    showControlRes(int ret);

	//电源 数据显示
	void showdyData(int, QString , QString, QString, QLabel***);

	//测试
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
