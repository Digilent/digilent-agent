#include "agent.h"

Agent::Agent()
{
    this->majorVersion = 1;
    this->minorVersion = 0;
    this->patchVersion = 0;
}

QByteArray Agent::getVersion() {
    return QByteArray(QString("%1.%2.%3").arg(majorVersion).arg(minorVersion).arg(patchVersion).toUtf8());
}

int Agent::getMajorVersion() {
    return this->majorVersion;
}

int Agent::getMinorVersion() {
    return this->minorVersion;
}

int Agent::getPatchVersion() {
    return this->patchVersion;
}
