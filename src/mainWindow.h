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

//WFL Agent Core
#include "core/agent.h"
#include "core/comboBoxEventFilter.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Agent* agent, QWidget *parent = 0);
    ~MainWindow();

    Agent *agent;   
    QComboBox *deviceDropDown;

protected:
    //void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
     void releaseActiveDeviceSignal();

private slots:   
    void onActiveDeviceNameChange(QString activeDeviceName);
    void releaseActiveDevice();
    //void refreshDeviceList();
    //void onConnectReleased();

private:
    //UI Elements
    Ui::MainWindow *ui;


    //System Tray
    QMenu *trayIconMenu;
    QMenu* activeDeviceSubMenu;

    QAction *launchWflAction;
    QAction *checkForUpdatesAction;

    QAction *activeDeviceNameAction;
    QAction *activeDeviceSubMenuRelease;

    QAction *versionAction;
    QAction *quitAction;


    QSystemTrayIcon *trayIcon;



    void createWindowActions();
    void createTrayIcon();
    void launchWfl();
    bool checkForUpdates();
    void runUpdater();
};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
