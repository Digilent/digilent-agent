#ifndef AGENTCONFIGCTRL_H
#define AGENTCONFIGCTRL_H

#include <QJsonObject>

#include "httprequesthandler.h"
#include "../core/agent.h"


class AgentConfigCtrl : public HttpRequestHandler {
    Q_OBJECT

public:
    AgentConfigCtrl(Agent* agent, QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void startUpdateActiveDeviceFirmware(QString hexPath, bool enterBootloader);

private:
    enum CmdCode {
        e_enumerateDevices,
        e_getInfo,
        e_setActiveDevice,
        e_saveToTempFile,
        e_uploadFirmware,
        e_updateFirmwareGetStatus,
        e_updateWaveFormsLiveBrowser,
        e_enterJsonMode,
        e_releaseActiveDevice,
        e_unknownCommand,
    };

    Agent *agent;

    QJsonObject processCommand(QJsonObject cmdObj, QByteArray data = QByteArray());
    CmdCode parseCmd(QString cmdString);
};

#endif // AGENTCONFIGCTRL_H
