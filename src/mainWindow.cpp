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
    //hostnameIp = ui->hostnameIp;

    createWindowActions();
    createTrayIcon();

    //Set Tray Icon
    QIcon icon = QIcon(":/images/icon.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    //Show system tray icon
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWindowActions()
{
    versionAction = new QAction(QString("Agent Version: ") + QString(this->agent->getVersion()), this);
    versionAction->setEnabled(false);

    //Active Device
    //activeDeviceNameAction = new QAction(QString("Active Device:  -"), this);
    //activeDeviceNameAction->setEnabled(false);
    connect(this->agent, SIGNAL(activeDeviceChanged(QString)), this, SLOT(onActiveDeviceNameChange(QString)));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    launchWflAction = new QAction(tr("Launch WaveForms Live"), this);
    connect(launchWflAction, &QAction::triggered, this, &launchWfl);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);    
    trayIconMenu->addAction(launchWflAction);

    //Active Device    
    this->activeDeviceSubMenu = trayIconMenu->addMenu("Active Device:  -");
    this->activeDeviceSubMenuRelease = activeDeviceSubMenu->addAction("Release");
    connect(activeDeviceSubMenuRelease, &QAction::triggered, agent, &Agent::releaseActiveDevice);
    activeDeviceSubMenu->setEnabled(false);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(versionAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::launchWfl(){
    agent->launchWfl();
}

void MainWindow::onActiveDeviceNameChange(QString activeDeviceName) {
    qDebug("onActiveDeviceNameChange()");
    if(activeDeviceName == "")
    {
        activeDeviceSubMenu->setTitle("Active Device:  -");
        activeDeviceSubMenu->setEnabled(false);
    }
    else
    {
        activeDeviceSubMenu->setTitle("Active Device: " + activeDeviceName);
        activeDeviceSubMenu->setEnabled(true);
    }
}

#endif //QT_NO_SYSTEMTRAYICON

