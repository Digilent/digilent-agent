#include "wflDptiDevice.h"
#include <QEventLoop>
#include <QTime>

//FT245 Support
#include "lib/digilent/adept/include/dpcdecl.h"
#include "lib/digilent/adept/include/dmgr.h"
#include "lib/digilent/adept/include/dpti.h"

WflDptiDevice::WflDptiDevice(QString deviceName, QString serialNumber, QObject* parent) : WflDevice(parent)
{
    qDebug() << "WflDptiDevice::WflDptiDevice()" << "thread: " << QThread::currentThread();
    this->name = deviceName;
    this->deviceType = "DPTI";
    this->serialNumber = serialNumber;

    char* name = deviceName.toLatin1().data();
    qDebug() << deviceName.toLatin1().data();
    qDebug() << this->serialNumber;

    if(!DmgrOpen(&this->deviceHandle, this->serialNumber.toLatin1().data()) ) {
        qDebug() << "Unable to open " << deviceName;
    }

    //Set transfer timeout
    DmgrSetTransTimeout(this->deviceHandle, 1000);

    //Enable DPTI port
    if(!DptiEnable(this->deviceHandle) ) {
        qDebug() << "Failed to open DPTI port";
    }

}

WflDptiDevice::~WflDptiDevice(){
    qDebug() << "WflDptiDevice::~WflDptiDevice()" << "thread: " << QThread::currentThread();

    if(this->deviceHandle != 0) {
        DptiDisable(this->deviceHandle);
        DmgrClose(this->deviceHandle);
    }

    this->name = "";
}

//Send a command to the device and return the response.  Upon completion this function emits the execCommandComplete signal with the response from the device.
void WflDptiDevice::execCommand(QByteArray cmd) {
    qDebug() << "WflDptiDevice::execCommand()" << "thread: " << QThread::currentThread();
    QByteArray resp = this->writeRead(cmd);
    emit execCommandComplete(resp);
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
   return this->deviceHandle != 0;
}

//Release this device and free the COM port by destroying this object.
void WflDptiDevice::release(){
     qDebug() << "WflDptiDevice::release() - Done" << "thread: " << QThread::currentThread();

        //Disable the DPTI port and close.
     if(this->deviceHandle != 0)
     {
        DptiDisable(this->deviceHandle);
        DmgrClose(this->deviceHandle);
     }
     emit releaseComplete();
     qDebug() << "WflDptiDevice::release()  - Complete";
}





//---------------------------- Write ---------------------------
//bool write(const char *data, int numBytes);

bool WflDptiDevice::write(QByteArray data)
{
    return DptiIO(this->deviceHandle, reinterpret_cast <BYTE *>(data.data()), static_cast <unsigned long>(data.length()), nullptr, 0, false);
}

QByteArray WflDptiDevice::writeRead(QByteArray cmd)
{
    return dipWriteRead(cmd, 2000, 2000);
}

//Write data then read until a valid DIP resposne is received or a timeout
QByteArray WflDptiDevice::dipWriteRead(QByteArray data, int delay, int timeout)
{
    qDebug() << "WflDptiDevice::dipWriteRead()" << "thread: " << QThread::currentThread();

    QByteArray resp;
    QTime stopWatch;
    stopWatch.start();


    //To do (???) - Clear incoming buffer before writing new command

    //Write Command
    this->write(data);

    //Read until first byte comes in or the delay expires
    stopWatch.restart();
    while(bytesAvailable() < 1)
    {
        if(stopWatch.elapsed() > delay)
        {
            qDebug() << "WflDptiDevice::dipWriteRead() - Timeout while waiting for first byte of resposne";
            return read();
        }
        break;
    }

    //Read all available bytes, reset timeout
    resp.append(read());
    stopWatch.restart();

    //Continue reading as long as bytes continue coming in until the DIP packet is complete or a timeout
    while(stopWatch.elapsed() < timeout)
    {
        if(bytesAvailable())
        {
            resp.append(read());
            //Check if DIP packet is complete
            while(resp.length() > 0)
            {
                int status = validDipPacket(resp);
                if(status < 0 ){
                    //Response is malformed strip leading byte and try again
                    resp = resp.mid(1);
                }
                else if (status > 0)
                {
                    return resp;
                }
                //Packet looks valid but is not complete.
                break;
            }
            stopWatch.restart();
        }
    }

    //Timeout waiting for more bytes

    qDebug() << "WflDptiDevice::dipWriteRead() - Timeout waiting for DIP packet to complete";
    qDebug() << "-------------------------------------\r\n\r\n";
    qDebug() << resp;
    qDebug() << "-------------------------------------\r\n\r\n";
    return resp;
}

//---------------------------- Read ---------------------------
//Read all available bytes from the DPTI buffer.  Data is returned as a byte array.
QByteArray WflDptiDevice::read() {
    if(this->deviceHandle == 0) {
        return nullptr;
    }

    unsigned long count = bytesAvailable();
    QByteArray data(static_cast<int>(count), '0');

    DptiIO(this->deviceHandle, nullptr, 0, reinterpret_cast <BYTE *>(data.data()), count, false);
    unsigned long rxCount = 0;
    if(!DmgrGetTransResult(this->deviceHandle, nullptr, &rxCount, 0))
    {
        qDebug() << "Error when reading from DPTI buffer";
        return nullptr;
    } else if(rxCount != count) {
        qDebug() << "Error when reading from DPTI buffer - incorrect number of bytes read";
        return nullptr;
    } else {
        return data;
    }
}

//Read the specified number of bytes from the buffer.
/*
QByteArray read(qint64 numBytes)
{

}
*/

//Return the number of bytes available in the DPTI buffer
unsigned long WflDptiDevice::bytesAvailable()
{
    unsigned long count;
    if(this->deviceHandle != 0){
        DptiGetQueueStatus(this->deviceHandle, &count);
    }
    //qDebug() << "=======Count: " << count;
    return count;
}

//Wait for the specified number of bytes to be avialable or timeout.  Returns true if the specified number of bytes are available, false otherwise.
bool WflDptiDevice::waitForBytesAvailable(unsigned long numBytes, int timeout) {

    QTime startTime = QTime::currentTime();
    QTime doneTime = startTime.addMSecs(timeout);
    while(QTime::currentTime() < doneTime) {
        if(bytesAvailable() >= numBytes) {
            return true;
        }
    }
    return false;
}







