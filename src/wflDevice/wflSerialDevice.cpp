#include "wflSerialDevice.h"

WflSerialDevice::WflSerialDevice(QString address)
{
    qDebug("WflSerialDevice Constructor");
    name = "Serial Device";
    deviceType = "UART";

    //Initialize serial object, open the port and upgrade to the target baud rate (Work around Mac Issue)
    if(this->serial.open(address, 9600)) {
        if(!this->serial.setBaudRate(1250000)) {
            qDebug() << "Failed to updgrade serial baud rate when creating wflDevice for " << address;
        }
    }
}

WflSerialDevice::~WflSerialDevice(){
    qDebug() << "~WflSerialDevice()";
}

void WflSerialDevice::execCommand(QByteArray cmd) {
    qDebug() << "WflSerialDevice::execCommand()";
    QByteArray resp = this->serial.fastWriteRead(cmd, 2000, 50);
    emit execCommandComplete(resp);
}

QByteArray WflSerialDevice::writeRead(QByteArray cmd) {
    return this->serial.fastWriteRead(cmd, 2000, 50);
}

//Close and re-open the serial port;
bool WflSerialDevice::softReset() {
    QString name = this->serial.getName();
    int baudRate = this->serial.getBaudRate();
    this->serial.close();
    return this->serial.open(name, baudRate);
}

//Return true if the device serial port is open, false otherwise.
bool WflSerialDevice::isOpen() {
   return this->serial.isOpen();
}
