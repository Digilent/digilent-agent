#ifndef WFLHTTPDEVICE_H
#define WFLHTTPDEVICE_H

//QT core includes
#include <QUrl>

#include "wflDevice.h"
#include "../httpClient/httpClient.h"

class WflHttpDevice : public WflDevice {
    Q_OBJECT

public:
    WflHttpDevice();
    WflHttpDevice(QUrl _url);

    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);
    void setUrl(QUrl _url);

    QUrl url;

signals:
    void commandComplete();

public slots:
    void onHttpComplete(QNetworkReply *reply);
    void onWriteReadResponse(QByteArray reply);

private:
    HttpClient* httpClient;
};

#endif // WFLHTTPDEVICE_H
