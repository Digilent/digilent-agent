#include <QDebug>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork>
#include <QUrl>

#include "agent.h"

//Agent::Agent()
Agent::Agent(QObject *parent) : QObject(parent)
{
    this->httpCapable = true;
    this->majorVersion = 0;
    this->minorVersion = 1;
    this->patchVersion = 7;

    //Initialize devices array with null pointers
    this->activeDevice = 0;
}

Agent::~Agent(){
    qDebug("Agent Descructor");
    if(this->activeDevice != 0)
    {
        qDebug("Freeing Active Device");
        releaseActiveDevice();
    }
}

//Return all UART devices on the system that are not busy plus the active device even if it is busy
QVector<QString> Agent::enumerateDevices() {
    //---------- UART ----------
    QVector<QString> devices = QVector<QString>();
    QList<QSerialPortInfo> serialPortInfo = Serial::getSerialPortInfo();

    //Loop over all devices on the system
    for(int i=0; i<serialPortInfo.length(); i++)
    {
        if(serialPortInfo[i].isBusy())
        {
            //Only add a busy device if it is the active device
            if(this->activeDevice != 0)
            {
                if(serialPortInfo[i].portName() == this->activeDevice->name)
                {
                    devices.append(serialPortInfo[i].portName());
                }
            }
        }
        else
        {
            //Device is available, add it
            devices.append(serialPortInfo[i].portName());
        }
    }

    //---------- HTTP ----------
    //TODO?

    return devices;
}

QByteArray Agent::getVersion() {
    return QByteArray(QString("%1.%2.%3").arg(majorVersion).arg(minorVersion).arg(patchVersion).toUtf8());
}

int Agent::getMajorVersion() {
    return this->majorVersion;
}

int Agent::getMinorVersion() {
    return this->minorVersion;
}

int Agent::getPatchVersion() {
    return this->patchVersion;
}

bool Agent::launchWfl() {
    //Todo - Launch Electron Version If It Exists

    if(internetAvailable()) {
        return QDesktopServices::openUrl(QUrl("http://waveformslive.com/"));
    } else {
       return QDesktopServices::openUrl(QUrl("http://127.0.0.1:56089/"));
    }
}

//Set the active device by name.  A new device object is created unless the target device is already active and open.  This command also puts the device into JSON mode.
bool Agent::setActiveDeviceByName(QString deviceName) {

    QVector<QString> devices = enumerateDevices();

    if(this->activeDevice != 0)
    {
        //An active device exists
        if(this->activeDevice->name == deviceName)
        {
            //The target device matches the active device, check if it is still available
            for(int i=0; i<devices.size(); i++)
            {
                if(deviceName == devices[i])
                {
                    //Target device is already active and still exists.  Try to put it in JSON mode to confirm the agent has the device open and not some other app
                    this->activeDevice->softReset();
                    QByteArray res = this->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");
                    qDebug("Agent::setActiveDeviceByName() - Target Already Active - %s", res.data());
                    QJsonDocument resDoc = QJsonDocument::fromJson(res);
                    if(!resDoc.isNull()){
                        QJsonObject resObj = resDoc.object();
                        QString mode = resObj.value("mode").toString();
                        if(mode == "JSON")
                        {
                            return true;
                        }
                        else
                        {
                            //No response from the device, something else must have it open
                            releaseActiveDevice();
                            return false;
                        }
                    } else {
                        //No response from the device, something else must have it open
                        releaseActiveDevice();
                        return false;
                    }
                }
            }
                //Target device is already active but no longer available
                releaseActiveDevice();
                return false;
        } else {
            //The current active device is not the target active device, free it
            releaseActiveDevice();
        }
    }
    //No active device, check if target device is available, if so set it as the active device
    for(int i=0; i<devices.size(); i++)
    {
        if(devices[i] == deviceName)
        {
            //Create device object and enable JSON mode
            this->activeDevice = new WflSerialDevice(deviceName);
            this->activeDevice->name = deviceName;
            emit activeDeviceChanged(QString(deviceName));
            this->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");
            return true;
        }
    }

    //Target device does not exist
    return false;
}

void Agent::releaseActiveDevice(){
    qDebug("Agent::releaseActiveDevice()");
    if(this->activeDevice != 0) {
        delete this->activeDevice;
        this->activeDevice = 0;
        emit activeDeviceChanged("");
    }
}

//Returns true if internet access is available
bool Agent::internetAvailable() {
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://waveformslive.com"));
    QNetworkReply *response = nam.get(req);
    QEventLoop loop;
    connect(response, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(response->bytesAvailable()) {
        return true;
    } else {
        return false;
    }
}

