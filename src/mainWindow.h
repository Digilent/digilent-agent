#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//HTTP core includes
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>

//OpenScope device includes
#include "osDevice/osDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>
#include "httpClient/httpClient.h"
#include "uartClient/uartInfo.h"

#define MAX_DEVICE_COUNT 16


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
    OsDevice *devices[MAX_DEVICE_COUNT];
    OsDevice *activeDevice;
    QComboBox *activeDeviceDropDown;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:   
    void onSelectedDeviceComboChange(int);
    void refreshDeviceList();
    void onConnectReleased();

private:
    //UI Elements
    Ui::MainWindow *ui;
    QLineEdit *hostnameIp;
    QPushButton *connectBtn;
    QPushButton *refreshDeviceListBtn;

    //System Tray
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    void createWindowActions();
    void createTrayIcon();

    //UART
    UartInfo *uartInfo;
};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
