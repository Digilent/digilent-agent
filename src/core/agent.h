#ifndef AGENT_H
#define AGENT_H

//QT Core Includes
#include <QString>
#include <QJsonObject>

//OS Device Includes
#include "../osDevice/osDevice.h";

class Agent
{
public:
    Agent();

    OsDevice* activeDevice;
    QByteArray getVersion();
    int getMajorVersion();
    int getMinorVersion();
    int getPatchVersion();

private:
    //Variables
    int majorVersion;
    int minorVersion;
    int patchVersion;
};

#endif // AGENT_H
