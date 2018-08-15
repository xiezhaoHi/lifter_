#include "beginRenwu.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include "client_manager.h"
//楼层最大
static const int louCMax = 7;
//控件固定宽 高
static const int fixHeight = 30;
static const int fixWidth = 100;
beginRenwu::beginRenwu(QDialog *parent)
	: QDialog(parent)
{
	//ui.setupUi(this);
	resize(400, 300);
	setWindowTitle(QStringLiteral("周期执行"));

	QLabel* pChooseLifterLabel = new QLabel(this);
	pChooseLifterLabel->setText(QStringLiteral("电梯选择"));

	m_chooseLifter = new QComboBox(this);
	m_chooseLifter->addItem(QStringLiteral("笼A"));
	m_chooseLifter->addItem(QStringLiteral("笼B"));

	QLabel* pBeginLable = new QLabel(this);
	pBeginLable->setText(QStringLiteral("开始楼层"));
	m_beginCom = new QComboBox(this);
	

	QLabel* pEndLabel = new QLabel(this);
	pEndLabel->setText(QStringLiteral("结束楼层"));
	m_endCom = new QComboBox(this);

	for (int index = 1; index <= louCMax; ++index)
	{
		m_beginCom->addItem(QString(QStringLiteral("%1楼")).arg(index ));
		m_endCom->addItem(QString(QStringLiteral("%1楼")).arg(index ));
	}

	QLabel* pZhouqi = new QLabel(this);
	pZhouqi->setText(QStringLiteral("周期选择"));

	m_chooseType = new QComboBox(this);
	m_chooseType->addItem(QStringLiteral("次数"));
	m_chooseType->addItem(QStringLiteral("时间(天)"));

	m_chooseData = new QLineEdit(this);
	

	QPushButton* pStartBt = new QPushButton(QStringLiteral("开始执行"));
	
	connect(pStartBt, &QPushButton::clicked, this, &beginRenwu::beginStartRenwu);

	QPushButton* pEndBt = new QPushButton(QStringLiteral("开始执行"));

	connect(pEndBt, &QPushButton::clicked, this, &beginRenwu::beginEndRenwu);
	
	m_beginCom->setFixedWidth(fixWidth);
	pStartBt->setFixedWidth(fixWidth);
	pEndBt->setFixedWidth(fixWidth);
	m_chooseLifter->setFixedWidth(fixWidth);
	m_endCom->setFixedWidth(fixWidth);
	m_chooseType->setFixedWidth(fixWidth);
	pEndBt->setFixedWidth(fixWidth);
	m_chooseData->setFixedWidth(fixWidth);
	m_chooseData->setFixedHeight(fixHeight);


	QGridLayout* pGrid = new QGridLayout;
	pGrid->addWidget(pChooseLifterLabel, 0, 0);
	pGrid->addWidget(m_chooseLifter, 0, 1);
	pGrid->addWidget(pBeginLable, 1,0);
	pGrid->addWidget(m_beginCom, 1, 1);
	pGrid->addWidget(pEndLabel, 2, 0);
	pGrid->addWidget(m_endCom, 2, 1);

	pGrid->addWidget(pZhouqi, 3, 0);
	pGrid->addWidget(m_chooseType, 3, 1);
	pGrid->addWidget(m_chooseData, 3, 2);

	pGrid->addWidget(pStartBt, 4,0);
	pGrid->addWidget(pEndBt, 4, 2);
	setLayout(pGrid);
}
void beginRenwu::beginStartRenwu()
{
	int lifterIndex = m_chooseLifter->currentIndex();
	QString strLifterID = lifterIndex == 0 ? "00030001" : "00030002";

	int beginIndex = m_beginCom->currentIndex()+1;
	int endIndex = m_endCom->currentIndex()+1;

	int chooseType = m_chooseType->currentIndex();
	int chooseDate = m_chooseData->text().toInt();
	//数据无效
	if (chooseDate < 1)
	{
		QMessageBox box;
		box.setWindowTitle(QStringLiteral("提示"));
		box.setText(QStringLiteral("输入的时间或者次数无效"));
		box.exec();
		return;
	}

	//发送任务到服务端
	QStringList list;
	list.append(QString("<start>1</start>\
		<lifter>%1</lifter>\
		<begin>%2</begin>\
		<end>%3</end>\
		<times type = '%4'>%5</times>").arg(strLifterID)
		.arg(beginIndex).arg(endIndex).arg(chooseType).arg(chooseDate));
	bool ret = client_manager::GetInstance()->send_to_server("4002", list);
	QString strShow;
	if (ret)
	{
		strShow = QStringLiteral("操作成功!");
	}
	else
	{
		strShow = QStringLiteral("操作失败!");
	}
	QMessageBox box;
	box.setWindowTitle(QStringLiteral("提示"));
	box.setText(strShow);
	box.exec();
	return;
}

void beginRenwu::beginEndRenwu()
{
	//发送任务到服务端
	QStringList list;
	list.append(QString("<start>0</start>\
		<lifter>0</lifter>\
		<begin>0</begin>\
		<end>0</end>\
		<times type = '0'>0</times>"));
	bool ret = client_manager::GetInstance()->send_to_server("4002", list);
	QString strShow;
	if (ret)
	{
		strShow = QStringLiteral("操作成功!");
	}
	else
	{
		strShow = QStringLiteral("操作失败!");
	}
	QMessageBox box;
	box.setWindowTitle(QStringLiteral("提示"));
	box.setText(strShow);
	box.exec();
	return;
}

beginRenwu::~beginRenwu()
{
}
