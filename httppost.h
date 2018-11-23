#ifndef HTTPPOST_H
#define HTTPPOST_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#pragma execution_character_set("utf-8")

enum HttpPostResult{
    CONNECTION_REFUSED,
    SUCCESS,
    ISLOGIN,
    FAILED
};

class HttpPost : public QObject
{
    Q_OBJECT
public:
    explicit HttpPost(QObject *parent = nullptr);
    ~HttpPost();

    void send_login_post();
    void send_register_post();

signals:
    void login_result(HttpPostResult result);
    void register_result(QString reponse);
    void reconnect_result(HttpPostResult result);

public slots:

private:
    QNetworkAccessManager *_network_manager;
    QNetworkRequest _network_request;
};

#endif // HTTPPOST_H
