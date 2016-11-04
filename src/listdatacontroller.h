#ifndef LISTDATACONTROLLER_H
#define LISTDATACONTROLLER_H

#include <QList>
#include <QString>
#include "httprequesthandler.h"

class ListDataController: public HttpRequestHandler {
    Q_OBJECT
public:
    ListDataController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    QList<QString> list;
};

#endif // LISTDATACONTROLLER_H
