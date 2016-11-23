#include <QEventLoop>
#include "osUartDevice.h"
#include "osHttpDevice.h"

OsUartDevice::OsUartDevice(QString address){
    qDebug("OsUartDevice Constructor");
    name = "UART Device";
    deviceType = "UART";
    this->address = address;

    this->uartClient = new UartClient(this);
    connect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onUartComplete(QByteArray)));
    connect(uartClient, SIGNAL(timeout(QByteArray)), this, SLOT(onUartTimeout(QByteArray)));

}

void OsUartDevice::execCommand(QByteArray cmd) {
    qDebug() << "OsUartDevice::execCommand() - " << name;

    uartClient->writeRead(address, 1000, cmd);
}

void OsUartDevice::onUartComplete(QByteArray reply) {

    qDebug("OsUartDevice::onUartComplete()");
    //qDebug() << reply;
    emit execCommandComplete(reply);
}

void OsUartDevice::onUartTimeout(QByteArray message) {
    qDebug("MainWindow::onUartTimeout()");
    qDebug() << message;
}
