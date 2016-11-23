#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//HTTP core includes
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>
#include <QEvent>
#include <QLabel>

//OpenScope device includes
#include "osDevice/osDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>
#include "httpClient/httpClient.h"
#include "uartClient/uartInfo.h"
#include "core/comboBoxEventFilter.h"

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
    QComboBox *deviceDropDown;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;    

private slots:   
    void onDeviceDropDownChange(int);
    void refreshDeviceList();
    void onConnectReleased();

private:
    //UI Elements
    Ui::MainWindow *ui;
    QLineEdit *hostnameIp;
    QLabel *activeDeviceName;
    QPushButton *connectBtn;
    QPushButton *refreshDeviceListBtn;

    //Event Filters
    ComboBoxEventFilter* comboBoxEventFilter;

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
