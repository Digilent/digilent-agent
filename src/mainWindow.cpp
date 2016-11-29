#include "mainWindow.h"
#include "ui_mainWindow.h"

#ifndef QT_NO_SYSTEMTRAYICON

//QT core includes
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QComboBox>
#include <QEvent>

#include "wflDevice/wflDevice.h"
#include "wflDevice/wflhttpdevice.h"


MainWindow::MainWindow(Agent* agent, QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));

    this->agent = agent;

    //Get UI element refs
    ui->setupUi(this);    
    hostnameIp = ui->hostnameIp;
    connectBtn = ui->connectBtn;
    activeDeviceName = ui->activeDeviceName;
    refreshDeviceListBtn = ui->refreshDeviceListBtn;
    deviceDropDown = ui->deviceDropDown;

    comboBoxEventFilter = new ComboBoxEventFilter(this);
    //deviceDropDown->installEventFilter(comboBoxEventFilter);

    //connect(comboBoxEventFilter, SIGNAL(mouseClick()), this, SLOT(refreshDeviceList()));

    //UI Actions
    //connect(deviceDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceDropDownChange(int)));
    //connect(refreshDeviceListBtn, SIGNAL(released()), this, SLOT(refreshDeviceList()));
    //connect(connectBtn, SIGNAL(released()), this, SLOT(onConnectReleased()));

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


/*
void MainWindow::refreshDeviceList(){
    qDebug("Refreshing Device List");

    //Clear dropdown items
    deviceDropDown->clear();

    //Deallocate previous osDevice objects
    agent->flushDevices();

    //Refresh UART device info
    agent->enumerateDevices();
}

void MainWindow::onDeviceDropDownChange(int index) {
    qDebug("MainWindow::onActiveDeviceDropDownSelectionChanged()");
    if(agent->devices[deviceDropDown->currentIndex()] != 0)
    {
        WflHttpDevice* httpDevPtr = dynamic_cast<WflHttpDevice*>(agent->devices[deviceDropDown->currentIndex()]);
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
    agent->activeDevice = agent->devices[deviceDropDown->currentIndex()];

    activeDeviceName->setText(agent->activeDevice->name);

    //If device is an (the) HTTP device set it's url to the value in the hostname / ip box
    WflHttpDevice* httpDevPtr = dynamic_cast<WflHttpDevice*>(agent->devices[deviceDropDown->currentIndex()]);
    if(httpDevPtr != nullptr)
    {
        httpDevPtr->setUrl(QUrl(hostnameIp->text()));
    }
}
*/


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

