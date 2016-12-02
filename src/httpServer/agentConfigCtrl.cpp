#include "agentConfigCtrl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

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

    QJsonDocument reqDoc = QJsonDocument::fromJson(request.getBody());
    if(!reqDoc.isNull()){
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
                res.insert("statusCode", 2147483648);
            } else {
                qDebug("Set Active Device Success");
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
    return e_unknownCommand;
}
