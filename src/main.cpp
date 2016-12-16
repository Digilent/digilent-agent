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
#include "core/utils/runGuard.h"
#include "wflDevice/wflDevice.h"
#include "wflDevice/wflHttpDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include "mainWindow.h"

//Forward declarations
QString searchConfigFile();
QString createNewConfigFile();

int main(int argc, char *argv[])
{
    //Prevent multiple instances of this agent
    RunGuard guard( "WaveFroms_Live_Agent_1.x.x" );
       if ( !guard.tryToRun() )
           return 0;

    QApplication app(argc, argv);

    //Instantiate the agent and mainWindow
    Agent* agent = new Agent();
    MainWindow mainWindow(agent);

    // Load the http configuration file

    QString configFileName = searchConfigFile();
    QSettings* listenerSettings;


    if(configFileName != "") {
         listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &app);

    } else  {
        qDebug("No waveforms-live-agent.ini found, using defaults.");
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
#ifdef Q_OS_LINUX
        listenerSettings->setValue("path", "/usr/share/waveforms-live-agent/www");
#elif defined(W_OS_WIN32)
    listenerSettings->setValue("path", "./www");
#endif
        listenerSettings->setValue("encoding", "UTF-8");
        listenerSettings->setValue("maxAge", 90000);
        listenerSettings->setValue("cacheTime", 60000);
        listenerSettings->setValue("cacheSize", 1000000);
        listenerSettings->setValue("maxCachedFileSize", 65536);
        listenerSettings->endGroup();
    }


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
    QFile file;
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();

#ifdef Q_OS_LINUX
    file.setFileName(QDir::homePath() + "/.config/waveforms-live-agent.ini");
#elif defined(W_OS_WIN32)

#endif
    if (!file.exists()) {
        file.setFileName(binDir+"/waveforms-live-agent.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../waveforms-live-agent.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/waveforms-live-agent.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/waveforms-live-agent.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../"+appName+"/waveforms-live-agent.ini");
                        if (!file.exists()) {
                            file.setFileName(binDir+"/../../../../"+appName+"/waveforms-live-agent.ini");
                            if (!file.exists()) {
                                file.setFileName(binDir+"/../../../../../"+appName+"/waveforms-live-agent.ini");
                                if (!file.exists()) {
                                    file.setFileName(QDir::rootPath()+"waveforms-live-agent.ini");
                                    if (!file.exists()) {
                                        file.setFileName(createNewConfigFile());
                                    }
                                }
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

QString createNewConfigFile(){
#ifdef Q_OS_LINUX
    QString path = QDir::homePath() + "/.config/waveforms-live-agent.ini";
    QFile file(path);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream( &file );
        stream << "[listener]\n;host=0.0.0.0\nport=56089\nminThreads=4\nmaxThreads=100\ncleanupInterval=60000\nreadTimeout=60000\nmaxRequestSize=16000\nmaxMultiPartSize=10000000\n\n[files]\npath=./www\nencoding=UTF-8\nmaxAge=90000\ncacheTime=60000\ncacheSize=1000000\nmaxCachedFileSize=65536\n\n";
        file.close();
        return path;
    }
    return QString();

#elif defined(W_OS_WIN32)

#endif
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
