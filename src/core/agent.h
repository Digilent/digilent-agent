#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QObject>

//WFL Agent includes
#include "lib/digilent/qtSerial/serial.h"
#include "../wflDevice/wflDevice.h"
//#include "../wflDevice/wflUartDevice.h"
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
    bool launchWfl();
    void releaseActiveDevice();
    bool setActiveDeviceByName(QString deviceName);
    bool internetAvailable();

signals:
    void activeDeviceChanged(QString activeDeviceName);

private:
    //Variables
    int deviceCount;
    bool httpCapable;
    int majorVersion;
    int minorVersion;
    int patchVersion;

};

#endif // AGENT_H
