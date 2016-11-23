#include "mainWindow.h"
#include "ui_mainWindow.h"

//OpenScope device includes
#include "osDevice/osDevice.h"
#include "osDevice/osHttpDevice.h"
#include "osDevice/osUartDevice.h"

#include "uartClient/uartInfo.h"

#ifndef QT_NO_SYSTEMTRAYICON

//HTTP core includes
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QComboBox>
#include <QEvent>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));

    //Get UI element refs
    ui->setupUi(this);    
    hostnameIp = ui->hostnameIp;
    connectBtn = ui->connectBtn;
    activeDeviceName = ui->activeDeviceName;
    refreshDeviceListBtn = ui->refreshDeviceListBtn;
    deviceDropDown = ui->deviceDropDown;

    comboBoxEventFilter = new ComboBoxEventFilter(this);
    //deviceDropDown->installEventFilter(comboBoxEventFilter);

    connect(comboBoxEventFilter, SIGNAL(mouseClick()), this, SLOT(refreshDeviceList()));


    //Create devices
    for(int i=0; i<MAX_DEVICE_COUNT; i++)
    {
        devices[i] = 0;
    }
    activeDevice = 0;

    //UART
    uartInfo = new UartInfo();

    refreshDeviceList();

    //UI Actions
    connect(deviceDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceDropDownChange(int)));
    connect(refreshDeviceListBtn, SIGNAL(released()), this, SLOT(refreshDeviceList()));
    connect(connectBtn, SIGNAL(released()), this, SLOT(onConnectReleased()));


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

void MainWindow::refreshDeviceList(){
    qDebug("Refreshing Device List");
    //Clear previous device list
    //delete[] devices;

    //Clear dropdown items
    deviceDropDown->clear();

    //Deallocate previous osDevice objects
    for(int i=0; i<MAX_DEVICE_COUNT; i++)
    {
        if(devices[i] != 0)
        {
            delete devices[i];
            devices[i] = 0;
        }
    }

    //Refresh UART device info
    uartInfo->refreshPortInfo();
    int httpIndex = uartInfo->ports.count();

    //Add UARTS to device drop down.
    for(int i=0; i<uartInfo->ports.count(); i++)
    {
        if(!uartInfo->ports[i].isBusy())
        {
            QString portName = uartInfo->ports[i].portName();
            devices[i] = new OsUartDevice(portName);
            devices[i]->name = portName;
            deviceDropDown->addItem(QIcon(), portName, QVariant());
        }
    }
    //Add HTTP option
    devices[httpIndex] = new OsHttpDevice();
    devices[httpIndex]->name = "HTTP";
    deviceDropDown->addItem(QIcon(), "HTTP", QVariant());
    if(deviceDropDown->currentIndex() == httpIndex)
    {
        hostnameIp->setEnabled(true);
    }
    else
    {
        hostnameIp->setEnabled(false);
    }
}

void MainWindow::onDeviceDropDownChange(int index) {
    qDebug("MainWindow::onActiveDeviceDropDownSelectionChanged()");
    if(devices[deviceDropDown->currentIndex()] != 0)
    {
        OsHttpDevice* httpDevPtr = dynamic_cast<OsHttpDevice*>(devices[deviceDropDown->currentIndex()]);
        if(httpDevPtr != nullptr) {
            //HTTP device
            hostnameIp->setEnabled(true);

        } else {
            //Non-HTTP device
            hostnameIp->setEnabled(false);
        }
    }
}

void MainWindow::onConnectReleased() {
    qDebug("MainWindow::onConnectReleased()");
    int devIndex = deviceDropDown->currentIndex();
    activeDevice = devices[deviceDropDown->currentIndex()];

    activeDeviceName->setText(activeDevice->name);

    //If device is an (the) HTTP device set it's url to the value in the hostname / ip box
    OsHttpDevice* httpDevPtr = dynamic_cast<OsHttpDevice*>(devices[deviceDropDown->currentIndex()]);
    if(httpDevPtr != nullptr)
    {
        httpDevPtr->setUrl(QUrl(hostnameIp->text()));
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

