#include <QEventLoop>
#include "osUartDevice.h"
#include "osHttpDevice.h"

OsUartDevice::OsUartDevice(QString address){
    qDebug("OsUartDevice Constructor");
    this->name = "UART Device";
    this->address = address;

    this->uartClient = new UartClient(this);
    connect(uartClient, SIGNAL(response(QString)), this, SLOT(onUartComplete(QString)));
    connect(uartClient, SIGNAL(timeout(QString)), this, SLOT(onUartTimeout(QString)));

}

void OsUartDevice::execCommand(QString cmd) {
    qDebug() << "OsUartDevice::execCommand() - " << name;

    uartClient->writeRead(address, 1000, cmd);
}

void OsUartDevice::onUartComplete(QString reply) {

    qDebug("OsUartDevice::onUartComplete()");
    qDebug() << reply;    
    emit execCommandComplete(reply);
}

void OsUartDevice::onUartTimeout(QString message) {
    qDebug("MainWindow::onUartTimeout()");
    qDebug() << message;
}
