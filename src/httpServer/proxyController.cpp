#include "proxyController.h"

ProxyController::ProxyController(QObject* parent) : HttpRequestHandler(parent) {

}

void ProxyController::service(HttpRequest &request, HttpResponse &response) {
    response.write("Proxy Controller Page...", true);
}
