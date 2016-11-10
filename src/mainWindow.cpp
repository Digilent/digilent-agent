#include "mainWindow.h"
#include "ui_mainWindow.h"


//OpenScope device includes
#include "osDevice/osDevice.h"
#include "osDevice/osHttpDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON

//HTTP core includes
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));

    //Get UI element refs
    ui->setupUi(this);
    httpSetIp = ui->httpSetIp;
    httpIp = ui->httpIp;

    //Create devices
    deviceOne = new OsHttpDevice(QUrl("http://feeds.labviewmakerhub.com"));
    deviceTwo = new OsHttpDevice(QUrl("http://www.google.com"));
    activeDevice = deviceOne;

    //UI Actions
    connect(httpSetIp, SIGNAL(released()), this, SLOT(httpSetIpOnRelease()));

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

void MainWindow::httpSetIpOnRelease() {   
    activeDevice->name = httpIp->text();
    qDebug() << activeDevice->name;
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
