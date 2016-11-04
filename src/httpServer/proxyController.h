#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

#include "httprequesthandler.h"

class ProxyController : public HttpRequestHandler {
    Q_OBJECT
public:
    ProxyController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // PROXYCONTROLLER_H
