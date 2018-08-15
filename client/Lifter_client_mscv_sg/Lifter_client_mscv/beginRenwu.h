#pragma once

#include <QDialog>
//#include "ui_beginRenwu.h"
class QComboBox;
class QLineEdit;
class beginRenwu : public QDialog
{
	Q_OBJECT

public:
	beginRenwu(QDialog *parent = Q_NULLPTR);
	~beginRenwu();
	public slots:
	void beginStartRenwu();
	void beginEndRenwu();
private:
	//Ui::beginRenwu ui;

	//开始 结束 combox
	QComboBox*  m_beginCom;
	QComboBox*  m_endCom;
	//选择类型 0 次数 1时间(天数)
	QComboBox*  m_chooseType;
	//选择的周期 
	QLineEdit*  m_chooseData;

	//电梯选择
	QComboBox* m_chooseLifter;
};
