#include "wflDevice.h"

#include <QDebug>

WflDevice::WflDevice(QObject *parent) : QObject(parent)
{
    qDebug() << "WflDevice::WflDevice()" << "thread: " << QThread::currentThread();
    deviceType = "Unknown";
    name = "Unknown Device";
}

WflDevice::~WflDevice() {
    qDebug() << "~WflDevice()";
}
