/********************************************************************************
** Form generated from reading UI file 'testCHART.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTCHART_H
#define UI_TESTCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testCHARTClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testCHARTClass)
    {
        if (testCHARTClass->objectName().isEmpty())
            testCHARTClass->setObjectName(QStringLiteral("testCHARTClass"));
        testCHARTClass->resize(600, 400);
        menuBar = new QMenuBar(testCHARTClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        testCHARTClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testCHARTClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        testCHARTClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(testCHARTClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        testCHARTClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(testCHARTClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        testCHARTClass->setStatusBar(statusBar);

        retranslateUi(testCHARTClass);

        QMetaObject::connectSlotsByName(testCHARTClass);
    } // setupUi

    void retranslateUi(QMainWindow *testCHARTClass)
    {
        testCHARTClass->setWindowTitle(QApplication::translate("testCHARTClass", "testCHART", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class testCHARTClass: public Ui_testCHARTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTCHART_H
