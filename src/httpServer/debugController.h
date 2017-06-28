#ifndef DEBUGCONTROLLER_H
#define DEBUGCONTROLLER_H

#include "httprequesthandler.h"

class DebugController : public HttpRequestHandler {
    Q_OBJECT
public:
    DebugController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // DEBUGCONTROLLER_H
