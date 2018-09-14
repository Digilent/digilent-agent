#include "wflDevice.h"

#include <QDebug>

WflDevice::WflDevice(QObject *parent) : QObject(parent)
{
    qDebug() << "WflDevice::WflDevice()" << "thread: " << QThread::currentThread();
    deviceType = "Unknown";
    name = "Unknown Device";
    serialNumber = "";
}

WflDevice::~WflDevice() {
    qDebug() << "~WflDevice()";
}
