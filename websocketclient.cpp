#include "websocketclient.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QSound>

WebSocketClient* WebSocketClient::_instance = nullptr;

WebSocketClient::WebSocketClient(QObject *parent):
    QObject(parent)
{

}

void WebSocketClient::connect_server()
{
    connect(&_web_socket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&_web_socket, &QWebSocket::disconnected, this, &WebSocketClient::onClosed);
    connect(&_web_socket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);

    connect(&_web_socket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            this, [=](QAbstractSocket::SocketError error){
                        qDebug(_web_socket.errorString().toLatin1());

                        if(!_timer->isActive())
                        {
                            qDebug("Timer is not active");
                            _timer->start(3000);

                            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                                                      + "(" + UserAccount::get_instance().get_username() + ")" +
                                                      "      连接中...");
                        }
    });

    _timer = new QTimer();
    connect(_timer, &QTimer::timeout,
            this, [=](){
                    qDebug("time out");
                    _http_post.send_login_post();
    });

    connect(&_http_post, SIGNAL(reconnect_result(HttpPostResult)), this, SLOT(on_reconnect_result(HttpPostResult)));

    _web_socket.open(QUrl("ws://120.24.61.253:9999/new_order"));
}

void WebSocketClient::onConnected()
{
    qDebug("Connected");
    _timer->stop();

    QJsonObject json;
    json["type"] = QJsonValue("login");
    QJsonObject form;
    form["token"] = QJsonValue(UserAccount::get_instance().get_session_id());
    form["username"] = QJsonValue(UserAccount::get_instance().get_username());
    form["branch"] = QJsonValue(UserAccount::get_instance().get_branch_name());
    form["branch_id"] = QJsonValue(UserAccount::get_instance().get_branch_id());
    json["form"] = QJsonValue(form);

    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    _web_socket.sendTextMessage(strJson);
}

void WebSocketClient::send_heart()
{
    UserStat state = UserAccount::get_instance().get_user_state();

    if(state == CONNECTED)
    {
        QJsonObject json;
        json["type"] = QJsonValue("heart");
        QJsonObject form;
        form["token"] = QJsonValue(UserAccount::get_instance().get_session_id());
        form["username"] = QJsonValue(UserAccount::get_instance().get_username());
        form["branch"] = QJsonValue(UserAccount::get_instance().get_branch_name());
        form["branch_id"] = QJsonValue(UserAccount::get_instance().get_branch_id());
        json["form"] = QJsonValue(form);

        QJsonDocument doc(json);
        QString strJson(doc.toJson(QJsonDocument::Compact));

        _web_socket.sendTextMessage(strJson);
    }
}

void WebSocketClient::onTextMessageReceived(QString message)
{

    UserStat state = UserAccount::get_instance().get_user_state();

    switch (state) {
    case LOGIN:
    {
        qDebug("case LOGIN message: " + message.toUtf8());
        if(!message.compare("success"))
        {
            UserAccount::get_instance().set_user_state(CONNECTED);
            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                                      + "(" + UserAccount::get_instance().get_username() + ")" +
                                      "      已连接");
            emit connect_server_result(true);
        }else{
            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                                      + "(" + UserAccount::get_instance().get_username() + ")" +
                                      "      连接失败");
            emit connect_server_result(false);
        }
    }
        break;

    case REGISTER:

        break;

    case RECONNECT:
    {
        qDebug("case RECONNECT message: " + message.toUtf8());
        if(!message.compare("success"))
        {
            UserAccount::get_instance().set_user_state(CONNECTED);
            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                                      + "(" + UserAccount::get_instance().get_username() + ")" +
                                      "      已连接");
        }else{
            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                                      + "(" + UserAccount::get_instance().get_username() + ")" +
                                      "      连接失败");
        }
    }
        break;

    case CONNECTED:
        {
            if(!message.compare("[WS]NewOrder"))
            {
                QSound::play(":/sound/New order.wav");
            }
            qDebug("case CONNECTED message: " + message.toUtf8());
        }
        break;

    default:
        break;
    }
}

void WebSocketClient::onClosed()
{
    qDebug("Disconnected");

    UserAccount::get_instance().set_user_state(RECONNECT);
    _timer->start(3000);

    emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name()
                              + "(" + UserAccount::get_instance().get_username() + ")" +
                              "      连接中...");
}

void WebSocketClient::on_reconnect_result(HttpPostResult result)
{
    switch (result) {
    case SUCCESS:
        _timer->stop();
        _web_socket.open(QUrl("ws://120.24.61.253:9999/new_order"));
        break;
    case FAILED:
        break;
    case CONNECTION_REFUSED:
        break;
    default:
        break;
    }
}
