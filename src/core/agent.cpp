#include "agent.h"

Agent::Agent()
{
    this->httpCapable = true;
    this->majorVersion = 1;
    this->minorVersion = 0;
    this->patchVersion = 0;

    //Initialize devices array with null pointers
    this->activeDevice = 0;
    /*
    this->deviceCount = 0;
    for(int i=0; i < MAX_DEVICE_COUNT; i++)
    {
        devices[i] = 0;
    }
    */
    //Uart
    this->uartInfo = new UartInfo();
}

QVector<QString> Agent::enumerateDevices() {
    //---------- UART ----------
    QVector<QString> devices = QVector<QString>();
    uartInfo->refreshPortInfo();    
    for(int i=0; i<uartInfo->ports.count(); i++)
    {
        if(!uartInfo->ports[i].isBusy())
        {
            devices.append((uartInfo->ports[i].portName()));
         }
    }

    //---------- HTTP ----------
    //TODO?

    return devices;
}

/*
void Agent::flushDevices() {
    for(int i=0; i < MAX_DEVICE_COUNT; i++)
    {
        delete devices[i];
        devices[i] = 0;
    }
    this->deviceCount = 0;
}
*/

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

bool Agent::setActiveDeviceByName(QString deviceName) {
    this->activeDevice = new WflUartDevice(deviceName);
    return true;
}

/*
bool Agent::setActiveDeviceByIndex(int index){
    if(index > 0 && index < this->deviceCount){
        this->activeDevice = devices[index];
        return true;
    }
    return false;
}
*/
