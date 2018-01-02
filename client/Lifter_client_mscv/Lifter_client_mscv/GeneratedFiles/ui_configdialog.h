/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_qd;
    QLineEdit *lineEdit_d;
    QLineEdit *lineEdit_p;
    QComboBox *comboBox;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QStringLiteral("ConfigDialog"));
        ConfigDialog->resize(300, 300);
        ConfigDialog->setMinimumSize(QSize(300, 300));
        ConfigDialog->setMaximumSize(QSize(300, 300));
        label = new QLabel(ConfigDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 91, 16));
        label_2 = new QLabel(ConfigDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 160, 81, 16));
        pushButton_qd = new QPushButton(ConfigDialog);
        pushButton_qd->setObjectName(QStringLiteral("pushButton_qd"));
        pushButton_qd->setGeometry(QRect(110, 240, 93, 28));
        lineEdit_d = new QLineEdit(ConfigDialog);
        lineEdit_d->setObjectName(QStringLiteral("lineEdit_d"));
        lineEdit_d->setGeometry(QRect(130, 80, 120, 21));
        lineEdit_d->setMinimumSize(QSize(80, 20));
        lineEdit_d->setMaximumSize(QSize(120, 30));
        lineEdit_p = new QLineEdit(ConfigDialog);
        lineEdit_p->setObjectName(QStringLiteral("lineEdit_p"));
        lineEdit_p->setGeometry(QRect(130, 160, 120, 21));
        lineEdit_p->setMinimumSize(QSize(80, 20));
        lineEdit_p->setMaximumSize(QSize(120, 30));
        comboBox = new QComboBox(ConfigDialog);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(126, 20, 151, 21));
        label_3 = new QLabel(ConfigDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(120, 210, 72, 71));
        label_4 = new QLabel(ConfigDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(22, 20, 72, 16));

        retranslateUi(ConfigDialog);

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("ConfigDialog", "\345\234\210\345\215\212\345\276\204(CM):", Q_NULLPTR));
        label_2->setText(QApplication::translate("ConfigDialog", "\350\204\211\345\206\262\345\210\273\345\272\246:", Q_NULLPTR));
        pushButton_qd->setText(QApplication::translate("ConfigDialog", "\347\241\256\345\256\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("ConfigDialog", "TextLabel", Q_NULLPTR));
        label_4->setText(QApplication::translate("ConfigDialog", "\351\200\211\346\213\251\347\224\265\346\242\257:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
