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

//class Agent
class Agent : public QObject
{
    Q_OBJECT

public:
    explicit Agent(QObject *parent = 0);
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
    void clearActiveDevice();
    bool setActiveDeviceByName(QString deviceName);

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
