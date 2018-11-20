#include "httppost.h"

#include "useraccount.h"

#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QNetworkRequest>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

HttpPost::HttpPost(QObject *parent) : QObject(parent)
{
    _network_manager = new QNetworkAccessManager;
    connect(_network_manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply){

        //_information_label->setStyleSheet("QLabel {color:red}");

        if(reply->error()){
            qDebug(reply->errorString().toLatin1());
            //_information_label->setText("网络连接异常，请稍后再试");
            return;
        }

        QString reponse = reply->readAll();
        qDebug(reponse.toUtf8());

        QJsonDocument doc = QJsonDocument::fromJson(reponse.toUtf8());
        QJsonObject obj = doc.object();

        qDebug("result from json:" + obj["result"].toString().toUtf8());
        qDebug("branch from json:" + obj["branch"].toString().toUtf8());
        qDebug("token from json:" + obj["token"].toString().toUtf8());


        QList<QByteArray> headerList = reply->rawHeaderList();
        foreach(QByteArray head, headerList) {
            qDebug() << head << ":" << reply->rawHeader(head);
        }

        _post_type = LOGIN;

        switch (_post_type) {
            case LOGIN:
                {
                    if(!obj["result"].toString().compare("success"))
                    {
                        UserAccount::get_instance().set_branch_name(obj["branch"].toString().toUtf8());
                        UserAccount::get_instance().set_session_id(obj["token"].toString());
                        qDebug("Login Success!!!");
                        //emit accept();
                    }else{
                        //_information_label->setText("登录失败，用户名或密码错误");
                    }
                }
                break;
            case REGISTER:
                //_information_label->setText(reponse);
                break;
            case RECONNECT:

                break;
            default:
                break;
        }
    });
}

void HttpPost::send_login_post()
{
    _network_request.setUrl(QUrl("http://localhost:19966/user/login"));
    _network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery qu;
    qu.addQueryItem("", "");
    qu.addQueryItem("username", UserAccount::get_instance().get_username());
    qu.addQueryItem("password", UserAccount::get_instance().get_password());

    QUrl params;
    params.setQuery(qu);
    _network_manager->post(_network_request, params.toEncoded());
}

void HttpPost::send_register_post()
{
    _network_request.setUrl(QUrl("http://localhost:19966/user/register"));
    _network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery qu;
    qu.addQueryItem("", "");
    qu.addQueryItem("merchant_name", tr("愉康大药房"));
    qu.addQueryItem("username", UserAccount::get_instance().get_username());
    qu.addQueryItem("password", UserAccount::get_instance().get_password());
    qu.addQueryItem("branch", UserAccount::get_instance().get_branch_name());

    QUrl params;
    params.setQuery(qu);
    _network_manager->post(_network_request, params.toEncoded());
}
