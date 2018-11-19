#include "logindialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QDebug>

#include "websocketclient.h"

LoginDialog::LoginDialog()
{
    QSize size(400,300);
    setFixedSize(size);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("愉康大药房 登录"));

    _information_label = new QLabel(tr("欢迎登录愉康大药房订单语音提示系统！"));
    _information_label->setStyleSheet("QLabel {color:blue}");

    _username_label = new QLabel(tr("用户名"));
    _username_edit = new QLineEdit;

    _password_label = new QLabel(tr("密码"));
    _password_edit = new QLineEdit;

    _confirm_passwd_label = new QLabel(tr("确认密码"));
    _confirm_passwd_edit = new QLineEdit;

    _register_label = new RegisterLabel(tr("注册"));
    _login_button = new QPushButton(tr("登录"));

    _branch_label = new QLabel(tr("请选择分店"));
    _branch_list = new QComboBox;

    for(int i = 0; i < 20; i++)
    {
        _branch_list->addItem(tr(yukang_branch[i][0]));
    }

    _username_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    _password_edit->setEchoMode(QLineEdit::Password);
    _password_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    _confirm_passwd_edit->setEchoMode(QLineEdit::Password);
    _confirm_passwd_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));

    _register_label->setStyleSheet("QLabel {color:green; text-decoration: underline}");

    _username_label->setFixedWidth(100);
    _password_label->setFixedWidth(100);
    _confirm_passwd_label->setFixedWidth(100);
     _branch_label->setFixedWidth(100);

    _username_edit->setFixedHeight(26);
    _password_edit->setFixedHeight(26);
    _confirm_passwd_edit->setFixedHeight(26);
    _branch_list->setFixedHeight(26);

    _login_button->setFixedWidth(180);
    _login_button->setFixedHeight(30);

    _main_layout = new QFormLayout;
    _main_layout->addRow("", _information_label);
    _main_layout->addRow(_username_label, _username_edit);
    _main_layout->addRow(_password_label, _password_edit);
    _main_layout->addRow(_confirm_passwd_label, _confirm_passwd_edit);
    _main_layout->addRow(_branch_label, _branch_list);
    _main_layout->addRow(_register_label, _login_button);

    _confirm_passwd_label->hide();
    _confirm_passwd_edit->hide();
    _branch_label->hide();
    _branch_list->hide();

    _main_layout->setFormAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _main_layout->setVerticalSpacing(20);

    setLayout(_main_layout);

    connect(_login_button, SIGNAL(clicked()), this , SLOT(loginButtonClicked()));
    connect(_register_label, SIGNAL(clicked(QPoint)), SLOT(registerLabelClicked()));

    _is_login = true;

    _network_manager = new QNetworkAccessManager;
    connect(_network_manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply){

        _information_label->setStyleSheet("QLabel {color:red}");

        if(reply->error()){
            qDebug(reply->errorString().toLatin1());
            _information_label->setText("网络连接异常，请稍后再试");
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
                WebSocketClient::get_instance().connect_server();
                emit accept();
            }else{
                _information_label->setText("登录失败，用户名或密码错误");
            }
        }else{
            _information_label->setText(answer);
        }
    });

    _multi_part = new QHttpMultiPart(QHttpMultiPart::FormDataType);
}

void LoginDialog::loginButtonClicked()
{
    _information_label->setStyleSheet("QLabel {color:red}");

    if(_username_edit->text().length() == 0)
    {
        _information_label->setText(tr("用户名不能为空"));
      return;
    }

    int passwd_length = _password_edit->text().length();

    if(passwd_length == 0)
    {
        _information_label->setText(tr("密码不能为空"));
        return;
    }else if((passwd_length < 6) || (passwd_length > 16))
    {
        _information_label->setText(tr("密码长度必须在6个字符到16个字符之间"));
        return;
    }

    if(_is_login)
    {
        qDebug("is login");

         _information_label->setText("");

         _network_request.setUrl(QUrl("http://localhost:19966/user/login"));
         _network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

         QUrlQuery qu;
         qu.addQueryItem("", "");
         qu.addQueryItem("username", _username_edit->text());
         qu.addQueryItem("password", _password_edit->text());

         QUrl params;
         params.setQuery(qu);
         _network_manager->post(_network_request, params.toEncoded());

         _information_label->setStyleSheet("QLabel {color:green}");
         _information_label->setText("登陆进行中...");
    }else{
        qDebug("is register");
        qDebug(_branch_list->currentText().toUtf8());

        if(QString::compare(_password_edit->text(), _confirm_passwd_edit->text()))
        {
            _information_label->setText(tr("两次输入的密码不一致，请重新输入密码"));
        }else{
             _information_label->setText("");

             _network_request.setUrl(QUrl("http://localhost:19966/user/register"));
             _network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

             QUrlQuery qu;
             qu.addQueryItem("", "");
             qu.addQueryItem("merchant_name", tr("愉康大药房"));
             qu.addQueryItem("username", _username_edit->text());
             qu.addQueryItem("password", _password_edit->text());
             qu.addQueryItem("branch", _branch_list->currentText());

             QUrl params;
             params.setQuery(qu);
             _network_manager->post(_network_request, params.toEncoded());

             _information_label->setStyleSheet("QLabel {color:green}");
             _information_label->setText("注册进行中...");
        }
    }
}

void LoginDialog::registerLabelClicked()
{
    if(_is_login)
    {
        _is_login = false;
        _login_button->setText(tr("注册"));
        _register_label->setText(tr("登录"));
        _information_label->setText("");
        setWindowTitle(tr("愉康大药房 注册"));
        _information_label->setStyleSheet("QLabel {color:blue}");
        _information_label->setText("欢迎注册愉康大药房订单语音提示系统！");

        _confirm_passwd_label->show();
        _confirm_passwd_edit->show();
        _branch_label->show();
        _branch_list->show();
    }else{
        _is_login = true;
        _login_button->setText(tr("登录"));
        _register_label->setText(tr("注册"));
        _information_label->setText("");
        setWindowTitle(tr("愉康大药房 登录"));
        _information_label->setStyleSheet("QLabel {color:blue}");
        _information_label->setText("欢迎登录愉康大药房订单语音提示系统！");

        _confirm_passwd_label->hide();
        _confirm_passwd_edit->hide();
        _branch_label->hide();
        _branch_list->hide();
    }
}
