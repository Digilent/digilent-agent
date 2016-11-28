#ifndef AGENT_H
#define AGENT_H

#include <QString>
#include <QJsonObject>

class Agent
{
public:
    Agent();
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
