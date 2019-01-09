#include "wflDevice.h"

#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

WflDevice::WflDevice(QObject *parent) : QObject(parent)
{
    qDebug() << "WflDevice::WflDevice()" << "thread: " << QThread::currentThread();
    deviceType = "Unknown";
    name = "Unknown Device";
    serialNumber = "";
}

//Return -1 if packet is malformed and leading data should be stripped
//Return 0 if packet is incomplete
//Return 1 if packet is complete
int WflDevice::validDipPacket(QByteArray data)
{
    qDebug() << ":::::::::::::::Checking for valid DIP packet";
    //Check for invalid leading characters
    if(!(data[0] == '{') && !(data[0] >= '0' && data[0] <= '9') && !(data[0] >= 'a' && data[0] <= 'f') && !(data[0] >= 'A' && data[0] <= 'F'))
    {
        qDebug() << "================== Found invalid leading characters";
        return -1;
    }
    else if(data[0] == '{')
    {
        //Check if response is a complete JSON object
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if(!doc.isNull())
        {

            qDebug() << "================== Complete JSON packet";
            qDebug() << data;
            qDebug() << "---------------------------";
            qDebug() << doc;
            qDebug() << "---------------------------";
            return 1;
        }
        else
        {
            qDebug() << "================== Incomplete JSON packet";
            return 0;
        }

    }
    else {
        //Check if data is complete chunked transfer
        return validChunkedData(data);
    }
}

//Returns the chunk size if the specified input is in valid chunk format or -1 otherwise
int WflDevice::getChunkSize(QString data) {
    int endIndex = data.indexOf("\r\n");
    if(endIndex > 0) {
        bool ok = false;
        unsigned int chunkSize = data.left(endIndex).toUInt(&ok, 16);
        if(ok){
            return (int)chunkSize;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

bool WflDevice::validChunkedData(QByteArray data) {
    qDebug() << "=========================Validating chuncked data===============";
    while(getChunkSize(data) >= 0)
    {
        int chunkSize = getChunkSize(data);
        int startOfChunk = data.indexOf("\r\n") + 2;

        //Return false if chunk is not complete
        if(data.length() < startOfChunk + chunkSize + 2) {
            qDebug() << "==================Incomplete Chunk";
            return false;
        }

        //If chunk size is 0 and the chunk is complete we're at the end
        if(chunkSize == 0)
        {
            qDebug() << "============================================Found Valid End Of Chunked Transfer" << chunkSize;
            return true;
        }

        QByteArray chunk = data.mid(startOfChunk, chunkSize);
        //qDebug() << "==================Valid" << chunkSize << "Byte Chunk:" << chunk;
        //qDebug() << "==================Remaining Data" << data.mid(startOfChunk + chunkSize + 2).length() << "Bytes" << data.mid(startOfChunk + chunkSize + 2);
        data = data.mid(startOfChunk + chunkSize + 2);
    }
    return false;
}





WflDevice::~WflDevice() {
    qDebug() << "~WflDevice()";
}
