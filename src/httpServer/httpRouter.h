#ifndef HTTPROUTER_H
#define HTTPROUTER_H

//HTTP Core Includes
#include "httprequesthandler.h"
#include "staticfilecontroller.h"

class HttpRouter : public HttpRequestHandler {
    Q_OBJECT
public:
    HttpRouter(QObject* _parent=0);
    static StaticFileController* staticFileController;
    void service(HttpRequest& request, HttpResponse& response);

private:       

};

#endif // HTTPROUTER_H
