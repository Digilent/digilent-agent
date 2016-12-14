//HTTP Controllers
#include "httpRouter.h"
#include "debugController.h"

StaticFileController* HttpRouter::staticFileController = 0;

HttpRouter::HttpRouter(Agent* agent, QObject* parent) : HttpRequestHandler(parent) {
    this->agent = agent;
    agentConfigCtrl = new AgentConfigCtrl(this->agent, this);
}

void HttpRouter::service(HttpRequest& request, HttpResponse& response) {

    QByteArray path = request.getPath();
    QByteArray method = request.getMethod();

    qDebug() << "HttpRouter: method = " << method << "path = ", path.data();

    if(method == "OPTIONS"){
        response.setHeader("Access-Control-Allow-Headers", "Content-Type");
        response.setHeader("Access-Control-Allow-Origin", "*");
        response.setHeader("Access-Control-Max-Age", "86400");
        response.write("Options Response....", true);
    }
    else if (path=="/config" || path=="/config/") {
        agentConfigCtrl->service(request, response);
    }
    else if (path=="/debug" || path=="/debug/") {
        qDebug("Routing To Debug Controller");
        DebugController(this).service(request, response);
    }
    else if (path=="/" && method == "POST") {
        if(agent->activeDevice == 0)
        {
            qDebug("No Active Device Selected!!!");
            response.write("No Active Device Selected!!!", true);
            //TODO Send Back Command Response
        }
        else
        {
            QEventLoop loop;
            //Connect slots and signals
            connect(this, SIGNAL(deviceComplete()), &loop, SLOT(quit()));
            connect(agent->activeDevice, SIGNAL(execCommandComplete(QByteArray)), this, SLOT(onComplete(QByteArray)));

            waitingForResponse = true;
            qDebug("::::Request:::: \n %s", request.getBody().data());
            agent->activeDevice->execCommand(request.getBody());

            //Wait for signal that device call has returned
            if(waitingForResponse){
                qDebug("HttpRouter Loop Begin");
                loop.exec();
                qDebug("HttpRouter Loop Done");
            }
            qDebug("::::Response:::: \n %s", reply.data());

            //Add headers and return device call response to original requester
            response.setHeader("Access-Control-Allow-Origin", "*");
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Connection", "close");
            response.setHeader("Content-Type", "application/json");
            response.setStatus(200, "OK");

            response.write(reply, true);

            //Disconnect signals to prevent multiple responses on subsequent calls

           if(disconnect(agent->activeDevice, SIGNAL(execCommandComplete(QByteArray)), this, SLOT(onComplete(QByteArray))))
           {
              qDebug("Succesfully disconnected execCommandComplete signal");
           }
           else
           {
               qDebug("Failed to disconnected execCommandComplete signal");
           }

        }
    }
    else {
        qDebug("Routing To Static Controller");
        staticFileController->service(request, response);
    }
    qDebug("HttpRouter: Request Complete");
}

void HttpRouter::onComplete(QByteArray reply){
    qDebug("HttpRouter::onComplete(): %s", reply.data());
    waitingForResponse = false;
    this->reply = reply;
    //qDebug() << this->reply;
    emit deviceComplete();
}

