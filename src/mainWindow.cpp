#include "mainWindow.h"
#include "ui_mainwindow.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>




MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowTitle(tr("OpenScope Utility"));
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    //Set Tray Icon
    QIcon icon = QIcon(":/images/icon.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    httpClient = new HttpClient(this);
    httpClient->get("http://www.labviewmakerhub.com/feeds");

    //requestShowPage();
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
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




/*
void MainWindow::requestShowPage(){
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QString mes = QString("Hello World!");
    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://www.google.com/")));
    //manager->get(QNetworkRequest(QUrl("http://feeds.labviewmakerhub.com/")));
}


void MainWindow::requestReceived(QNetworkReply *reply)
{
    reply->deleteLater();

    if(reply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
             // Here we got the final reply
            QByteArray replyText = reply->readAll();
            QString sData = QString(replyText);
            //QString replyBA = replyText.toHtmlEscaped();
            QString test5 = "TEST5555555555555555";

            qDebug("Before");
            //const char* data = replyBA.data();
            char test[3] = {'a', 'b', 'c'};
            char* test2 = "TestTwo";
            char* test3 = (char*)replyText.data_ptr();
            //char* test4 = (char*)appendPlainText(replyText)
            qDebug() << "test output" << sData;

            ui->txt_debug->appendPlainText(sData);
             qDebug(replyText);
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative,
            // we have to use the previous one to resolve it
            newUrl = reply->url().resolved(newUrl);

            QNetworkAccessManager *manager = reply->manager();
            QNetworkRequest redirection(newUrl);
            QNetworkReply *newReply = manager->get(redirection);

            return; // to keep the manager for the next request
        }
    }
    else
    {
        // Error
        qDebug(qPrintable(reply->errorString()));
        //ui->txt_debug->appendPlainText(reply->errorString());
    }

    reply->manager()->deleteLater();
}
*/


#endif //QT_NO_SYSTEMTRAYICON
