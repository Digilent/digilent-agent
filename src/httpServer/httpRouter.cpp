//QT core includes
#include <QtCore>

//HTTP Controllers
#include "httpRouter.h"
#include "debugController.h"
#include "proxyController.h"

StaticFileController* HttpRouter::staticFileController = 0;

HttpRouter::HttpRouter(QObject* _parent, OsDevice* _activeDevice) : HttpRequestHandler(_parent) {
    activeDevice = _activeDevice;
}

void HttpRouter::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    qDebug("HttpRouter: path=%s", path.data());

    if (path=="/debug") {       
        qDebug("Routing To Debug Controller");
        DebugController(this).service(request, response);
    } else if (path=="/proxy") {
        qDebug("Routing To Proxy Controller");

        //Setup device request slot and initiate device call
        connect(activeDevice, SIGNAL(commandComplete(QString)), this, SLOT(onComplete(QString)));
        activeDevice->execCommand("none");

        //Wait for signal that device call has returned
        QEventLoop loop;
        connect(this, SIGNAL(deviceComplete()), &loop, SLOT(quit()));
        loop.exec();

        //Return device call response to original requester
        response.write(reply.toUtf8());
    }
    else{
        qDebug("Routing To Static Controller");
        staticFileController->service(request, response);
    }

    qDebug("HttpRouter: Request Complete");
}

void HttpRouter::onComplete(QString _reply){

    qDebug("HttpRouter::onComplete()");
    reply = _reply;
    emit deviceComplete();
}

