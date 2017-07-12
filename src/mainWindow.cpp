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

MainWindow::MainWindow(Agent* agent, QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow::MainWindow()" << "thread: " << QThread::currentThread();

    setWindowTitle(tr("Digilent Agent"));

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

    //Set dock icon if on mac
    #if defined(TARGET_OS_MAC)
        QApplication::setWindowIcon(QIcon(":/images/icon.png"));
    #endif
    

    //Show system tray icon
    trayIcon->show();
        

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWindowActions()
{
#ifdef AGENT_BUILD_NOTE
    versionAction = new QAction(QString("Agent Version: ") + QString(this->agent->getVersion()) + " - " + this->agent->getBuildNote(), this);
#else
    versionAction = new QAction(QString("Agent Version: ") + QString(this->agent->getVersion()), this);
#endif
    versionAction->setEnabled(false);

#ifdef _WIN32
    checkForUpdatesAction = new QAction(QString("Check For Updates"), this);
    checkForUpdatesAction->setEnabled(true);
    connect(checkForUpdatesAction, &QAction::triggered, this, &MainWindow::runUpdater);
#endif

    //Active Device
    //activeDeviceNameAction = new QAction(QString("Active Device:  -"), this);
    //activeDeviceNameAction->setEnabled(false);
    connect(this->agent, SIGNAL(activeDeviceChanged(QString)), this, SLOT(onActiveDeviceNameChange(QString)));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    launchWflAction = new QAction(tr("Launch WaveForms Live"), this);
    connect(launchWflAction, &QAction::triggered, this, &MainWindow::launchWfl);
    
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);    
    trayIconMenu->addAction(launchWflAction);

    //Active Device    
    this->activeDeviceSubMenu = trayIconMenu->addMenu("Active Device:  -");
    this->activeDeviceSubMenuRelease = activeDeviceSubMenu->addAction("Release");
    connect(activeDeviceSubMenuRelease, SIGNAL(triggered()), this, SLOT(releaseActiveDevice()));
    activeDeviceSubMenu->setEnabled(false);
    trayIconMenu->addSeparator();
    
    trayIconMenu->addAction(versionAction);
    
    #ifdef _WIN32
        //Add check for updates button on Windows only since it depends on the maintainence tool
        trayIconMenu->addAction(checkForUpdatesAction);
    #endif

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

void MainWindow::releaseActiveDevice() {
    if(this->agent->activeDevice != 0)
    {
        emit releaseActiveDeviceSignal();
    }
    //this->agent->releaseActiveDevice();
    //Signal to relase agent
}

bool MainWindow::checkForUpdates(){
    //QString maintenanceToolPath = qApp->applicationDirPath() + "/maintenancetool";
    QString maintenanceToolPath = "C:/Program Files (x86)/Digilent/Agent/maintenancetool.exe";

    this->runUpdater();

    QProcess checkForUpdates;
    //checkForUpdates.start("maintenancetool --checkupdates");
    //checkForUpdates.start("C:/Users/samkr/Documents/My Received Files/test.bat");



    //Wait for update check to complete
    checkForUpdates.waitForFinished();

    if(checkForUpdates.error() != QProcess::UnknownError)
    {
        qDebug() << "Update check failed" << checkForUpdates.error();
        return false;
    }

    QByteArray updateData = checkForUpdates.readAllStandardOutput();

    if(updateData.isEmpty())
    {
        qDebug() << "No updates";
        return false;
    }

    //Update is available
    return true;
}

void MainWindow::runUpdater() {

    QStringList args("--updater");
    QProcess::startDetached(QCoreApplication::applicationDirPath() + "/maintenancetool.exe", args);

    // Close the agent so update can run
    qApp->closeAllWindows();
}

#endif //QT_NO_SYSTEMTRAYICON

