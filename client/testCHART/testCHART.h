#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testCHART.h"
#include "qcustomplot.h"
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>
class testCHART : public QMainWindow
{
	Q_OBJECT

public:

	void setupRealtimeDataDemo(QCustomPlot *customPlot);

	QCPGraph *pGraph;
	QSerialPort *my_serialport;
	QComboBox* pBox;
	
	QCustomPlot *pCustomPlot;
	testCHART(QWidget *parent = Q_NULLPTR);
	//void setupRealtimeDataDemo(QCustomPlot *customPlot);
	//��ʱ�������ڵ���realtimeDataSlot()�ۣ�ʵ�ֶ�̬������ӵ�����
	QTimer* pdataTimer;
	QCustomPlot* m_qCustomPlot;
	private slots:
	//���ʵʱ���ݲ�
	void updataX();
	void startCj();
	void realtimeDataSlot();
	//void realtimeDataSlot();
private:
	QByteArray  requestData;
	Ui::testCHARTClass ui;
	QTimer dataTimer;
};
