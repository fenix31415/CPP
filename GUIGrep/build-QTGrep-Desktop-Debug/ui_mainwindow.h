/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QTableWidget *table_result;
    QPushButton *button_search;
    QLineEdit *lineEdit_2;
    QPushButton *button_cancel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit_fileName;
    QLineEdit *lineEdit_String;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(516, 473);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        table_result = new QTableWidget(centralWidget);
        if (table_result->columnCount() < 3)
            table_result->setColumnCount(3);
        table_result->setObjectName(QStringLiteral("table_result"));
        table_result->setColumnCount(3);
        table_result->horizontalHeader()->setCascadingSectionResizes(true);

        gridLayout_3->addWidget(table_result, 3, 0, 1, 4);

        button_search = new QPushButton(centralWidget);
        button_search->setObjectName(QStringLiteral("button_search"));

        gridLayout_3->addWidget(button_search, 1, 3, 1, 1);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit_2, 2, 0, 1, 1);

        button_cancel = new QPushButton(centralWidget);
        button_cancel->setObjectName(QStringLiteral("button_cancel"));

        gridLayout_3->addWidget(button_cancel, 2, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 1, 1, 1);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit, 1, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        lineEdit_fileName = new QLineEdit(centralWidget);
        lineEdit_fileName->setObjectName(QStringLiteral("lineEdit_fileName"));

        gridLayout_3->addWidget(lineEdit_fileName, 1, 1, 1, 2);

        lineEdit_String = new QLineEdit(centralWidget);
        lineEdit_String->setObjectName(QStringLiteral("lineEdit_String"));

        gridLayout_3->addWidget(lineEdit_String, 2, 1, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        button_search->setText(QApplication::translate("MainWindow", "Search", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("MainWindow", "String:", Q_NULLPTR));
        button_cancel->setText(QApplication::translate("MainWindow", "Cancel", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("MainWindow", "File:", Q_NULLPTR));
        lineEdit_fileName->setText(QApplication::translate("MainWindow", "/home/fenix31415/t/findfiles", Q_NULLPTR));
        lineEdit_String->setText(QApplication::translate("MainWindow", "bash", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
