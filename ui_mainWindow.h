/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *activeDevice;
    QLabel *label_selectDevice;
    QComboBox *deviceDropDown;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *hostNameIp;
    QLabel *hostnameIpLabel;
    QLineEdit *hostnameIp;
    QPushButton *connectBtn;
    QPushButton *refreshDeviceListBtn;
    QLabel *activeDeviceLabel;
    QLabel *activeDeviceName;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(496, 437);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(53, 21, 104, 51));
        activeDevice = new QVBoxLayout(verticalLayoutWidget);
        activeDevice->setSpacing(6);
        activeDevice->setContentsMargins(11, 11, 11, 11);
        activeDevice->setObjectName(QStringLiteral("activeDevice"));
        activeDevice->setSizeConstraint(QLayout::SetMinimumSize);
        activeDevice->setContentsMargins(0, 0, 0, 0);
        label_selectDevice = new QLabel(verticalLayoutWidget);
        label_selectDevice->setObjectName(QStringLiteral("label_selectDevice"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_selectDevice->setFont(font);
        label_selectDevice->setAlignment(Qt::AlignCenter);

        activeDevice->addWidget(label_selectDevice);

        deviceDropDown = new QComboBox(verticalLayoutWidget);
        deviceDropDown->setObjectName(QStringLiteral("deviceDropDown"));

        activeDevice->addWidget(deviceDropDown);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(163, 25, 135, 46));
        hostNameIp = new QVBoxLayout(verticalLayoutWidget_2);
        hostNameIp->setSpacing(6);
        hostNameIp->setContentsMargins(11, 11, 11, 11);
        hostNameIp->setObjectName(QStringLiteral("hostNameIp"));
        hostNameIp->setContentsMargins(0, 0, 0, 0);
        hostnameIpLabel = new QLabel(verticalLayoutWidget_2);
        hostnameIpLabel->setObjectName(QStringLiteral("hostnameIpLabel"));
        hostnameIpLabel->setFont(font);
        hostnameIpLabel->setAlignment(Qt::AlignCenter);

        hostNameIp->addWidget(hostnameIpLabel);

        hostnameIp = new QLineEdit(verticalLayoutWidget_2);
        hostnameIp->setObjectName(QStringLiteral("hostnameIp"));

        hostNameIp->addWidget(hostnameIp);

        connectBtn = new QPushButton(centralWidget);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));
        connectBtn->setGeometry(QRect(83, 80, 75, 23));
        refreshDeviceListBtn = new QPushButton(centralWidget);
        refreshDeviceListBtn->setObjectName(QStringLiteral("refreshDeviceListBtn"));
        refreshDeviceListBtn->setGeometry(QRect(20, 50, 30, 23));
        QIcon icon;
        icon.addFile(QStringLiteral("images/refresh-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        refreshDeviceListBtn->setIcon(icon);
        activeDeviceLabel = new QLabel(centralWidget);
        activeDeviceLabel->setObjectName(QStringLiteral("activeDeviceLabel"));
        activeDeviceLabel->setGeometry(QRect(20, 140, 111, 16));
        activeDeviceLabel->setFont(font);
        activeDeviceName = new QLabel(centralWidget);
        activeDeviceName->setObjectName(QStringLiteral("activeDeviceName"));
        activeDeviceName->setGeometry(QRect(20, 160, 161, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 496, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_selectDevice->setText(QApplication::translate("MainWindow", "Devices", 0));
        hostnameIpLabel->setText(QApplication::translate("MainWindow", "Hostname or IP", 0));
        hostnameIp->setText(QApplication::translate("MainWindow", "http://192.168.1.15", 0));
        connectBtn->setText(QApplication::translate("MainWindow", "Connect", 0));
        refreshDeviceListBtn->setText(QString());
        activeDeviceLabel->setText(QApplication::translate("MainWindow", "Active Device", 0));
        activeDeviceName->setText(QApplication::translate("MainWindow", "No Active Device", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
