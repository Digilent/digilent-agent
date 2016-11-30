//Disable SSL
//#define QT_NO_SSL

//QT core includes
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>

//HTTP core includes
#include "httplistener.h"

//WFL Agent HTTP includes
#include "httpServer/httpRouter.h"
#include "httpServer/debugController.h"

//WFL Agent includes
#include "core/agent.h"
#include "wflDevice/wflDevice.h"
#include "wflDevice/wflHttpDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include "mainWindow.h"

//Forward declarations
QString searchConfigFile();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Instantiate the agent and mainWindow
    Agent* agent = new Agent();
    MainWindow mainWindow(agent);

    // Load the http configuration file

    QString configFileName = searchConfigFile();
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &app);

        /*
    if(configFileName != "") {

    } else  {
        listenerSettings = new QSettings();
        listenerSettings->beginGroup("listener");
        listenerSettings->setValue("port", 56089);
        listenerSettings->setValue("minThreads", 4);
        listenerSettings->setValue("maxThreads", 100);
        listenerSettings->setValue("cleanupInterval", 60000);
        listenerSettings->setValue("readTimeout", 60000);
        listenerSettings->setValue("maxRequestSize", 16000);
        listenerSettings->setValue("maxMultiPartSize", 10000000);
        listenerSettings->endGroup();

        listenerSettings->beginGroup("files");
        listenerSettings->setValue("path", "./www");
        listenerSettings->setValue("encoding", "UTF-8");
        listenerSettings->setValue("maxAge", 90000);
        listenerSettings->setValue("cacheTime", 60000);
        listenerSettings->setValue("cacheSize", 1000000);
        listenerSettings->setValue("maxCachedFileSize", 65536);
        listenerSettings->endGroup();
    }
    */

    // Static file controller
    QSettings* fileSettings=new QSettings(configFileName, QSettings::IniFormat, &app);
    fileSettings->beginGroup("files");
    HttpRouter::staticFileController = new StaticFileController(fileSettings,&app);

    //Create and start the HTTP Server
    listenerSettings->beginGroup("listener");
    //new HttpListener(listenerSettings, new HttpRouter(&app, &mainWindow.activeDevice), &app);
    new HttpListener(listenerSettings, new HttpRouter(agent, &app), &app);

    mainWindow.hide();

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
                    file.setFileName(binDir+"/../../../"+appName+"/config.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../"+appName+"/config.ini");
                        if (!file.exists()) {
                            file.setFileName(binDir+"/../../../../../"+appName+"/config.ini");
                            if (!file.exists()) {
                                file.setFileName(QDir::rootPath()+"config.ini");
                            }
                        }
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
        return QString();
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
