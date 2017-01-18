#include "wflSerialDevice.h"

WflSerialDevice::WflSerialDevice(QString address)
{
    qDebug("WflSerialDevice Constructor");
    name = "Serial Device";
    deviceType = "UART";

    //Initialize serial object and open port.
    this->serial.open(address, 1250000);
}

void WflSerialDevice::execCommand(QByteArray cmd) {
    qDebug() << "WflSerialDevice::execCommand()";
    QByteArray resp = this->serial.writeRead(cmd, 1000, 50);
    emit execCommandComplete(resp);
}

QByteArray WflSerialDevice::writeRead(QByteArray cmd) {
    return this->serial.writeRead(cmd, 1000, 50);
}

//Close and re-open the serial port;
void WflSerialDevice::softReset() {
    QString name = this->serial.getName();
    int baudRate = this->serial.getBaudRate();
    this->serial.close();
    this->serial.open(name, baudRate);
}
