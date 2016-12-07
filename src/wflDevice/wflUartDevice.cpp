#include <QEventLoop>
#include "wflUartDevice.h"
#include "wflHttpDevice.h"

WflUartDevice::WflUartDevice(QString address){
    qDebug("WflUartDevice Constructor");
    name = "UART Device";
    deviceType = "UART";
    this->address = address;

    this->uartClient = new UartClient(this);
    connect(uartClient, SIGNAL(timeout(QByteArray)), this, SLOT(onUartTimeout(QByteArray)));

}

void WflUartDevice::execCommand(QByteArray cmd) {
    qDebug() << "OsUartDevice::execCommand() - " << name;
    connect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onUartExecCmdComplete(QByteArray)));
    uartClient->writeRead(address, 1000, cmd);
}

//Synchronous UART Write / Read
QByteArray WflUartDevice::writeRead(QByteArray cmd){

    QEventLoop loop;

    //Connect slots and signals
    connect(this, SIGNAL(writeReadComplete()), &loop, SLOT(quit()));
    connect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onWriteReadResponse(QByteArray)));

    bool waitingForResponse = true;
    uartClient->writeRead(address, 1000, cmd);

    //Wait for signal that device call has returned
    if(waitingForResponse){
        qDebug("UART writeRead() Loop Begin");
        loop.exec();
          qDebug("UART writeRead() Loop Done");
    }
    qDebug("::::UART Response:::: " + this->reply);

    return reply;
}

void WflUartDevice::onUartExecCmdComplete(QByteArray reply) {
    qDebug("OsUartDevice::onUartComplete()");
    disconnect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onUartExecCmdComplete(QByteArray)));
    emit execCommandComplete(reply);
}

void WflUartDevice::onUartTimeout(QByteArray message) {
    qDebug("WflUartDevice::onUartTimeout()");
    qDebug() << message;

    //Not sure which event timed out but only one will be connected so emit both signals
    emit execCommandComplete("");
    emit writeReadComplete();


    //disconnect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onUartExecCmdComplete(QByteArray)));
}

void WflUartDevice::onWriteReadResponse(QByteArray reply){
    qDebug("WflUartDevice::onWriteReadResponse()");
    this->reply = reply;
    disconnect(uartClient, SIGNAL(response(QByteArray)), this, SLOT(onWriteReadResponse(QByteArray)));
    emit writeReadComplete();
}

void WflUartDevice::softReset(){
    qDebug("WflUartDevice::softReset()");
    this->uartClient->softReset();

}
