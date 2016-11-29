#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>
#include <QVector>

//WFL Agent includes
#include "../uartClient/uartInfo.h"
#include "../wflDevice/wflDevice.h"
#include "../wflDevice/wflUartDevice.h"

//#define MAX_DEVICE_COUNT 32

class Agent
{
public:
    Agent();

    WflDevice* activeDevice;
    //WflDevice* devices[MAX_DEVICE_COUNT];
    UartInfo* uartInfo;

    QVector<QString> enumerateDevices();
    //void flushDevices();
    QByteArray getVersion();
    int getMajorVersion();
    int getMinorVersion();
    int getPatchVersion();
    bool setActiveDeviceByName(QString deviceName);
    //bool setActiveDeviceByIndex(int index);

private:
    //Variables
    int deviceCount;
    bool httpCapable;
    int majorVersion;
    int minorVersion;
    int patchVersion;

};

#endif // AGENT_H
