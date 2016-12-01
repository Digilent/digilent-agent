#include <QEventLoop>
#include "wflUartDevice.h"
#include "wflHttpDevice.h"

WflUartDevice::WflUartDevice(QString address){
    qDebug("WflUartDevice Constructor");
    name = "UART Device";
    deviceType = "UART";
    this->address = address;

    this->uartClient = new UartClient(this);
    connect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onUartComplete(QByteArray)));
    connect(uartClient, SIGNAL(timeout(QByteArray)), this, SLOT(onUartTimeout(QByteArray)));

}

void WflUartDevice::execCommand(QByteArray cmd) {
    qDebug() << "OsUartDevice::execCommand() - " << name;
    uartClient->writeRead(address, 1000, cmd);
}

void WflUartDevice::write(QByteArray cmd){

}

void WflUartDevice::onUartComplete(QByteArray reply) {
    qDebug("OsUartDevice::onUartComplete()");
    //qDebug() << reply;
    emit execCommandComplete(reply);
}

void WflUartDevice::onUartTimeout(QByteArray message) {
    qDebug("MainWindow::onUartTimeout()");
    qDebug() << message;
}
