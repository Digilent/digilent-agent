#include "httpRouter.h"
#include "debugController.h"
#include "proxyController.h"


StaticFileController* HttpRouter::staticFileController = 0;

HttpRouter::HttpRouter(QObject* parent) : HttpRequestHandler(parent) {
    // empty
}

void HttpRouter::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s", path.data());

    if (path=="/debug") {
        DebugController().service(request, response);
    } else if (path=="/proxy") {
        ProxyController().service(request, response);
    }
    else{
            staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");
}
