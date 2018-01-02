/********************************************************************************
** Form generated from reading UI file 'custom.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOM_H
#define UI_CUSTOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_custom
{
public:
    QLabel *label_7;
    QListWidget *listWidget_zdy;
    QPushButton *pushButton_zdy_ks;
    QComboBox *comboBox;
    QLineEdit *lineEdit_zdy_yxzq;

    void setupUi(QDialog *custom)
    {
        if (custom->objectName().isEmpty())
            custom->setObjectName(QStringLiteral("custom"));
        custom->resize(435, 473);
        custom->setMinimumSize(QSize(435, 473));
        custom->setMaximumSize(QSize(435, 473));
        label_7 = new QLabel(custom);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(91, 361, 68, 16));
        label_7->setAlignment(Qt::AlignCenter);
        listWidget_zdy = new QListWidget(custom);
        listWidget_zdy->setObjectName(QStringLiteral("listWidget_zdy"));
        listWidget_zdy->setGeometry(QRect(11, 11, 411, 311));
        pushButton_zdy_ks = new QPushButton(custom);
        pushButton_zdy_ks->setObjectName(QStringLiteral("pushButton_zdy_ks"));
        pushButton_zdy_ks->setGeometry(QRect(160, 420, 121, 28));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_zdy_ks->sizePolicy().hasHeightForWidth());
        pushButton_zdy_ks->setSizePolicy(sizePolicy);
        comboBox = new QComboBox(custom);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(290, 360, 87, 22));
        lineEdit_zdy_yxzq = new QLineEdit(custom);
        lineEdit_zdy_yxzq->setObjectName(QStringLiteral("lineEdit_zdy_yxzq"));
        lineEdit_zdy_yxzq->setGeometry(QRect(167, 361, 111, 21));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_zdy_yxzq->sizePolicy().hasHeightForWidth());
        lineEdit_zdy_yxzq->setSizePolicy(sizePolicy1);
        lineEdit_zdy_yxzq->setEchoMode(QLineEdit::Normal);

        retranslateUi(custom);

        QMetaObject::connectSlotsByName(custom);
    } // setupUi

    void retranslateUi(QDialog *custom)
    {
        custom->setWindowTitle(QApplication::translate("custom", "Dialog", Q_NULLPTR));
        label_7->setText(QApplication::translate("custom", "\350\277\220\350\241\214\345\221\250\346\234\237:", Q_NULLPTR));
        pushButton_zdy_ks->setText(QApplication::translate("custom", "\345\274\200\345\247\213", Q_NULLPTR));
        comboBox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class custom: public Ui_custom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOM_H
