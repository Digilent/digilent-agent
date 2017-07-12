#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QObject>

//WFL Agent includes
#include "lib/digilent/qtSerial/serial.h"
#include "lib/digilent/pgm/digilentPgm.h"
#include "../wflDevice/wflDevice.h"
#include "../wflDevice/wflSerialDevice.h"

//class Agent
class Agent : public QObject
{
    Q_OBJECT

public:
    explicit Agent(QObject *parent = 0);
    virtual ~Agent();

    WflDevice* activeDevice;

    QVector<QString> enumerateDevices();
    //void flushDevices();
    QByteArray getVersion();
    int getMajorVersion();
    int getMinorVersion();
    int getPatchVersion();
    QString getBuildNote();
    QString getFirmwareUploadStatus();
    int getFirmwareUploadProgress();
    bool launchWfl();   
    bool internetAvailable();    
    QString waveFormsLiveBrowserPath;
    QThread* getThread();


signals:
    void activeDeviceChanged(QString activeDeviceName);    
    void startReleaseDevice();
    void softResetActiveDeviceSignal();
    void releaseComplete();
    void startUpdateActiveDeviceFirmware(QString hexFilePath, bool enterBootloader);
    void updateActiveDeviceFirmwareComplete(bool success);

public slots:
     //WflDevice *createNewWflSerialDevice(QString address);
     void releaseActiveDevice();
     bool setActiveDeviceByName(QString deviceName);
     bool updateActiveDeviceFirmware(QString hexFilePath, bool enterBootloader);


private:
    //Variables
    int deviceCount;
    bool httpCapable;
    int majorVersion;
    int minorVersion;
    int patchVersion;
    QString buildNote;

    QString firmwareUploadStatus;
    DigilentPgm *pgm;

};

#endif // AGENT_H
