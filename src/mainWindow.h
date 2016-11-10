#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//HTTP core includes
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>
#include "httpClient/httpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString activeDevice;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void httpSetIpOnRelease();    

private:
    //UI Elements
    Ui::MainWindow *ui;
    QLineEdit *httpIp;
    QPushButton *httpSetIp;


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
