#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//HTTP core includes
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>

//OpenScope device includes
#include "osDevice/osDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>
#include "httpClient/httpClient.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Devices
    OsDevice* activeDevice;
    OsDevice* deviceOne;
    OsDevice* deviceTwo;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void httpSetIpOnRelease();

private:
    //UI Elements
    Ui::MainWindow *ui;
    QLineEdit *httpIp;
    QPushButton *httpSetIp;

    //System Tray
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    void createWindowActions();
    void createTrayIcon();


};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
