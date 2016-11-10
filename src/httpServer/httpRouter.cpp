#include "httpRouter.h"
#include "debugController.h"
#include "proxyController.h"

StaticFileController* HttpRouter::staticFileController = 0;

HttpRouter::HttpRouter(QObject* parent) : HttpRequestHandler(parent) {

}

void HttpRouter::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    qDebug("HttpRouter: path=%s", path.data());

    if (path=="/debug") {       
        qDebug("Routing To Debug Controller");
        DebugController(this).service(request, response);
    } else if (path=="/proxy") {
        qDebug("Routing To Proxy Controller");
        ProxyController().service(request, response);
    }
    else{
        qDebug("Routing To Static Controller");
        staticFileController->service(request, response);
    }

    qDebug("HttpRouter: Request Complete");
}
