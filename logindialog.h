#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QObject>
#include <QDialog>

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include "registerlabel.h"
#include "useraccount.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog();

private slots:
    void loginButtonClicked();
    void registerLabelClicked();

private:
    QLabel *_information_label;

    QLabel *_username_label;
    QLineEdit *_username_edit;

    QLabel *_password_label;
    QLineEdit *_password_edit;

    QLabel *_confirm_passwd_label;
    QLineEdit *_confirm_passwd_edit;

    RegisterLabel *_register_label;
    QPushButton *_login_button;

    QLabel *_branch_label;
    QComboBox *_branch_list;

    QFormLayout *_main_layout;

    bool _is_login;

    QNetworkAccessManager *_network_manager;
    QNetworkRequest _network_request;
};

extern const char *yukang_branch[20][2];

#endif // LOGINDIALOG_H
