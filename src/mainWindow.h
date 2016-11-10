#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QSystemTrayIcon>

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

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    //void requestReceived(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    void createActions();
    void createTrayIcon();

    HttpClient* httpClient;

    //void requestShowPage();
};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
