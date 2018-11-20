#ifndef HTTPPOST_H
#define HTTPPOST_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

enum PostType{
    LOGIN,
    REGISTER,
    RECONNECT
};

class HttpPost : public QObject
{
    Q_OBJECT
public:
    explicit HttpPost(QObject *parent = nullptr);

    void send_login_post();
    void send_register_post();

    void set_post_type(PostType post_type){ _post_type = post_type;}

signals:

public slots:

private:
    QNetworkAccessManager *_network_manager;
    QNetworkRequest _network_request;

    PostType _post_type;
};

#endif // HTTPPOST_H
