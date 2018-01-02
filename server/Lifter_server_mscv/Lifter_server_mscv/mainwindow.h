#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QThread>
#include"buffer/usebuffer.h"
#include<QStringListModel>
#include"server_manager.h"
namespace Ui {
class MainWindow;
}
class Thread:public QThread
{
    Q_OBJECT
public:
    Thread():m_stop(true){}
    void stop()
    {
        m_stop = false;
    }

    void run()
    {
        while(m_stop)
        {
            if(!UserBuffer::GetInstance()->IsEmptyUserDLData())
                emit showData(UserBuffer::GetInstance()->PopUserDLData());
            ::Sleep(100);
        }

    }

    volatile bool   m_stop;
Q_SIGNALS:
    void    showData(QStringList const& list);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStringListModel*    m_model;
    Thread                  m_showThread;
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

public Q_SLOTS:
    void    show_ui_Data(QStringList const& list);

private slots:
    void on_pushButton_clear_clicked();

};

#endif // MAINWINDOW_H
