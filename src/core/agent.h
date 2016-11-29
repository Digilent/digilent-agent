#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>
#include <QVector>

//WFL Agent includes
#include "../uartClient/uartInfo.h"
#include "../osDevice/osDevice.h"
#include "../osDevice/osUartDevice.h"
#include "../osDevice/oshttpdevice.h"

#define MAX_DEVICE_COUNT 32

class Agent
{
public:
    Agent();

    OsDevice* activeDevice;
    OsDevice* devices[MAX_DEVICE_COUNT];
    UartInfo* uartInfo;

    void enumerateDevices();
    void flushDevices();
    QByteArray getVersion();
    int getMajorVersion();
    int getMinorVersion();
    int getPatchVersion();
    bool setActiveDeviceByIndex(int index);

private:
    //Variables
    int deviceCount;
    bool httpCapable;
    int majorVersion;
    int minorVersion;
    int patchVersion;

};

#endif // AGENT_H
