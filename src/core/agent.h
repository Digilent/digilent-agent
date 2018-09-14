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
#include "../wflDevice/wflDptiDevice.h"

//class Agent
class Agent : public QObject
{
    Q_OBJECT

public:
    explicit Agent(QObject *parent = 0);
    virtual ~Agent();

    WflDevice* activeDevice;

    enum class WflDeviceType {
        Unknown,
        Serial,
        Dpti
    };


    struct WflDeviceDescriptor {
      QString Name;
      WflDeviceType Type;
      QString Serial;
    };

    QVector<WflDeviceDescriptor> enumerateDevices();
    QVector<WflDeviceDescriptor> enumerateAdeptDevices();
    QVector<WflDeviceDescriptor> enumerateSerialDevices();
    QString dptiToSerialPortName(QString dptiSerialNumber);
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
    WflDeviceDescriptor newWflDeviceDescriptor(QString name, WflDeviceType type, QString serial);
    QVector<QString> getDeviceNames(QVector<WflDeviceDescriptor> deviceDescriptors);





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
