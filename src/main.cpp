
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>

#include "httplistener.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include "mainwindow.h"

//Forward declarations
QString searchConfigFile();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load the http configuration file
    QString configFileName = searchConfigFile();
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &app);

    //Create and start the HTTP Server
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new HttpRequestHandler(&app), &app);


    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

//Search for the HTTP server config ini
QString searchConfigFile() {
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/config.ini");
    if (!file.exists()) {
        file.setFileName(binDir+"/../config.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../"+appName+"/config.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../../"+appName+"/config.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../../../../"+appName+"/config.ini");
                    if (!file.exists()) {
                        file.setFileName(QDir::rootPath()+"config.ini");
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString configFileName = QDir(file.fileName()).canonicalPath();
        qDebug("Using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("Config file not found.");
    }
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text("QSystemTrayIcon is not supported on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->show();
    qDebug() << text;

    app.exec();
}

#endif
