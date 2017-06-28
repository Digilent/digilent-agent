#include "agentConfigCtrl.h"
#include "../../lib/digilent/qtHttp/httpClient.h"

#include <QtConcurrent>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QThread>
#include <QUrl>

AgentConfigCtrl::AgentConfigCtrl(Agent* agent, QObject* parent) : HttpRequestHandler(parent) {
    qDebug() << "AgentConfigCtrl::AgentConfigCtrl()" << "thread: " << QThread::currentThread();
    this->agent = agent;
}

void AgentConfigCtrl::service(HttpRequest &request, HttpResponse &response) {

    //Move service to agent thread to execute commands
    qDebug() << "AgentConfigCtrl::Service()" << "thread: " << QThread::currentThread();

    //Add headers and return device call response to original requester
    qDebug("---------------setting headers");
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Cache-Control", "no-cache");
    response.setHeader("Connection", "close");
    response.setHeader("Content-Type", "application/json");
    response.setStatus(200, "OK");

    QByteArray body = request.getBody();
    QJsonDocument reqDoc;
    QByteArray cmdBinary = QByteArray();

    if(body[0] != '{') {

        //Process chunk1 (JSON)
        QByteArray chunk = body;
        bool ok = false;
        unsigned int chunk1Length = chunk.mid(0, chunk.indexOf("\r")).toUInt(&ok, 16);
        if(!ok) {
            response.write("Invalid Chunk Formatting", true);
            return;
        }
        QByteArray cmdJson = chunk.mid((chunk.indexOf("\n") + 1), chunk1Length);
        reqDoc = QJsonDocument::fromJson(cmdJson);

        //Process Chunk
        chunk = body.mid(body.indexOf("\n") + 3 + chunk1Length);
        unsigned int chunk2Length = chunk.mid(0, chunk.indexOf("\r")).toUInt(&ok, 16);
        if(!ok) {
            response.write("Invalid Chunk Formatting", true);
            return;
        }
        cmdBinary.append(chunk.mid((chunk.indexOf("\n") + 1), chunk2Length));
    } else {
        //JSON COMMAND
        reqDoc = QJsonDocument::fromJson(request.getBody());
    }

    if(!reqDoc.isNull()) {
        QJsonObject reqObj = reqDoc.object();
        QJsonArray agentCmds = reqObj.value("agent").toArray();
        QJsonArray cmdRespObjs;

        //Iterate over commands
        for(int i=0; i<agentCmds.size(); i++){
            QJsonObject cmdObj = agentCmds[i].toObject();
            QJsonObject resp = processCommand(cmdObj, cmdBinary);
            cmdRespObjs.insert(i, resp);

            //Check if an error occured, if so don't continue processing commands
            double status = resp.value("statusCode").toDouble();
            if(status > 2147483647) {
                QJsonObject jsonResponse = QJsonObject();
                jsonResponse.insert("agent", cmdRespObjs);
                response.write(QJsonDocument(jsonResponse).toJson());
                return;
            }
        }

        QJsonObject jsonResponse = QJsonObject();
        jsonResponse.insert("agent", cmdRespObjs);
        response.write(QJsonDocument(jsonResponse).toJson());
        return;
    }
    //Default Response
    response.write("Invalid Request", true);
}

QJsonObject AgentConfigCtrl::processCommand(QJsonObject cmdObj, QByteArray data){
    QJsonObject res = QJsonObject();
    QString command = cmdObj.value("command").toString();
    res.insert("command", command);
    res.insert("statusCode", 0);

    //Switch over command
    switch(parseCmd(command)){
        case e_enumerateDevices:
        {
            QJsonArray devices = QJsonArray();
            QVector<QString> deviceNames = agent->enumerateDevices();

            for(int i=0; i<deviceNames.size(); i++)
            {
                devices.insert(devices.size(), deviceNames[i]);
            }
            res.insert("devices", devices);
            break;
        }
        case e_getInfo:
        {
            QJsonObject version = QJsonObject();
            version.insert("major", agent->getMajorVersion());
            version.insert("minor", agent->getMinorVersion());
            version.insert("patch", agent->getPatchVersion());
            res.insert("version", version);
            break;
        }
        case e_saveToTempFile:
        {
            //Prase file name
            QString fileName = cmdObj.value("fileName").toString();
            if(fileName == "") {
                res.insert("statusCode", qint64(0x80000666));
                break;
            }

            //Write binary data to file
            QFile file(QDir::tempPath() + "/" + fileName);

            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
            break;
        }
        case e_setActiveDevice:
        {
            QString port = cmdObj.value("device").toString();
            bool success = agent->setActiveDeviceByName(port);
            if(!success) {
                qDebug("Set Active Device Failed");
                //setActiveDevice() failed, return error code
                res.insert("statusCode", qint64(0x80000666));
            } else {
                qDebug("Set Active Device Success");
            }
            break;
        }
        case e_enterJsonMode:
        {
            QByteArray devResp;
            devResp = this->agent->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");
            qDebug() << "~~~~ENTER JSON MODE~~~~~~" << devResp;

            QJsonDocument resDoc = QJsonDocument::fromJson(devResp);
            if(!resDoc.isNull()) {
                QJsonObject resObj = resDoc.object();
                QString mode = resObj.value("mode").toString();
                if(mode == "JSON")
                {
                    res.insert("statusCode", qint64(0));
                }
                else
                {
                    //No response from the device, something else must have it open
                    //releaseActiveDevice(); - WFL will release the device if it should be released due to not entering JSON mode
                    qDebug() << "Bad Response To Enter JSON Mode Command";
                    res.insert("statusCode", qint64(0x80000666));
                }
            } else {
                qDebug() << "e_enterJsonMode - No response from deveice";
                res.insert("statusCode", qint64(0x80000666));
            }
            break;
        }
        case e_uploadFirmware:
        {
            qDebug() << "Uploading Firmware";
            bool enterBootloader = cmdObj.value("enterBootloader").toBool();
            QString firmwarePath = cmdObj.value("firmwarePath").toString();

            //Download firmware if necessary
            if(firmwarePath != ""){
               //TODO - Download firmware from URL
            }

            //Using Concurrent
            QFuture<bool> future = QtConcurrent::run(this->agent, &Agent::updateActiveDeviceFirmware, QDir::tempPath() + QString("/" + firmwarePath), enterBootloader);
            res.insert("statusCode", qint64(0));
            break;
        }
        case e_updateFirmwareGetStatus:
        {
            res.insert("command", command);
            res.insert("statusCode", qint64(0));
            res.insert("status", this->agent->getFirmwareUploadStatus());
            res.insert("progress", this->agent->getFirmwareUploadProgress());
            break;
        }        
        case e_releaseActiveDevice:
        {
            this->agent->releaseActiveDevice();
            break;
        }
        case e_unknownCommand:
            default:
                res.insert("command", command);
                res.insert("status", "Unknown Command");
            break;
        }

    return res;
}

AgentConfigCtrl::CmdCode AgentConfigCtrl::parseCmd(QString cmdString){    
    if(cmdString == "enterJsonMode") {
        return e_enterJsonMode;
    }
    if(cmdString == "enumerateDevices"){
        return e_enumerateDevices;
    }
    if(cmdString == "getInfo"){
        return e_getInfo;
    }
    if(cmdString == "releaseActiveDevice") {
        return e_releaseActiveDevice;
    }
    if(cmdString == "saveToTempFile"){
        return e_saveToTempFile;
    }
    if(cmdString == "setActiveDevice"){
        return e_setActiveDevice;
    }
    if(cmdString == "updateFirmwareGetStatus") {
        return e_updateFirmwareGetStatus;
    }
    if(cmdString == "uploadFirmware") {
        return e_uploadFirmware;
    }   
    return e_unknownCommand;
}
