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

        if(reply->error()){
            qDebug(reply->errorString().toLatin1());
            emit login_result(CONNECTION_REFUSED);
            return;
        }

        QString reponse = reply->readAll();
        qDebug(reponse.toUtf8());

        UserStat state = UserAccount::get_instance().get_user_state();

        switch (state) {
            case LOGIN:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(reponse.toUtf8());
                    QJsonObject obj = doc.object();

                    if(!obj["result"].toString().compare("success"))
                    {
                        UserAccount::get_instance().set_branch_name(obj["branch"].toString().toUtf8());
                        UserAccount::get_instance().set_branch_name_en(obj["branch_name_en"].toString().toUtf8());
                        UserAccount::get_instance().set_branch_id(obj["branch_id"].toString().toUtf8());
                        UserAccount::get_instance().set_session_id(obj["token"].toString());
                        qDebug("Login Success!!!");
                        emit login_result(SUCCESS);
                    }else if(!obj["result"].toString().compare("islogin")){
                        emit login_result(ISLOGIN);
                    }else{
                        emit login_result(FAILED);
                    }
                }
                break;
            case REGISTER:
                emit register_result(reponse);
                break;
            case RECONNECT:
                qDebug("Reconnect success");
                {
                    QJsonDocument doc = QJsonDocument::fromJson(reponse.toUtf8());
                    QJsonObject obj = doc.object();

                    if(!obj["result"].toString().compare("success"))
                    {
                        UserAccount::get_instance().set_branch_name(obj["branch"].toString().toUtf8());
                        UserAccount::get_instance().set_branch_name_en(obj["branch_name_en"].toString().toUtf8());
                        UserAccount::get_instance().set_branch_id(obj["branch_id"].toString().toUtf8());
                        UserAccount::get_instance().set_session_id(obj["token"].toString());
                        qDebug("Login Success!!!");
                        emit reconnect_result(SUCCESS);
                    }else if(!obj["result"].toString().compare("islogin")){
                        emit login_result(ISLOGIN);
                    }else{
                        emit reconnect_result(FAILED);
                    }
                }
                break;
            default:
                break;
        }
    });
}

void HttpPost::send_login_post()
{
    _network_request.setUrl(QUrl("http://120.24.61.253:9999/user/login"));
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
    _network_request.setUrl(QUrl("http://120.24.61.253:9999/user/register"));
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

HttpPost::~HttpPost()
{
    qDebug("Http post deconstructed!");
}
