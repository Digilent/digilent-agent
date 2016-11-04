#include "requestmapper.h"
#include "helloworldcontroller.h"
#include "listdatacontroller.h"

RequestMapper::RequestMapper(QObject* parent) : HttpRequestHandler(parent) {
    // empty
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s", path.data());

    if (path=="/" || path=="/hello") {
        HelloWorldController().service(request, response);
    }
    else if (path=="/list") {
        ListDataController().service(request, response);
    }
    else {
        response.setStatus(404,"Not found");
        response.write("The URL is wrong, no such document.",true);
    }

    qDebug("RequestMapper: finished request");
}
