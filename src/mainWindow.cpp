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
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));

    //Get UI element refs
    ui->setupUi(this);
    httpSetIp = ui->httpSetIp;
    httpIp = ui->httpIp;
    deviceDropDown = ui->deviceDropDown;

    //Create devices
    devicesHead = 0;
    activeDevice = 0;

    //UI Actions
    connect(httpSetIp, SIGNAL(released()), this, SLOT(onHttpSetIpRelease()));
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

void MainWindow::onHttpSetIpRelease() {
    qDebug() << "Adding Device: " << httpIp->text();

    devices[devicesHead] = new OsHttpDevice(QUrl(httpIp->text()));
    devices[devicesHead]->name = httpIp->text();
    deviceDropDown->addItem(QIcon(), httpIp->text(), QVariant());
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

