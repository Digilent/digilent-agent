#include "agentConfigCtrl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

AgentConfigCtrl::AgentConfigCtrl(QObject* parent) : HttpRequestHandler(parent) {
    this->agent = new Agent();

}

void AgentConfigCtrl::service(HttpRequest &request, HttpResponse &response) {

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
    response.write(agent->getVersion(), true);
}

QJsonObject AgentConfigCtrl::processCommand(QJsonObject cmdObj){
    QJsonObject res = QJsonObject();
    QString command = cmdObj.value("command").toString();
    res.insert("command", command);
    res.insert("status", 0);

    //Switch over command
    switch(parseCmd(command)){
        case e_getInfo:
        {
            QJsonObject version = QJsonObject();
            version.insert("major", agent->getMajorVersion());
            version.insert("minor", agent->getMinorVersion());
            version.insert("patch", agent->getPatchVersion());
            res.insert("version", version);
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
    return e_unknownCommand;
}
