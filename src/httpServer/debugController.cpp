#include "debugController.h"

DebugController::DebugController(QObject* parent) : HttpRequestHandler(parent) {

}

void DebugController::service(HttpRequest &request, HttpResponse &response) {
    qDebug() << request.getBody();
    response.write("Debug Controller Page...", true);
}
