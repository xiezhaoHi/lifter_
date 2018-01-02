#include "custom.h"
#include "ui_custom.h"
#include<QHBoxLayout>
#include<QListWidgetItem>
#include<QDebug>
#include<QMessageBox>
#include"mylabel.h"
#include"mylabel_add.h"
#include<QTimer>
#include"enum.h"

//,ui(new Ui::custom)    setupUi(this);
/*自定义实验,以某一层为起点,以额定速度上升,上升高度不小与10m ,中间制动一次, 到达终点后,以额定速度下降, 中间制动一次,回到起点.
 * 此为一个循环
 *
 *
 */
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif

custom::custom(QWidget *parent) :
    QDialog(parent)
{

    initTaskToDevIDMap();
    m_xc_max = 6; //假定
    m_flag = false; //是否点击了删除的标志  false 没点击 true 点击了删除
    m_row = -1;

   combox_cycle = new QComboBox(this);
    pushButton_zdy_ks = new QPushButton(this);
     listWidget_zdy = new QListWidget(this);
     lineEdit_zdy_yxzq = new QLineEdit(this);
    resize(435,473);
     setMinimumSize(QSize(435, 473));
     setMaximumSize(QSize(435, 473));
    listWidget_zdy->setGeometry(QRect(11, 11, 411, 311));

    QLabel* pLabel = new QLabel(this);
    pLabel->setGeometry(QRect(91, 361, 68, 16));
    pLabel->setText("运行周期");

    lineEdit_zdy_yxzq->setGeometry(QRect(167, 361, 111, 21));
    combox_cycle->setGeometry(QRect(290, 360, 121, 22));
    combox_cycle->addItem("单位:次数");
    combox_cycle->addItem("单位:天数");
    pushButton_zdy_ks->setGeometry(QRect(160, 420, 121, 28));;
    pushButton_zdy_ks->setText("开始执行");




    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(dodeleteitme()));
    timer->start(100);
    connect(this,&custom::deletselectItem_signal,this,&custom::deletselectItem_slot);

    setWindowTitle(QString("自定义"));
   // setFixedSize()
  //选层列表


   //  listWidget_zdy
    connect(pushButton_zdy_ks,&QPushButton::clicked,this,&custom::button_clicked); //链接 开始按钮 点击信号 和槽
     listWidget_zdy->setResizeMode(QListView::Adjust);
       listWidget_zdy->setAutoScroll(true);

       connect( listWidget_zdy,&QListWidget::currentRowChanged,this,&custom::currentRowChanged);

     QWidget * wContainer = new QWidget( listWidget_zdy);

       QHBoxLayout *hLayout = new QHBoxLayout(wContainer);

       QLabel* label_bz = new QLabel(wContainer);
       label_bz->setText(QString("步骤%1").arg( listWidget_zdy->count()+1));
       QComboBox *combox_task = new QComboBox(wContainer);
       combox_task->addItem(("上行"));
       combox_task->addItem(("下行"));
       combox_task->addItem(("开门"));
        combox_task->addItem(("关门"));
       combox_task->addItem(("选层"));
       combox_task->addItem(("制动"));
    connect(combox_task,&QComboBox::currentTextChanged,this,&custom::editText_xc);

    QComboBox* combox_xc = new QComboBox(wContainer); //选层
     combox_xc->hide();
    for(int index = 1; index <= m_xc_max; ++ index )
        combox_xc->addItem(QString("%1").arg(index));


//    combox_task->setStyleSheet("QComboBox{border:1px solid gray;}"
//      "QComboBox QAbstractItemView::item{height:20px;}" //下拉选项高度
//      "QComboBox::down-arrow{image:url(:/images/G:/测试图片/Entypo_26c8(0).ico);}" //下拉箭头
//      "QComboBox::drop-down{border:0px;}"); //下拉按钮

       mylabel_add *pushbutton_add = new mylabel_add(wContainer);
       pushbutton_add->setText(QStringLiteral("增加"));


       connect(pushbutton_add,&mylabel_add::label_clicked,this,&custom::clicked);

         hLayout->addWidget(label_bz);
         hLayout->addStretch(1);
       hLayout->addWidget(combox_task);
       hLayout->addStretch(1);//将空白没有widget的地方分成了若干份，按比例分配
       hLayout->addWidget(combox_xc);
       hLayout->addStretch(1);//将空白没有widget的地方分成了若干份，按比例分配

       hLayout->addWidget(pushbutton_add);


       hLayout->setContentsMargins(0,0,0,0);//关键代码，如果没有很可能显示不出来
    // wContainer->setLayout(hLayout);//如果layout在创建时就已经将父窗口指针当参数，那就不用setlayout
     //  wContainer->resize(350,150);

       QListWidgetItem *alarmItem = new QListWidgetItem( listWidget_zdy);
       struct_rwqd*  prwqd = new struct_rwqd;
       prwqd->m_label_bz = label_bz;  //步骤数
       prwqd->m_combox_rw = combox_task; //任务清单
       prwqd->m_combox_xc = combox_xc; //层数选择
         m_mapWidgets.insert(alarmItem,prwqd);  //map 保存 控件组
         m_mapCombox.insert(combox_task,alarmItem);//保存 任务控件对应的item
        listWidget_zdy->setItemWidget(alarmItem,wContainer);

    /*
     * 2017/06/22
     * 固定化 步骤 起点 终点
     */
#ifndef ZDY_TASK  //自定义任务
    //combox_task->setItemText(0,QString("起点"));
    combox_task->setCurrentIndex(4);
    combox_task->setEnabled(false);
    pushbutton_add->setVisible(false);

    clicked(true);
#endif

	/************************************************************************/
	/*界面设置 */

	pushButton_zdy_ks->setStyleSheet(QString("QPushButton{background-color:rgb(101,203,211)}QPushButton:pressed{background-color:rgb(21,172,230)}"));

	setWindowIcon(QIcon(":/images/Resources/logo.ico"));

	/************************************************************************/


}


void    custom::initTaskToDevIDMap()
{
    m_Task_to_DevID[QString("上行")]=QString("20020007");
    m_Task_to_DevID[QString("下行")]=QString("20020008");
    m_Task_to_DevID[QString("开门")]=QString("20020010");
    m_Task_to_DevID[QString("关门")]=QString("20020011");
    m_Task_to_DevID[QString("选层")]=QString("2002000");
    m_Task_to_DevID[QString("制动")]=QString("20020009");
}



custom::~custom()
{
   // delete ui;
}
 void custom::clicked(bool flag)
 {

     QWidget *wContainer = new QWidget( listWidget_zdy);

     QHBoxLayout *hLayout = new QHBoxLayout(wContainer);
     QLabel* label_bz = new QLabel(wContainer);
     label_bz->setText(QString("步骤%1").arg( listWidget_zdy->count()+1));
     QComboBox *combox_task = new QComboBox(wContainer);
     combox_task->addItem(("上行"));
     combox_task->addItem(("下行"));
     combox_task->addItem(("开门"));
     combox_task->addItem(("关门"));
     combox_task->addItem(("选层"));
     combox_task->addItem(("制动"));
    connect(combox_task,&QComboBox::currentTextChanged,this,&custom::editText_xc);

    QComboBox* combox_xc = new QComboBox(wContainer); //选层
     combox_xc->hide();
    for(int index = 1; index <= m_xc_max; ++ index )
        combox_xc->addItem(QString("%1").arg(index));


     mylabel *pushbutton_delete = new mylabel(wContainer);
     pushbutton_delete->setText(("删除"));

     connect(pushbutton_delete,&mylabel::label_clicked,this,&custom::clicked_delete);


        hLayout->addWidget(label_bz);
        hLayout->addStretch(1);
        hLayout->addWidget(combox_task);
         hLayout->addStretch(1);
         hLayout->addWidget(combox_xc);

     hLayout->addStretch(1);//将空白没有widget的地方分成了若干份，按比例分配
     hLayout->addWidget(pushbutton_delete);
     hLayout->setContentsMargins(0,0,0,0);//关键代码，如果没有很可能显示不出来
  // wContainer->setLayout(hLayout);//如果layout在创建时就已经将父窗口指针当参数，那就不用setlayout
     wContainer->resize(350,150);

     QListWidgetItem *alarmItem = new QListWidgetItem( listWidget_zdy);
     struct_rwqd*  prwqd = new struct_rwqd;
     prwqd->m_label_bz = label_bz;  //步骤数
     prwqd->m_combox_rw = combox_task; //任务清单
     prwqd->m_combox_xc = combox_xc; //层数选择
       m_mapWidgets.insert(alarmItem,prwqd);  //map 保存 控件组
       m_mapCombox.insert(combox_task,alarmItem); //保存 任务控件对应的item
      listWidget_zdy->setItemWidget(alarmItem,wContainer);


#ifndef ZDY_TASK  //自定义任务
    combox_task->setCurrentIndex(4);
    combox_task->setEnabled(false);
    pushbutton_delete->setVisible(false);
    combox_xc->setCurrentIndex(m_xc_max-1);

#endif
}
 void custom::clicked_delete(bool flag)
 {
    m_flag = true;
 }
 void custom::itemChanged(QListWidgetItem *item)
 {

 }
void custom::itemSelectionChanged(void)
{
}

void  custom::currentRowChanged(int row)
{
if(m_flag)
m_row = row;
}


void  custom::deletselectItem_slot(int row)
{
}
void  custom::dodeleteitme(QPrivateSignal)
{
    if(m_flag )
    {
        m_row =  listWidget_zdy->currentRow();
        if(m_row != -1 || m_row != 0)
        {
            QListWidgetItem* item =  listWidget_zdy->takeItem(m_row);

            delete item;
           //  listWidget_zdy->removeItemWidget( listWidget_zdy->currentItem());
            m_flag = false;
            m_row = -1;
             listWidget_zdy->setCurrentRow(m_row);

            for(int index = 0; index <  listWidget_zdy->count(); ++ index)
            {
               m_mapWidgets[ listWidget_zdy->item(index)]->m_label_bz->setText(QString("步骤%1").arg(index+1));
            }
        }

    }
}
  void custom::editText_xc(const QString &str)
  {
    QComboBox* pSender =  static_cast<QComboBox*>(sender()) ;
    //pSender->hide();
    if(str == "选层")
    {

       m_mapWidgets[ m_mapCombox[pSender]]->m_combox_xc->show();
          //  gcombox->show();

    }
    else {
        m_mapWidgets[ m_mapCombox[pSender]]->m_combox_xc->hide();
    }
  }

  //开始执行
void custom::button_clicked(bool flag)
{
#ifndef ZDY_TASK
    QStringList strList;
    unsigned int times = lineEdit_zdy_yxzq->text().toInt();

    if(0 == combox_cycle->currentIndex() && times < 30)
    {
        QMessageBox msg(QMessageBox::NoIcon,QString("提示!"),QString("周期次数不少于30次"),QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(1 == combox_cycle->currentIndex() && times <= 0)
    {
        QMessageBox msg(QMessageBox::NoIcon,QString("提示!"),QString("周期天数应大于0"),QMessageBox::Ok);
        msg.exec();
        return;
    }

    /*list 为    起点层数#终点层数#周期时间#
     * 周期时间 1表示 天数 ,0 表示 次数
     */
    QList<struct_result> list ;//= new QList<struct_result>();
    for(int index = 0; index <  listWidget_zdy->count(); ++ index)
    {
       struct_rwqd* pstruct =  m_mapWidgets[ listWidget_zdy->item(index)];

       if(pstruct->m_combox_rw->currentText() == "选层")
         {
           strList.append(pstruct->m_combox_xc->currentText()) ;
         }
     }
    if(combox_cycle )
    {
        strList << QString("%1").arg(combox_cycle->currentIndex() );
    }


    emit sendresult(strList,times);
    accept() ;
#else
     QStringList strList;
     int times  = 1;
    QList<struct_result> list ;//= new QList<struct_result>();
    for(int index = 0; index <  listWidget_zdy->count(); ++ index)
    {
       struct_rwqd* pstruct =  m_mapWidgets[ listWidget_zdy->item(index)];
       struct_result result;
       result.strStep =  pstruct->m_label_bz->text();
       result.strTask = pstruct->m_combox_rw->currentText();


       if(result.strTask == "选层")
         {
           result.strChooseNum = pstruct->m_combox_xc->currentText();
           //strList
           strList.append(QString("%1%2").arg(m_Task_to_DevID[QString("选层")]).arg(result.strChooseNum));
          }
        else
           strList.append(m_Task_to_DevID[result.strTask]);
    list.append(result);
    }
    struct_cycle cycle;
    if(combox_cycle)
        cycle.strCycleType =combox_cycle->currentText();
    cycle.strCycleNum =  lineEdit_zdy_yxzq->text();

    times =lineEdit_zdy_yxzq->text().toInt();
    emit sendresult(strList,times);
    accept() ;
#endif
}
