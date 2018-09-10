#include "wflDptiDevice.h"
#include <QEventLoop>

//FT245 Support
//#include "lib/digilent/adept/include/dpcdecl.h"
//#include "lib/digilent/adept/include/dmgr.h"
//#include "lib/digilent/adept/include/dpti.h"

WflDptiDevice::WflDptiDevice(QString deviceName, QObject* parent) : WflDevice(parent)
{
    qDebug() << "WflDptiDevice::WflDptiDevice()" << "thread: " << QThread::currentThread();
    this->name = deviceName;
    this->deviceType = "";

    if (!DmgrOpen(this->deviceHandle, deviceName.toLatin1().data()) ) {
        qDebug() << "Unable to open " << deviceName;
    }
}

WflDptiDevice::~WflDptiDevice(){
    qDebug() << "WflDptiDevice::~WflDptiDevice()" << "thread: " << QThread::currentThread();

    if(this->deviceHandle != NULL) {
//        DptiDisable(*this->deviceHandle);
        DmgrClose(*this->deviceHandle);
    }

    this->name = "";
}

//Send a command to the device and return the response.  Upon completion this function emits the execCommandComplete signal with the response from the device.
void WflDptiDevice::execCommand(QByteArray cmd) {
   /*
    qDebug() << "WflSerialDevice::execCommand()" << "thread: " << QThread::currentThread();
    QByteArray resp = this->writeRead(cmd);
    emit execCommandComplete(resp);
    */
}

//Send a signal to the serial object to start the transfer.  This function blocks until the serial object response (via signal) and then this funciton returns the response from the serial device.
QByteArray WflDptiDevice::writeRead(QByteArray cmd) {
    /*
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

    */
    return this->data;
}

//Close and re-open the serial port;
bool WflDptiDevice::softReset() {
    /*
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
    */
}

//Return true if the device serial port is open, false otherwise.
bool WflDptiDevice::isOpen() {
    /*
   return this->serial->isOpen();
   */
}

//Release this device and free the COM port by destroying this object.
void WflDptiDevice::release(){
    /*
    qDebug() << "WflSerialDevice::release()" << "thread: " << QThread::currentThread();

    //Close the serial port so it is immediatly available to other processes, then schedule this object and it's children for deletion
    this->serial->close();
    //this->deleteLater();
    emit releaseComplete();

    qDebug() << "WflSerialDevice::release() - Done" << "thread: " << QThread::currentThread();
    */
}
