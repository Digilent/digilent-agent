#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QObject>

//WFL Agent includes
#include "../uartClient/uartInfo.h"
#include "../wflDevice/wflDevice.h"
#include "../wflDevice/wflUartDevice.h"

class Agent
{

public:
    Agent();
    virtual ~Agent();

    WflDevice* activeDevice;
    UartInfo* uartInfo;

    QVector<QString> enumerateDevices();
    //void flushDevices();
    QByteArray getVersion();
    int getMajorVersion();
    int getMinorVersion();
    int getPatchVersion();
    bool launchWfl();
    bool clearActiveDevice();
    bool setActiveDeviceByName(QString deviceName);

signals:
    //void activeDeviceChanged(QString activeDeviceName);

private:
    //Variables
    int deviceCount;
    bool httpCapable;
    int majorVersion;
    int minorVersion;
    int patchVersion;

};

#endif // AGENT_H
