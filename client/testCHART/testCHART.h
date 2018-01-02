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
	//定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
	QTimer* pdataTimer;
	QCustomPlot* m_qCustomPlot;
	private slots:
	//添加实时数据槽
	void updataX();
	void startCj();
	void realtimeDataSlot();
	//void realtimeDataSlot();
private:
	QByteArray  requestData;
	Ui::testCHARTClass ui;
	QTimer dataTimer;
};
