#include "mainWindow.h"
#include "ui_mainWindow.h"

//OpenScope device includes
#include "osDevice/osDevice.h"
#include "osDevice/osHttpDevice.h"
#include "osDevice/osUartDevice.h"

//
#include "uartClient/uartInfo.h"

//TEST UART CODE
#include "uartClient/uartClient.h"

#ifndef QT_NO_SYSTEMTRAYICON

//HTTP core includes
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));

    //Get UI element refs
    ui->setupUi(this);
    httpAddDevice = ui->httpAddDevice;
    httpAddress = ui->httpAddress;
    uartAddDevice = ui->uartAddDevice;
    uartAddress = ui->uartAddress;

    deviceDropDown = ui->deviceDropDown;

    //Create devices
    devicesHead = 0;
    activeDevice = 0;

    //UART
    uartInfo = new UartInfo();
    uartInfo->refreshPortInfo();

    //Add UARTS to device drop down.
    for(int i=0; i<uartInfo->ports.count(); i++)
    {
        if(!uartInfo->ports[i].isBusy())
        {
            QString portName = uartInfo->ports[i].portName();
            devices[devicesHead] = new OsUartDevice(portName);
            devices[devicesHead]->name = portName;
            deviceDropDown->addItem(QIcon(), portName, QVariant());
            devicesHead++;
        }
    }

    //UI Actions
    connect(httpAddDevice, SIGNAL(released()), this, SLOT(onHttpAddDeviceRelease()));
    connect(uartAddDevice, SIGNAL(released()), this, SLOT(onUartAddDeviceRelease()));
    connect(deviceDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceDropDownCurrentIndexChanged(int)));

    createWindowActions();
    createTrayIcon();

    //Set Tray Icon
    QIcon icon = QIcon(":/images/icon.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);   

    //requestShowPage();
    trayIcon->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWindowActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::onHttpAddDeviceRelease() {
    qDebug() << "Adding HTTP Device: " << httpAddress->text();

    devices[devicesHead] = new OsHttpDevice(QUrl(httpAddress->text()));
    devices[devicesHead]->name = httpAddress->text();
    deviceDropDown->addItem(QIcon(), httpAddress->text(), QVariant());
    devicesHead++;
}

void MainWindow::onUartAddDeviceRelease() {
    qDebug() << "Adding UART Device: " << uartAddress->text();

    devices[devicesHead] = new OsUartDevice(uartAddress->text());
    devices[devicesHead]->name = uartAddress->text();
    deviceDropDown->addItem(QIcon(), uartAddress->text(), QVariant());
    devicesHead++;
}

void MainWindow::onDeviceDropDownCurrentIndexChanged(int index) {
    activeDevice = devices[index];
    if(activeDevice != 0) {
        qDebug() << "Name: " << activeDevice->name;

    }
}

//Minimize to system tray on close
void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

#endif //QT_NO_SYSTEMTRAYICON

