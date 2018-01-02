#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include<QMap>
namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(double configD, double configP, QWidget *parent = 0);
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

//    /*
//     * 保存电梯 配置list
//     */
//    QStringList& m_config_list;
    /*
     * 客户端 关联的 lifter
     */
    QString     m_connect_lifter;
    /*
     * combox text 关联 电梯配置
     */
    QMap<QString,QString> m_combox_lifter;
    /*
     * 初始化combox
     */
    bool    InitCombox();
    /*
     * 通过 电梯id 获取相应的名字
     */
    QString GetLifterName(QString const& strTemp);
private slots:
    void on_pushButton_qd_clicked();

private:
    Ui::ConfigDialog *ui;
    /***********************************************************/

Q_SIGNALS:
    void SetBmqConfig(double d, double p,QString const& strLifter, const QString &strBmqID);
    /*
     * 设置窗口名字
     */
    void  SetTitle(QString const& strName);
};

#endif // CONFIGDIALOG_H
