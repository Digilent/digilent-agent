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

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    launchWflAction = new QAction(tr("Launch WaveForms Live"), this);
    connect(launchWflAction, &QAction::triggered, this, &launchWfl);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);    
    trayIconMenu->addAction(launchWflAction);
    trayIconMenu->addAction(versionAction);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::launchWfl(){
    agent->launchWfl();
}

//Minimize to system tray on close
/*
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
*/
#endif //QT_NO_SYSTEMTRAYICON

