#include "Lifter_server_mscv.h"
#include<QEventLoop>
Lifter_server_mscv::Lifter_server_mscv(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_model = new QStringListModel(this);

	ui.listView->setModel(m_model);
	connect(&m_showThread, &Thread::showData, this, &Lifter_server_mscv::show_ui_Data);
	m_showThread.start();
	if (!server_manager::GetInstance()->start_server())
		close();
}
void Lifter_server_mscv::closeEvent(QCloseEvent *event)
{
	//    QStringList list;
	//    list.append(QString("服务正在退出,请耐心等待!"));
	//    UserBuffer::GetInstance()->PushUserDLData(list);
	server_manager::GetInstance()->stop_server();

}

Lifter_server_mscv::~Lifter_server_mscv()
{

	QEventLoop eventLoop;
	connect(&m_showThread, &QThread::finished, &eventLoop, &QEventLoop::quit);
	m_showThread.stop();
	m_showThread.wait();
	eventLoop.exec();
	delete m_model;
	

}
void    Lifter_server_mscv::show_ui_Data(QStringList const& list)
{
	int row = ui.listView->currentIndex().row() + 1;
	m_model->insertRows(row, 1);

	QModelIndex index = m_model->index(row);
	ui.listView->setCurrentIndex(index);
	QString strData;
	for (int ind = 0; ind<list.size(); ++ind)
		strData += "  " + list.at(ind);
	m_model->setData(index, strData);

	int rowMax = m_model->rowCount();
	if (rowMax >= 100)
		on_pushButton_clear_clicked();
}

void Lifter_server_mscv::on_pushButton_clear_clicked()
{

	int rowMax = m_model->rowCount();
	for (; rowMax >= 0; --rowMax)
	{
		m_model->removeRow(rowMax);
	}
}
