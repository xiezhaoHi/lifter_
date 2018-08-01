#ifndef CUSTOM_H
#define CUSTOM_H

#include <QDialog>
#include<QListWidgetItem>
#include<QComboBox>
#include<QLabel>
#include<QMap>
#include<QList>
namespace Ui {
class custom;
}
typedef struct  //任务清单  控件结构体
{
    QLabel* m_label_bz;  //步骤数
    QComboBox* m_combox_rw; //具体执行的任务
    QComboBox* m_combox_xc; //选层数
}struct_rwqd;

typedef struct
{
    QString strStep; //步骤
    QString strTask; //任务
    QString strChooseNum; //选择一个层数
}struct_result;
typedef struct
{
    QString strCycleType; //周期类型
    QString strCycleNum;

}struct_cycle;
class custom : public QDialog
{
    Q_OBJECT

public:
    explicit custom(QWidget *parent = 0);
    ~custom();

private:
   // Ui::custom *ui;

    //
    void    initTaskToDevIDMap();
    QMap<QString,QString>   m_Task_to_DevID;


    bool    m_flag;
    int        m_row;
    QMap<QListWidgetItem*,struct_rwqd*>  m_mapWidgets; //listwidget 中每一个item对应的控件组
    QMap<QComboBox*, QListWidgetItem*> m_mapCombox; //控件组中 combox_task 对应的 item
    int    m_xc_max; //选层 最大值
    QComboBox* combox_cycle;
    QPushButton* pushButton_zdy_ks;
    QListWidget* listWidget_zdy;
    QLineEdit* lineEdit_zdy_yxzq;  //zhouqi
Q_SIGNALS:
    void deletselectItem_signal(int row);
    //void sendresult( QList<struct_result> listResult,struct_cycle cycle);
    void sendresult( QStringList  ,int);
public slots:
    void clicked(bool flag);
    void clicked_delete(bool flag);
     void itemChanged(QListWidgetItem *item);
    void itemSelectionChanged(void);
    void currentRowChanged(int row);
    void deletselectItem_slot(int row);
    void  dodeleteitme(QPrivateSignal);
    void editText_xc(const QString &str);
    void button_clicked(bool flag); //开始 button 点击
   // void  start_exec();
};

#endif // CUSTOM_H
