#include "osDevice.h"

OsDevice::OsDevice(QObject *parent) : QObject(parent)
{
    qDebug("OsDevice Constructor");
    deviceType = "Generic OpenScope Device";
    name = "Unknown Device";
}

void OsDevice::execCommand(QByteArray cmd){
    qDebug("OsDevice::execCommand()");
}
