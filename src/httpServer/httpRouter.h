#ifndef HTTPROUTER_H
#define HTTPROUTER_H

//HTTP Core Includes
#include "httprequesthandler.h"
#include "staticfilecontroller.h"

class HttpRouter : public HttpRequestHandler {
    Q_OBJECT
public:
    HttpRouter(QObject* _parent=0, QString* activeDevice=0);
    static StaticFileController* staticFileController;
    void service(HttpRequest& request, HttpResponse& response);

private:       
    QString* m_activeDevice;

};

#endif // HTTPROUTER_H
