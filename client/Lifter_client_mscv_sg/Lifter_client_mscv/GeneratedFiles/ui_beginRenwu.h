/********************************************************************************
** Form generated from reading UI file 'beginRenwu.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEGINRENWU_H
#define UI_BEGINRENWU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_beginRenwu
{
public:

    void setupUi(QWidget *beginRenwu)
    {
        if (beginRenwu->objectName().isEmpty())
            beginRenwu->setObjectName(QStringLiteral("beginRenwu"));
        beginRenwu->resize(400, 300);

        retranslateUi(beginRenwu);

        QMetaObject::connectSlotsByName(beginRenwu);
    } // setupUi

    void retranslateUi(QWidget *beginRenwu)
    {
        beginRenwu->setWindowTitle(QApplication::translate("beginRenwu", "beginRenwu", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class beginRenwu: public Ui_beginRenwu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEGINRENWU_H
