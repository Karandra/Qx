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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *CentralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTextEdit *ClientLog;
    QPushButton *ClientConnectButton;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QTextEdit *ServerLog;
    QPushButton *ServerSendButton;

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
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(CentralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        ClientLog = new QTextEdit(CentralWidget);
        ClientLog->setObjectName(QString::fromUtf8("ClientLog"));
        ClientLog->setReadOnly(true);

        verticalLayout_2->addWidget(ClientLog);

        ClientConnectButton = new QPushButton(CentralWidget);
        ClientConnectButton->setObjectName(QString::fromUtf8("ClientConnectButton"));

        verticalLayout_2->addWidget(ClientConnectButton);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(CentralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        ServerLog = new QTextEdit(CentralWidget);
        ServerLog->setObjectName(QString::fromUtf8("ServerLog"));
        ServerLog->setReadOnly(true);

        verticalLayout->addWidget(ServerLog);

        ServerSendButton = new QPushButton(CentralWidget);
        ServerSendButton->setObjectName(QString::fromUtf8("ServerSendButton"));

        verticalLayout->addWidget(ServerSendButton);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(CentralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TCP", nullptr));
        label->setText(QApplication::translate("MainWindow", "Client:", nullptr));
        ClientConnectButton->setText(QApplication::translate("MainWindow", "Connect to Server", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Server:", nullptr));
        ServerSendButton->setText(QApplication::translate("MainWindow", "Send to client", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
