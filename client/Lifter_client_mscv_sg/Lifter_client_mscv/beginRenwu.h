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

	//��ʼ ���� combox
	QComboBox*  m_beginCom;
	QComboBox*  m_endCom;
	//ѡ������ 0 ���� 1ʱ��(����)
	QComboBox*  m_chooseType;
	//ѡ������� 
	QLineEdit*  m_chooseData;

	//����ѡ��
	QComboBox* m_chooseLifter;
};
