#include <QDebug>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork>
#include <QTime>
#include <QThread>
#include <QUrl>

//OS specific includes and config
#ifdef _WIN32
    #include <windows.h>
#elif __linux__

#elif __APPLE__

#endif

#include "agent.h"

//FT245 Support
#include "lib/digilent/adept/include/dpcdecl.h"
#include "lib/digilent/adept/include/dmgr.h"
#include "lib/digilent/adept/include/dpti.h"

//Agent::Agent()
Agent::Agent(QObject *parent) : QObject(parent)
{
    qDebug() << "Agent::Agent()" << "thread: " << QThread::currentThread();

    this->httpCapable = true;
    this->majorVersion = 1;
    this->minorVersion = 2;
    this->patchVersion = 2;

#ifdef AGENT_BUILD_NOTE
    this->buildNote = AGENT_BUILD_NOTE;
#endif

    this->firmwareUploadStatus = "idle";

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
QVector<Agent::WflDeviceDescriptor> Agent::enumerateDevices() {

    //Start with Adept devices then append serial devices
    QVector<WflDeviceDescriptor> devices = this->enumerateAdeptDevices();
    devices.append(this->enumerateSerialDevices());
    return devices;
}

QVector<Agent::WflDeviceDescriptor> Agent::enumerateAdeptDevices() {

    QVector<Agent::WflDeviceDescriptor> devices = QVector<Agent::WflDeviceDescriptor>();

     //FT245 Variables
     DVC		dvc;
     int		idvc;
     int		cdvc;
     char	szTmp[1024];

     /* Produce the enumerated list of devices. */
     // DMGR API Call: DmgrEnumDevices
     if (!DmgrEnumDevices(&cdvc)) {
         qDebug("Error enumerating devices\n");
         return devices;
     }

     /* Print some basic information about each device. */
     qDebug() << "Found " << cdvc <<" devices\n";

     //Iterate over all Adept devices to populate devices list
     for (idvc = 0; idvc < cdvc; idvc++) {
         QString deviceName = "";

         if (!DmgrGetDvc(idvc, &dvc)) {
             qDebug("Error getting device info\n");
         } else {
             //Build device name and add it to the list

             //Get device name
             if (!DmgrGetInfo(&dvc, dinfoProdName, szTmp)) {
                 qDebug(szTmp, "Not accessible (name)");
             }
             QString name(szTmp);
             deviceName.append(name);

            //Get the serial number
             if (!DmgrGetInfo(&dvc, dinfoSN, szTmp)) {
                 qDebug(szTmp, "Not accessible (serial)");
             }

             QString serialNumber(szTmp);
             deviceName.append(" (");
             deviceName.append(serialNumber);
             deviceName.append(")");

             //Add device to list
            devices.append(newWflDeviceDescriptor(deviceName, WflDeviceType::Dpti, serialNumber));

            //To do remove test

            qDebug() << "Adept " << deviceName << "maps to " << this->dptiToSerialPortName(serialNumber);
         }


     }
     //Free Adept resources
     DmgrFreeDvcEnum();
     return devices;
}

QVector<Agent::WflDeviceDescriptor> Agent::enumerateSerialDevices()
{
    QVector<Agent::WflDeviceDescriptor> devices = QVector<Agent::WflDeviceDescriptor>();

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
                    devices.append(newWflDeviceDescriptor(serialPortInfo[i].portName(), WflDeviceType::Serial, serialPortInfo[i].serialNumber()));
                }
            }
        }
        else
        {
            //Device is available, add it
            devices.append(newWflDeviceDescriptor(serialPortInfo[i].portName(), WflDeviceType::Serial, serialPortInfo[i].serialNumber()));
        }
    }
    return devices;
}


Agent::WflDeviceDescriptor Agent::newWflDeviceDescriptor(QString name, WflDeviceType type, QString serial)
{
    WflDeviceDescriptor wflDevice = WflDeviceDescriptor();
    wflDevice.Name = name;
    wflDevice.Type = type;
    wflDevice.Serial = serial;
    return wflDevice;
}

QString Agent::dptiToSerialPortName(QString dptiSerialNumber)
{
    QVector<Agent::WflDeviceDescriptor> devices = this->enumerateSerialDevices();

    //Format DPTI serial number if necissary (Trim 'SN:')
    int i = dptiSerialNumber.indexOf("SN:");
    if(i >= 0)
    {
        dptiSerialNumber = dptiSerialNumber.right(dptiSerialNumber.length() - (i + 3));
    }

    for(int i=0; i<devices.length(); i++)
    {

        if(devices[i].Serial.contains(dptiSerialNumber.left(dptiSerialNumber.length()-1)))
        {
            return devices[i].Name;
        }
    }
    return "";
}

//Convert vector of device descritors into vector of device names (QStrings)
QVector<QString> Agent::getDeviceNames(QVector<WflDeviceDescriptor> deviceDescriptors)
{
    QVector<QString> deviceNames = QVector<QString>();
    for(int i=0; i < deviceDescriptors.length(); i++)
    {
        deviceNames.append(deviceDescriptors[i].Name);
    }
    return deviceNames;
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

QString Agent::getBuildNote() {
    return this->buildNote;
}

bool Agent::launchWfl() {
    //Todo - Launch Electron Version If It Exists

    if(internetAvailable()) {
        return QDesktopServices::openUrl(QUrl("http://waveformslive.com/"));
    } else {
       return QDesktopServices::openUrl(QUrl("http://127.0.0.1:42135/"));
    }
}

//Set the active device by name.  A new device object is created unless the target device is already active and open.  This command also puts the device into JSON mode.
/*
bool Agent::setActiveDeviceByName(QString deviceName) {
    qDebug() << "Agent::setActiveDeviceByName()" << "thread: " << QThread::currentThread();

    QVector<QString> devices = this->getDeviceNames(enumerateDevices());

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
                    // -- TODO OLD REMOVE - Target device is already active and still exists.  SoftReset it to make sure the agent is the app that has it open (not some other app)
                    // -- NEW - Check active device name against target device name to avoid having to soft reset the device.
                    if(this->activeDevice->name == deviceName)
                    {
                        releaseActiveDevice();
                        this->activeDevice = new WflSerialDevice(deviceName);
                        this->activeDevice->moveToThread(this->getThread());
                       // this->activeDevice = new WflSerialDevice(deviceName);
                        if(!this->activeDevice->isOpen()){
                            //Failed to open serial port
                            return false;
                        }
                        this->activeDevice->name = deviceName;
                        emit activeDeviceChanged(QString(deviceName));
                        qDebug() << "~~~~ENTER JSON MODE~~~~~~" << this->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");

                        //Connect release signal to device
                        connect(this, SIGNAL(startReleaseDevice()), activeDevice, SLOT(release()));
                        connect(this, SIGNAL(softResetActiveDeviceSignal()), activeDevice, SLOT(softReset()));
                        return true;
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
            this->activeDevice->moveToThread(this->getThread());
           // this->activeDevice = new WflSerialDevice(deviceName);
            if(!this->activeDevice->isOpen()){
                //Failed to open serial port
                return false;
            }
            this->activeDevice->name = deviceName;
            emit activeDeviceChanged(QString(deviceName));
            qDebug() << "~~~~ENTER JSON MODE~~~~~~" << this->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");

            //Connect release signal to device
            connect(this, SIGNAL(startReleaseDevice()), activeDevice, SLOT(release()));
            connect(this, SIGNAL(softResetActiveDeviceSignal()), activeDevice, SLOT(softReset()));
            return true;
        }
    }

    //Target device does not exist
    return false;
}
*/

bool Agent::setActiveDeviceByName(QString deviceName) {
    qDebug() << "Agent::setActiveDeviceByName()" << "thread: " << QThread::currentThread();

    QVector<WflDeviceDescriptor> devices = this->enumerateDevices();


    //Close active device is one exists
    if(this->activeDevice != 0)
    {
        this->releaseActiveDevice();
    }

    //Find device descriptor and create new device
    for(int i=0; i<devices.size(); i++)
    {
        if(devices[i].Name == deviceName)
        {
            qDebug() << devices[i].Name;
            //Create device object
             switch (devices[i].Type) {
                case WflDeviceType::Serial:
                    //Create serial device
                    this->activeDevice = new WflSerialDevice(deviceName, devices[i].Serial);
                    this->activeDevice->moveToThread(this->getThread());
                    if(!this->activeDevice->isOpen()){
                        //Failed to open serial port
                        return false;
                    }
                    this->activeDevice->name = deviceName;
                    emit activeDeviceChanged(QString(deviceName));

                     //Enable JSON Mode
                    this->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");
                    break;
                case WflDeviceType::Dpti:
                {
                    qDebug() << "Creating DPTI Device";
                    QString name = deviceName.left(deviceName.indexOf('(')).trimmed();
                    this->activeDevice = new WflDptiDevice(name, devices[i].Serial);
                    this->activeDevice->moveToThread(this->getThread());
                    emit activeDeviceChanged(QString(this->activeDevice->name));

                    //To do - DPTI Device setup?
                    break;
                }
                default:
                    qDebug() << "Unknown device type";
                    return false;
                    break;
            }

            //Connect release signal to device
            connect(this, SIGNAL(startReleaseDevice()), activeDevice, SLOT(release()));
            connect(this, SIGNAL(softResetActiveDeviceSignal()), activeDevice, SLOT(softReset()));
            return true;
        }
    }

    //Target device does not exist
    return false;
}


//Call to emit signal to free the active device.
void Agent::releaseActiveDevice(){
    qDebug() << "Agent::releaseActiveDevice" << "thread: " << QThread::currentThread();

    //Directly call activeDevice->release() if trigger source is in same thread as agent to avoid deadlock
    if(QThread::currentThread() == this->getThread()) {
        activeDevice->release();
    } else  {

        //Release trigger signal came from different thread, signal release and loop to wait for response
        QEventLoop loop;

        //Connect signal to detect when device has been released
        connect(this->activeDevice, SIGNAL(releaseComplete()), &loop, SLOT(quit()));

        if(this->activeDevice != 0) {
            //emit release signal
            emit startReleaseDevice();

            //Loop unit device has been released
            loop.exec();
            qDebug() << "Agent::releaseActiveDevice() - Post Loop";
        }
        disconnect(this->activeDevice, SIGNAL(releaseComplete()), &loop, SLOT(quit()));
    }

    this->activeDevice = 0;

    //Emit signal to update GUI menu text
    emit activeDeviceChanged("");
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

//Use Digilent PGM to update the active device firmware with the specified firmware hex file.  This function blocks until the firmware upload is complete and ruturns true if the firmware upload was successful and false otherwise.
bool Agent::updateActiveDeviceFirmware(QString hexPath, bool enterBootloader) {
    firmwareUploadStatus = "uploading";

    if(this->activeDevice != NULL) {

        this->pgm = new DigilentPgm();
        QString portName = "";

        //Determine UART port name
        if(this->activeDevice->deviceType == "UART")
        {
            portName = this->activeDevice->name;
        } else if (this->activeDevice->deviceType == "DPTI")
        {
            portName = this->dptiToSerialPortName(this->activeDevice->serialNumber);
        }

        //Send enter device bootloader command if necissary
        if(enterBootloader) {
            QByteArray devResp = this->activeDevice->writeRead("{\"device\":[{\"command\":\"enterBootloader\"}]})");
            QJsonDocument resDoc = QJsonDocument::fromJson(devResp);

            QJsonObject resObj = resDoc.object();
            QJsonArray deviceCmds = resObj.value("device").toArray();

            QJsonObject enterBootloaderObj = deviceCmds[0].toObject();
            int waitTime = enterBootloaderObj.value("wait").toInt();

            //Wait for device to enter bootloader
            QTime stopWatch;
            stopWatch.start();
            QTime startTime = QTime::currentTime();
            QTime doneTime = startTime.addMSecs(waitTime);
            while (QTime::currentTime() < doneTime) {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
        }

        //Release the device so we can update it's firmware
        this->releaseActiveDevice();

        //Use Digilent PGM to update the firmware
        if(this->pgm->programByPort(hexPath, portName)) {
            //Programming successful, free PGM and make device active again
            this->pgm->releaseDevice();
            delete this->pgm;
            qDebug() << "Firmware updated successfully";
            if(this->setActiveDeviceByName(portName)) {
                firmwareUploadStatus = "idle";
                emit updateActiveDeviceFirmwareComplete(true);
                return true;
            } else {
                qDebug() << "Failed to set" << portName << "as the active device after updating firmware";
                firmwareUploadStatus = "error";
                emit updateActiveDeviceFirmwareComplete(false);
                return false;
            }
        } else {            
            qDebug() << "Failed to update firmware";
            delete this->pgm;
            firmwareUploadStatus = "error";
            emit updateActiveDeviceFirmwareComplete(false);
            return false;
        }
    } else {
        qDebug() << "Unable to program non-uart devices at this time";
        firmwareUploadStatus = "error";
        emit updateActiveDeviceFirmwareComplete(false);
        return false;
    }
}

QString Agent::getFirmwareUploadStatus() {    
    return this->firmwareUploadStatus;
}

int Agent::getFirmwareUploadProgress() {
    return this->pgm->progress;
}

QThread* Agent::getThread() {
    return this->thread();
}

/*
WflDevice* Agent::createNewWflSerialDevice(QString address) {
    return new WflSerialDevice(deviceName);
}
*/


