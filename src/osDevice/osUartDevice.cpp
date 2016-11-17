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

    //Wait for signal that proxy call has returned
    //QEventLoop loop;
    //connect(this, SIGNAL(commandComplete()), &loop, SLOT(quit()));
    uartClient->writeRead(address, 1000, "Hello UART!");
    //loop.exec();
    qDebug(" OsUartDevice::execCommand Complete");

}

void OsUartDevice::onUartComplete(QString reply) {

    qDebug("OsUartDevice::onUartComplete()");
    qDebug() << reply;
    //emit commandComplete();
    emit execCommandComplete(reply);
}

void OsUartDevice::onUartTimeout(QString message) {
    qDebug("MainWindow::onUartTimeout()");
    qDebug() << message;
}
