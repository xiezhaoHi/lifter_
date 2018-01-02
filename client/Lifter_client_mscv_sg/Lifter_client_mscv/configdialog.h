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
//     * ������� ����list
//     */
//    QStringList& m_config_list;
    /*
     * �ͻ��� ������ lifter
     */
    QString     m_connect_lifter;
    /*
     * combox text ���� ��������
     */
    QMap<QString,QString> m_combox_lifter;
    /*
     * ��ʼ��combox
     */
    bool    InitCombox();
    /*
     * ͨ�� ����id ��ȡ��Ӧ������
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
     * ���ô�������
     */
    void  SetTitle(QString const& strName);
};

#endif // CONFIGDIALOG_H
