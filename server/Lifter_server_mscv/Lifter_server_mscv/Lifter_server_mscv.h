#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lifter_server_mscv.h"
#include<QThread>
#include"buffer/usebuffer.h"
#include<QStringListModel>
#include"server_manager.h"

class Thread :public QThread
{
	Q_OBJECT
public:
	Thread() :m_stop(true) {}
	void stop()
	{
		m_stop = false;
	}

	void run()
	{
		while (m_stop)
		{
			if (!UserBuffer::GetInstance()->IsEmptyUserDLData())
				emit showData(UserBuffer::GetInstance()->PopUserDLData());
			::Sleep(100);
		}

	}

	volatile bool   m_stop;
Q_SIGNALS:
	void    showData(QStringList const& list);
};
class Lifter_server_mscv : public QMainWindow
{
	Q_OBJECT

public:
	Lifter_server_mscv(QWidget *parent = Q_NULLPTR);
	~Lifter_server_mscv();
	QStringListModel*    m_model;
	Thread                  m_showThread;
	void closeEvent(QCloseEvent *event);
private:
	Ui::MainWindow ui;
	public Q_SLOTS:
	void    show_ui_Data(QStringList const& list);

	private slots:
	void on_pushButton_clear_clicked();
};
