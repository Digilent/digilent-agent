#include "agentConfigCtrl.h"
#include "../../lib/digilent/qtHttp/httpClient.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QUrl>

AgentConfigCtrl::AgentConfigCtrl(Agent* agent, QObject* parent) : HttpRequestHandler(parent) {
    this->agent = agent;
}

void AgentConfigCtrl::service(HttpRequest &request, HttpResponse &response) {

    //Add headers and return device call response to original requester
    qDebug("---------------setting headers");
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Cache-Control", "no-cache");
    response.setHeader("Connection", "close");
    response.setHeader("Content-Type", "application/json");
    response.setStatus(200, "OK");

    QByteArray body = request.getBody();
    QJsonDocument reqDoc;

    if(body[0] != '{') {
        bool ok = false;
        unsigned int jsonLength = body.mid(0, body.indexOf("\r")).toUInt(&ok);
        if(!ok) {
            response.write("Invalid OSJB", true);
            return;
        }

        QByteArray cmdJson = body.mid((body.indexOf("\n") + 1), jsonLength - 2);
        QByteArray cmdBinary = body.mid(body.indexOf("\n") + 1 + jsonLength);

        //Write binary to file
        QFile file(QDir::tempPath() + QString("/wflFirmware.hex"));

        file.open(QIODevice::WriteOnly);
        file.write(cmdBinary);
        file.close();

        reqDoc = QJsonDocument::fromJson(cmdJson);
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
            cmdRespObjs.insert(i, processCommand(cmdObj));
        }

        QJsonObject jsonResponse = QJsonObject();
        jsonResponse.insert("agent", cmdRespObjs);
        response.write(QJsonDocument(jsonResponse).toJson());
        return;
    }
    //Default Response
    response.write("Invalid Request", true);
}

QJsonObject AgentConfigCtrl::processCommand(QJsonObject cmdObj){
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
        case e_setActiveDevice:
        {
            QString port = cmdObj.value("device").toString();
            bool success = agent->setActiveDeviceByName(port);
            if(!success) {
                qDebug("Set Active Device Failed");
                //setActiveDevice() failed, return error code
                res.insert("statusCode", qint64(2147483648));
            } else {
                qDebug("Set Active Device Success");
            }
            break;
        }
        case e_uploadFirmware:
        {
            qDebug() << "Updating Firmware";
            bool enterBootloader = cmdObj.value("enterBootloader").toBool();
            QString firmwareUrl = cmdObj.value("firmwareUrl").toString();

            //Download firmware if necessary
            if(firmwareUrl != ""){
               //TODO - Download firmware from URL
            }

            if(!this->agent->updateActiveDeviceFirmware(QDir::tempPath() + QString("/wflFirmware.hex"), enterBootloader)){
                res.insert("statusCode", qint64(123456789));
            }
            break;
        }
        case e_enterJsonMode:
        {
            QByteArray devResp = this->agent->activeDevice->writeRead("{\"mode\":\"JSON\"}\r\n");
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
                    res.insert("statusCode", qint64(666));
                }
            } else {
                qDebug() << "e_enterJsonMode - No response from deveice";
                res.insert("statusCode", qint64(666));
            }
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
    if(cmdString == "getInfo"){
        return e_getInfo;
    }
    if(cmdString == "enumerateDevices"){
        return e_enumerateDevices;
    }
    if(cmdString == "setActiveDevice"){
        return e_setActiveDevice;
    }
    if(cmdString == "uploadFirmware") {
        return e_uploadFirmware;
    }
    if(cmdString == "enterJsonMode") {
        return e_enterJsonMode;
    }
    return e_unknownCommand;
}
