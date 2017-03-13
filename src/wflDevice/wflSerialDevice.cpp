#include "wflSerialDevice.h"
#include <QEventLoop>

WflSerialDevice::WflSerialDevice(QString address, QObject* parent) : WflDevice(parent)
{
    qDebug() << "WflSerialDevice::WflSerialDevice()" << "thread: " << QThread::currentThread();
    this->name = "";
    this->deviceType = "UART";

    //Initialize serial object, open the port and upgrade to the target baud rate (Work around Mac Issue)
    this->serial = new Serial(this);
    if(this->serial->open(address, 9600)) {
        this->name = address;
        if(!this->serial->setBaudRate(1250000)) {
            qDebug() << "Failed to updgrade serial baud rate when creating wflDevice for " << address;
        }
    }
}

WflSerialDevice::~WflSerialDevice(){
    qDebug() << "WflSerialDevice::~WflSerialDevice()" << "thread: " << QThread::currentThread();
    this->serial->close();
    delete this->serial;
    this->name = "";
}

//Send a command to the device and return the response.  Upon completion this function emits the execCommandComplete signal with the response from the device.
void WflSerialDevice::execCommand(QByteArray cmd) {
    qDebug() << "WflSerialDevice::execCommand()" << "thread: " << QThread::currentThread();
    QByteArray resp = this->writeRead(cmd);
    emit execCommandComplete(resp);
}

//Send a signal to the serial object to start the transfer.  This function blocks until the serial object response (via signal) and then this funciton returns the response from the serial device.
QByteArray WflSerialDevice::writeRead(QByteArray cmd) {
    qDebug() << "WflSerialDevice::writeRead2()" << "thread: " << QThread::currentThread();

    QEventLoop loop;

    connect(this, SIGNAL(writeReadComplete()), &loop, SLOT(quit()));                                                        //Connect signal to exit the blocking loop once this object gets a response
    connect(this, SIGNAL(startFastWriteRead(QByteArray,int,int)), this->serial, SLOT(fastWriteRead(QByteArray,int,int)));   //Connect signal that this object emits to start the fastWriteRead
    connect(this->serial, SIGNAL(fastWriteReadResponse(QByteArray)), this, SLOT(onFastWriteReadResponse(QByteArray)));      //Connect the signal that the fastWriteRead emits with the reponse

    //Start the fastWriteRead in thread that owns the serial object
    emit startFastWriteRead(cmd, 2000, 100);

    //Loop until writeRead is complete and writeReadComplete() signal is emited
    loop.exec();

    //Disconnect signals before returning
    disconnect(this, SIGNAL(writeReadComplete()), &loop, SLOT(quit()));
    disconnect(this, SIGNAL(startFastWriteRead(QByteArray,int,int)), this->serial, SLOT(fastWriteRead(QByteArray,int,int)));
    disconnect(this->serial, SIGNAL(fastWriteReadResponse(QByteArray)), this, SLOT(onFastWriteReadResponse(QByteArray)));

    return this->data;
}

//Close and re-open the serial port;
bool WflSerialDevice::softReset() {
    qDebug() << "WflSerialDevice::softReset()" << "thread: " << QThread::currentThread();

    QEventLoop loop;

    //Connect slots and signals for cross thread call
    connect(this, SIGNAL(softResetComplete()), &loop, SLOT(quit()));
    connect(this, SIGNAL(startSoftReset()), this->serial, SLOT(softReset()));
    connect(this->serial, SIGNAL(softResetResponse(bool)), this, SLOT(onSoftResetResponse(bool)));

    //Start the fastWriteRead in thread that owns the serial object
    emit startSoftReset();

    //Loop until writeRead is complete and writeReadComplete() signal is emited
    loop.exec();

    //Disconnect signals before returning
    disconnect(this, SIGNAL(softResetComplete()), &loop, SLOT(quit()));
    disconnect(this, SIGNAL(startSoftReset()), this->serial, SLOT(softReset()));
    disconnect(this->serial, SIGNAL(softResetResponse(bool)), this, SLOT(onSoftResetResponse(bool)));

    return this->softResetSuccess;
}

//Return true if the device serial port is open, false otherwise.
bool WflSerialDevice::isOpen() {
   return this->serial->isOpen();
}

//Release this device and free the COM port by destroying this object.
void WflSerialDevice::release(){
    qDebug() << "WflSerialDevice::release()" << "thread: " << QThread::currentThread();

    //Close the serial port so it is immediatly available to other processes, then schedule this object and it's children for deletion
    this->serial->close();
    //this->deleteLater();
    emit releaseComplete();

    qDebug() << "WflSerialDevice::release() - Done" << "thread: " << QThread::currentThread();
}

//Slot triggered when the Serial::fastWriteReadResponse signal is emitted
void WflSerialDevice::onFastWriteReadResponse(QByteArray response) {
    qDebug() << "WflSerialDevice::onFastWriteReadResponse()  Complete - " << response;

    //Save response data and emit signal to exit blocking loop
    this->data = response;
    emit writeReadComplete();
}


//Slot triggered when the Serial::softResetResponse signal is emitted
void WflSerialDevice::onSoftResetResponse(bool success) {
    qDebug() << "WflSerialDevice::onSoftResetResponse() - Success:" << success;

    //Save response and emit signal to exit blocking loop
    this->softResetSuccess = success;
    emit softResetComplete();
}
