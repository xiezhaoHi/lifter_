#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include"server_manager.h"
#include<QEventLoop>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    m_model = new QStringListModel(this);

    ui->listView->setModel(m_model);
    connect(&m_showThread,&Thread::showData,this,&MainWindow::show_ui_Data);
    m_showThread.start();
    if(!server_manager::GetInstance()->start_server())
        close();


}
void MainWindow::closeEvent(QCloseEvent *event)
{
//    QStringList list;
//    list.append(QString("服务正在退出,请耐心等待!"));
//    UserBuffer::GetInstance()->PushUserDLData(list);
    server_manager::GetInstance()->stop_server();

}

MainWindow::~MainWindow()
{

    QEventLoop eventLoop;
    connect(&m_showThread,&QThread::finished,&eventLoop, &QEventLoop::quit);
    m_showThread.stop();
    m_showThread.wait();
    eventLoop.exec();
    delete m_model;
    delete ui;

}
void    MainWindow::show_ui_Data(QStringList const& list)
{
    int row = ui->listView->currentIndex().row()+1;
        m_model->insertRows(row, 1);

        QModelIndex index = m_model->index(row);
        ui->listView->setCurrentIndex(index);
        QString strData;
        for(int ind =0; ind<list.size(); ++ind)
            strData += "  "+list.at(ind);
        m_model->setData(index,strData);

        int rowMax = m_model->rowCount();
        if(rowMax>=100)
            on_pushButton_clear_clicked();
}

void MainWindow::on_pushButton_clear_clicked()
{

   int rowMax = m_model->rowCount();
   for(;rowMax>=0;--rowMax)
   {
    m_model->removeRow(rowMax);
   }
}
