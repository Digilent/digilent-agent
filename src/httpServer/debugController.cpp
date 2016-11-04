#include "debugController.h"

DebugController::DebugController(QObject* parent) : HttpRequestHandler(parent) {

}

void DebugController::service(HttpRequest &request, HttpResponse &response) {
    response.write("Debug Controller Page...", true);
}
