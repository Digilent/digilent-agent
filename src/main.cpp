//QT core includes
#include <QtGlobal>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>

//Disable SSL on OSx
#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        #define QT_NO_SSL
    #endif
#endif

//HTTP core includes
#include "httplistener.h"

//WFL Agent HTTP includes
#include "httpServer/httpRouter.h"
#include "httpServer/debugController.h"

//WFL Agent includes
#include "core/agent.h"
#include "core/utils/runGuard.h"
#include "wflDevice/wflDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include "mainWindow.h"

//Forward declarations
QString searchConfigFile();
QString createNewConfigFile();


#ifdef Q_OS_LINUX
    QString iniPath = QString(QDir::homePath() + "/.config/digilent/digilent-agent.ini");
    QString wwwRoot = "/usr/share/digilent/digilent-agent/www";
#elif defined(_WIN32)
    QString iniPath = QString(QDir::homePath() + "/AppData/Local/Digilent/Digilent Agent/digilent-agent.ini");
    QString wwwRoot = QString(QDir::homePath() + "/AppData/Local/Digilent/Digilent Agent/www/");
#elif TARGET_OS_MAC
    QString iniPath = QString(QDir::homePath() + "/.config/digilent/digilent-agent.ini");
    QString wwwRoot = "/Users/Shared/digilent/digilent-agent/www";
#endif

int main(int argc, char *argv[])
{
    //Prevent multiple instances of this agent
    RunGuard guard( "Digilent_Agent_1.x.x" );
       if ( !guard.tryToRun() ) {
           return 0;
       }

    QApplication app(argc, argv);    

    //Instantiate the agent and mainWindow   
    Agent* agent = new Agent(&app);
    
    MainWindow mainWindow(agent);
    

    QObject::connect(&mainWindow, SIGNAL(releaseActiveDeviceSignal()), agent, SLOT(releaseActiveDevice()));
            //, SIGNAL(finished()), &loop, SLOT(quit()));


    // Load the http configuration file
    QString configFileName = searchConfigFile();
    QSettings* listenerSettings;


    if(configFileName != "") {
         listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &app);
         listenerSettings->sync();
    } else  {
        qDebug("Failed to load digilent-agent.ini");
        return -1;
    }

    //Pass ini values to agent
    agent->waveFormsLiveBrowserPath = listenerSettings->value("waveforms-live/wwwRoot").toString();
    
    // Static file controller
    QSettings* fileSettings=new QSettings(configFileName, QSettings::IniFormat, &app);
    fileSettings->beginGroup("files");
    fileSettings->setValue("path", agent->waveFormsLiveBrowserPath);
    HttpRouter::staticFileController = new StaticFileController(fileSettings, &app);
    
    //Create and start the HTTP Server  
    //new HttpListener(listenerSettings, new HttpRouter(&app, &mainWindow.activeDevice), &app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new HttpRouter(agent, &app), &app);
 
    mainWindow.hide();

    return app.exec();
}

//Search for the digilent-agent config ini
QString searchConfigFile() {
    QFile file;
    file.setFileName(iniPath);
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();

#ifdef Q_OS_LINUX_LINUX
    file.setFileName(QDir::homePath() + "/.config/digilent-agent.ini");
#elif defined(W_OS_WIN32)

#endif
    if (!file.exists()) {
        file.setFileName(binDir+"/digilent-agent.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../digilent-agent.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/digilent-agent.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/digilent-agent.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../"+appName+"/digilent-agent.ini");
                        if (!file.exists()) {
                            file.setFileName(binDir+"/../../../../"+appName+"/digilent-agent.ini");
                            if (!file.exists()) {
                                file.setFileName(binDir+"/../../../../../"+appName+"/digilent-agent.ini");
                                if (!file.exists()) {
                                    file.setFileName(QDir::rootPath()+"digilent-agent.ini");
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
    //Create default config data
    QSettings* listenerSettings = new QSettings(iniPath, QSettings::IniFormat);
    listenerSettings->beginGroup("listener");
    listenerSettings->setValue("port", 42135);
    listenerSettings->setValue("minThreads", 4);
    listenerSettings->setValue("maxThreads", 100);
    listenerSettings->setValue("cleanupInterval", 60000);
    listenerSettings->setValue("readTimeout", 60000);
    listenerSettings->setValue("maxRequestSize", 16000000);
    listenerSettings->setValue("maxMultiPartSize", 16000000);
    listenerSettings->endGroup();

    listenerSettings->beginGroup("files");    
    listenerSettings->setValue("encoding", "UTF-8");
    listenerSettings->setValue("maxAge", 90000);
    listenerSettings->setValue("cacheTime", 60000);
    listenerSettings->setValue("cacheSize", 1000000);
    listenerSettings->setValue("maxCachedFileSize", 65536);
    listenerSettings->endGroup();

    listenerSettings->beginGroup("waveforms-live");
    listenerSettings->setValue("wwwRoot", wwwRoot);

    listenerSettings->sync();

    return iniPath;
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
