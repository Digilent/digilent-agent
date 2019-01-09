#ifndef HTTPROUTER_H
#define HTTPROUTER_H

//QT core includes
#include <QtCore>

//HTTP Core Includes
#include "../core/agent.h"
#include "httprequesthandler.h"
#include "staticfilecontroller.h"
#include "agentConfigCtrl.h"

//OpenScope device includes
#include "../wflDevice/wflDevice.h"

class HttpRouter : public HttpRequestHandler {
    Q_OBJECT
public:
    HttpRouter(Agent* agent, QObject* parent=0);

    Agent *agent;
    static StaticFileController* staticFileController;

    void service(HttpRequest& request, HttpResponse& response);

signals:
    void deviceComplete();

private slots:
    void onComplete(QByteArray reply);

private:           
    AgentConfigCtrl *agentConfigCtrl;
    QByteArray reply;
    bool waitingForResponse;
};

#endif // HTTPROUTER_H

