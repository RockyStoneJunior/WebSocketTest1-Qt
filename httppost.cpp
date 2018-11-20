#include "httppost.h"

#include "useraccount.h"

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

        QString answer = reply->readAll();
        qDebug(answer.toUtf8());

        if(_is_login)
        {
            QStringRef answer_ref(&answer, 0, 8);
            QString success("success:");
            if(!answer_ref.compare(success))
            {
                UserAccount::get_instance().set_branch_name(answer.mid(8));
                emit accept();
            }else{
                //_information_label->setText("登录失败，用户名或密码错误");
            }
        }else{
            //_information_label->setText(answer);
        }
    });
}

void HttpPost::send_login_post()
{
    _network_request.setUrl(QUrl("http://localhost:19966/user/login"));
    _network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery qu;
    qu.addQueryItem("", "");
    qu.addQueryItem("username", UserAccount::get_instance().get_username();
    qu.addQueryItem("password", UserAccount::get_instance().get_password();

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
    qu.addQueryItem("username", UserAccount::get_instance().get_username();
    qu.addQueryItem("password", UserAccount::get_instance().get_password();
    qu.addQueryItem("branch", UserAccount::get_instance().get_branch_name();

    QUrl params;
    params.setQuery(qu);
    _network_manager->post(_network_request, params.toEncoded());
}
