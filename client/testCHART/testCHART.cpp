#include "testCHART.h"
#include "QtPrintSupport/QtPrintSupport"
#include "QComboBox"
#include "QHBoxLayout"
#include "QPushButton"
testCHART::testCHART(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	
	QWidget* centerWindow = new QWidget;
	pCustomPlot = new QCustomPlot(this);
	pCustomPlot->setGeometry(200, 0, 500, this->height()); //前两个参数是位置坐标，后两

	QHBoxLayout* pLayout = new QHBoxLayout;
	 pBox = new QComboBox(this);
	 for (int index = 1; index < 10; ++index)
	 {
		 pBox->addItem( QString("com%1").arg(index));
	 }
	 pBox->setGeometry(0, 0, 200, 50); //前两个参数是位置坐标，后两
	QPushButton* pButton = new QPushButton(this);
	pButton->setGeometry(0, 100, 200, 50); //前两个参数是位置坐标，后两
	pButton->setText("start");
	connect(pButton, &QPushButton::clicked, this, &testCHART::startCj);

	pLayout->addWidget(pBox);
	pLayout->addWidget(pButton);
	pLayout->addWidget(pCustomPlot);

	

	//centerWindow->setLayout(pLayout);

	//setCentralWidget(centerWindow);

	// 向绘图区域QCustomPlot添加一条曲线
	//

	// 添加数据
	
	// 设置坐标轴名称
	//pCustomPlot->xAxis->setLabel("x");
	//pCustomPlot->yAxis->setLabel("y");

	// 设置背景色
 	pCustomPlot->setBackground(QColor(50, 50, 50));
	pGraph = pCustomPlot->addGraph();
  	pGraph->setPen(QPen(QColor(32, 178, 170)));
 
	QVector<double> temp(10);
	QVector<double> temp1(10);


	for (int i = 0; i < 9; i++)
	{

		temp[i] = i;
		temp1[i] = 64;
	}
	temp1[0] = 0;
	pCustomPlot->addGraph();
	//pCustomPlot->graph(0)->setPen(QPen(Qt::red));
	//pCustomPlot->graph(0)->setData(temp, temp1);

// 
// 	// 设置x/y轴文本色、轴线色、字体等
// 	pCustomPlot->xAxis->setTickLabelColor(Qt::white);
// 	pCustomPlot->xAxis->setLabelColor(QColor(0, 160, 230));
// 	pCustomPlot->xAxis->setBasePen(QPen(QColor(32, 178, 170)));
// 	pCustomPlot->xAxis->setTickPen(QPen(QColor(128, 0, 255)));
// 	pCustomPlot->xAxis->setSubTickPen(QColor(255, 165, 0));
// 	QFont xFont = pCustomPlot->xAxis->labelFont();
// 	xFont.setPixelSize(20);
// 	pCustomPlot->xAxis->setLabelFont(xFont);
// 
// 	pCustomPlot->yAxis->setTickLabelColor(Qt::white);
// 	pCustomPlot->yAxis->setLabelColor(QColor(0, 160, 230));
// 	pCustomPlot->yAxis->setBasePen(QPen(QColor(32, 178, 170)));
// 	pCustomPlot->yAxis->setTickPen(QPen(QColor(128, 0, 255)));
// 	pCustomPlot->yAxis->setSubTickPen(QColor(255, 165, 0));
// 	QFont yFont = pCustomPlot->yAxis->labelFont();
// 	yFont.setPixelSize(20);
// 	pCustomPlot->yAxis->setLabelFont(yFont);

	// 设置坐标轴显示范围，否则只能看到默认范围
	pCustomPlot->xAxis->setRange(0, 255);
	pCustomPlot->yAxis->setRange(0, 255);

	pCustomPlot->xAxis->setVisible(false);
	pCustomPlot->yAxis->setVisible(false);

	

	setupRealtimeDataDemo(pCustomPlot);
	pCustomPlot->replot();



	my_serialport = new QSerialPort();     //定义1个串口类型
	
	//一旦串口接收到数据，便会去执行updata()函数
	//setLayout(pLayout);
	pdataTimer = new QTimer(this);
	connect(pdataTimer, SIGNAL(timeout()), this, SLOT(updataX()));
	
}

void testCHART::startCj()
{
	QString com = pBox->currentText();
	QSerialPortInfo port;
	my_serialport->setPortName(com);

// 	QSerialPortInfo com_info;
// 	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
// 	{
// 		QString strCom = info.portName();
// 		if (strCom.toUpper() == com.toUpper())
// 		{
// 			com_info = info;
// 			break;
// 		}
// 	}
// 	my_serialport->setPort(com_info);	//设置COM口
// 	
// 	if (my_serialport->open(QIODevice::ReadWrite))
// 	{
// 		qDebug() << "my_serialport->open(QIODevice::ReadWrite)";
// 		my_serialport->setBaudRate(QSerialPort::Baud115200);
// 		my_serialport->setParity(QSerialPort::NoParity);
// 		my_serialport->setDataBits(QSerialPort::Data8);
// 		my_serialport->setStopBits(QSerialPort::OneStop);
// 		my_serialport->setFlowControl(QSerialPort::NoFlowControl);
// 
// 		my_serialport->clearError();
// 		my_serialport->clear();
// 		connect(my_serialport, SIGNAL(readyRead()), this, SLOT(updataX()));
// 	}
// 	return;
// 
// 	

	my_serialport->close();					//先关串口，再打开，可以保证串口不被其它函数占用。

	if (my_serialport->open(QIODevice::ReadWrite))		//以可读写的方式打开串口
	{
		my_serialport->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);//设置波特率和读写方向

		my_serialport->setDataBits(QSerialPort::Data8);		//数据位为8位

		my_serialport->setFlowControl(QSerialPort::NoFlowControl);//无流控制

		my_serialport->setParity(QSerialPort::NoParity);	//无校验位

		my_serialport->setStopBits(QSerialPort::OneStop);	//一位停止位
		//my_serialport->setDataTerminalReady(true);
		//connect(my_serialport, &QSerialPort::readyRead, this,&testCHART::updataX);
		//my_serialport->waitForReadyRead(3000);
		//requestData = my_serialport->readAll();
		pdataTimer->start(100);
 //		my_serialport->waitForReadyRead(3000);
// 		QByteArray responseData = QString("wocao").toLocal8Bit();
		int er = GetLastError();
//		int ret =my_serialport->write(responseData);
//		my_serialport->waitForBytesWritten(1000);
		 er = GetLastError();
		 QMessageBox box;
		 box.setText(("begin"));
		 box.exec();
	}
}

//读取数据  串口读取出来的数据类型 是QByteArray 不是QString
const int gMax = 10;

static int gCount = 0;
static double gIndex = 0;
static QString gShow;
void testCHART::updataX()
{
	
	requestData.clear();


	my_serialport->waitForReadyRead(50);
	requestData = my_serialport->readAll();
	QString strRet = QString::fromStdString(requestData.toHex().toStdString());


	if (strRet.size() < 4 || strRet.size()%2 != 0) //数据 不全
	{
		return;
	}

	QString strSplit; //分隔符

	/*
	* 通过算法获取分割符号.
	* 当收到的数据大于等于 4字符(2字节),取前4个字符 拆分2个字符  分别计算字符串中个数. 
	* 取个数多的字符串为 分隔符.当相同时 取第一个字符串
	* 例如:2f402f50  取 2f 为分隔符   2f402f40  取 2f为分隔符
	*/
	QString strOne = strRet.mid(0, 2); 
	QString strTwo = strRet.mid(2, 2);

	QStringList listOne = strRet.split(strOne);
	QStringList listTwo = strRet.split(strTwo);

	int listSizeOne = listOne.size(), listSizeTwo = listTwo.size();
	if (listSizeOne >= listSizeTwo)
	{
		strSplit = strOne;
	}
	else
		strSplit = strTwo;

	

	QStringList listShow;
	QString strT = strRet;
	while (!strRet.isEmpty())
	{
		int index = strRet.indexOf(strSplit);
		if (index >= 0)
		{
			QString strTemp = strRet.mid(index, 4);
			if (strTemp.size() == 4)
			{
				listShow.append(strTemp);
				strRet = strRet.mid(index + 4, strRet.size() - 4);
			}
			else
			{
				strRet = "";
			}
		}
		else
		{
			strRet = "";
		}
	}


	
	
	int size = listShow.size();
	if (size < 1)
	{
		return;
	}

	QVector<double>* gx = new QVector<double>(size), *gy = new QVector<double>(size);
	//QVector<double> gx(gMax), gy(gMax);
	gCount = 0;
	
	for each (QString var in listShow)
	{
		bool ok;
		double t = var.right(2).toInt(&ok, 16);
		if (t < 1)
		{
			continue;
		}
	
		if (gIndex  >= 255)
		{
			gIndex = 0;
			pCustomPlot->clearGraphs();
			QCPGraph* pGraph = pCustomPlot->addGraph();
			pGraph->setPen(QPen(QColor(32, 178, 170)));
			pGraph->setBrush(QBrush(QColor(0, 0, 255, 20)));
			//pCustomPlot->replot();
		}
		gIndex += 1;
		//(*gx)[gCount-1] = gIndex;
		
		//(*gy)[gCount - 1] =255 -  t;
		pCustomPlot->graph(0)->addData(gIndex, 255- t);
		//gShow.append(QString("[%1-%2]").arg(gIndex).arg(255 - t));

		pCustomPlot->replot();
	}
	//QCPGraph* Graph = pCustomPlot->addGraph();

	delete gx;
	delete gy;
	


}

//画图初始化
void testCHART::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
	
}

void testCHART::realtimeDataSlot()
{
	
}
