#include<QMessageBox>
#include<QMovie>
#include <QTime>
#include<QSettings>
#include "configdialog.h"
#include "ui_configdialog.h"
#include"usebuffer.h"
#include"enum.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")

#endif
ConfigDialog::ConfigDialog(double configD, double configP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

     ui->lineEdit_d->setText(QString::number(configD))  ; //编码器 配置 圈半径  cm
     ui->lineEdit_p->setText(QString::number(configP)); //编码器 配置 脉冲 刻度

     ui->label_3->setVisible(false);
     if(!InitCombox())
     {
         QMessageBox message(QMessageBox::Warning,"提示","读配置失败,请检查配置文件是否正确!",QMessageBox::Ok,NULL);
         message.exec();
     }


    setWindowTitle(QString("旋转编码器配置"));
	/************************************************************************/
	/*界面设置 */

	ui->pushButton_qd->setStyleSheet(QString("QPushButton{background-color:rgb(101,203,211)}QPushButton:pressed{background-color:rgb(21,172,230)}"));

	setWindowIcon(QIcon(":/images/logo"));

	/************************************************************************/

}
ConfigDialog::ConfigDialog( QWidget *parent ):
    QDialog(parent),
    ui(new Ui::ConfigDialog)

{
    ui->setupUi(this);
    ui->label_3->setVisible(false);
    InitCombox(); //初始化combox

    setWindowTitle(QString("客户端关联电梯配置"));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}
//QMessageBox(Icon icon, const QString &title, const QString &text,
//            StandardButtons buttons = NoButton, QWidget *parent = Q_NULLPTR,
//            Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
void ConfigDialog::on_pushButton_qd_clicked()
{
    double d = ui->lineEdit_d->text().toDouble();
    double p = ui->lineEdit_p->text().toDouble();
    QString strLifter;
    if(0 != d && 0 != p)
    {

        QString text;
        text = ui->comboBox->currentText();
        ui->label_3->setVisible(true);
        ui->label_3->setPixmap(QString(":/images/rotate"));
        QMovie *movie;//创建movie

        movie = new QMovie(":/images/rotate");//和创建图片资源一样
        ui->label_3->setMovie(movie);//承载gif
        movie->start();//开启
        ui->label_3->show();

        ui->pushButton_qd->setVisible(false);
        strLifter = m_combox_lifter[text];
         emit SetBmqConfig(d,p,strLifter,"1");

        QTime dieTime = QTime::currentTime().addMSecs(WAIT_FOR_TIME);
        int response=2;
        while( QTime::currentTime() < dieTime )
        {
            response = UserBuffer::GetInstance()->PopServerResponse();
            if(-1 == response)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            else
                break;
        }
        delete movie;
        ui->label_3->setVisible(false);

        if(0 == response)
            text = "设置失败";
        else if(1 == response)
            text = "设置成功";
        else
            text = "设置超时";
        QMessageBox message(QMessageBox::Warning,"提示",text,QMessageBox::Ok,NULL);
        message.exec();
        if(1 == response)
        {


            QString strPath= qApp->applicationDirPath()+"\\config\\config.ini";
           QSettings iniRead(strPath,QSettings::IniFormat);
           iniRead.setValue("bmqConfig/configR",d);
           iniRead.setValue("bmqConfig/configP",p);

           //配置选择的电梯
           iniRead.setValue("lifter/lifter_choose",strLifter);
            QString name = GetLifterName(strLifter);
            emit SetTitle(name);
            close();
        }
        else
            ui->pushButton_qd->setVisible(true);
        return;
    }
    QString strPath= qApp->applicationDirPath()+"\\config\\config.ini";
    QSettings iniRead(strPath,QSettings::IniFormat);
    iniRead.setValue("lifter/lifter_choose",strLifter);
    QMessageBox message(QMessageBox::Warning,"提示","请填写正确的设置",QMessageBox::Ok,NULL);
    message.exec();
}

/*
 * 通过 电梯id 获取相应的名字
 */
QString ConfigDialog::GetLifterName(QString const& strTemp)
{
    QString strCombox;
    int iTemp = strTemp.right(4).toInt();
    if(strTemp.left(4) == LIFTER_TYPE_YLS) //曳引式 升降机
    {

        if(iTemp>1)
        {
            strCombox = QString("曳引式升降机_%1").arg(iTemp);
        }
        else
        {
            strCombox = QString("曳引式升降机");
        }
    }
    if(strTemp.left(4) == LIFTER_TYPE_QZS) //强制式简易升降机
    {

        if(iTemp>1)
        {
            strCombox = QString("强制式简易升降机_%1").arg(iTemp);
        }
        else
        {
            strCombox = QString("强制式简易升降机");
        }
    }
    if(strTemp.left(4) == LIFTER_TYPE_SCSL) //sc双笼施工升降机
    {

     strCombox = QString("sc双笼施工升降机");
       
    }
    return strCombox;
}

/*
 * 初始化combox
 */
bool    ConfigDialog::InitCombox()
{
    QString strPath= qApp->applicationDirPath()+"\\config\\config.ini";
    QSettings iniRead(strPath,QSettings::IniFormat);


     QString strRet;

     //查看是否选择了 关联的电梯

     QStringList conifg_list;
     QString strTemp;

     QString strCombox;
     //先填充 m_lisfter_list
      int  max = iniRead.value("lifter/lifter_max",-1).toInt();
      if(max < 1)
      {
         // QDebug() << "lifter config error";
          return false;
      }
     for(int index = 0; index < max; ++index)
     {
         strRet = iniRead.value(QString("lifter/lifter_%1").arg(index),"").toString();
         if(strRet.isEmpty())
         {
           break;
         }
         conifg_list.append(strRet);
     }

//     strRet = iniRead.value("lifter/lifter_choose","").toString();
//     if(strRet.isEmpty())
     {



         for(int index = 0; index < conifg_list.size(); ++index)
         {
             strTemp = conifg_list[index];

             strCombox = GetLifterName(strTemp);
              ui->comboBox->addItem(QIcon(),strCombox);
             m_combox_lifter[strCombox] = strTemp;
         }
     }
//     else
//       {
//         for(int index = 0; index < conifg_list.size(); ++index)
//         {
//             strTemp = conifg_list[index];
//             strCombox = GetLifterName(strTemp);
//             m_combox_lifter[strCombox] = strTemp;
//         }
//         ui->comboBox->addItem(QIcon(),GetLifterName(strRet));
//        }

     return true;
}
