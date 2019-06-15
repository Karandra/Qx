/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *CentralWidget;
    QGridLayout *gridLayout;
    QTabWidget *TabWidget;
    QWidget *ServerTab;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *ServerLog;
    QPushButton *ServerSendButton;
    QWidget *ClientTab;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *ClientLog;
    QPushButton *ClientConnectButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        CentralWidget = new QWidget(MainWindow);
        CentralWidget->setObjectName(QString::fromUtf8("CentralWidget"));
        CentralWidget->setMinimumSize(QSize(640, 480));
        gridLayout = new QGridLayout(CentralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        TabWidget = new QTabWidget(CentralWidget);
        TabWidget->setObjectName(QString::fromUtf8("TabWidget"));
        TabWidget->setEnabled(true);
        ServerTab = new QWidget();
        ServerTab->setObjectName(QString::fromUtf8("ServerTab"));
        gridLayout_2 = new QGridLayout(ServerTab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ServerLog = new QTextEdit(ServerTab);
        ServerLog->setObjectName(QString::fromUtf8("ServerLog"));
        ServerLog->setReadOnly(true);

        verticalLayout->addWidget(ServerLog);

        ServerSendButton = new QPushButton(ServerTab);
        ServerSendButton->setObjectName(QString::fromUtf8("ServerSendButton"));

        verticalLayout->addWidget(ServerSendButton);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        TabWidget->addTab(ServerTab, QString());
        ClientTab = new QWidget();
        ClientTab->setObjectName(QString::fromUtf8("ClientTab"));
        gridLayout_3 = new QGridLayout(ClientTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ClientLog = new QTextEdit(ClientTab);
        ClientLog->setObjectName(QString::fromUtf8("ClientLog"));
        ClientLog->setReadOnly(true);

        verticalLayout_2->addWidget(ClientLog);

        ClientConnectButton = new QPushButton(ClientTab);
        ClientConnectButton->setObjectName(QString::fromUtf8("ClientConnectButton"));

        verticalLayout_2->addWidget(ClientConnectButton);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);

        TabWidget->addTab(ClientTab, QString());

        gridLayout->addWidget(TabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(CentralWidget);

        retranslateUi(MainWindow);

        TabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "UDP", nullptr));
        ServerSendButton->setText(QApplication::translate("MainWindow", "Send", nullptr));
        TabWidget->setTabText(TabWidget->indexOf(ServerTab), QApplication::translate("MainWindow", "Server", nullptr));
        ClientConnectButton->setText(QApplication::translate("MainWindow", "Connect to Server", nullptr));
        TabWidget->setTabText(TabWidget->indexOf(ClientTab), QApplication::translate("MainWindow", "Client", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
