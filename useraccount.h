#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <QObject>

enum UserStat{
    LOGIN,
    REGISTER,
    RECONNECT,
    CONNECTED
};


class UserAccount : public QObject
{
    Q_OBJECT
protected:
    explicit UserAccount(QObject *parent = nullptr){
        UserAccount::_instance = static_cast<UserAccount*>(this);
    }

    virtual ~UserAccount(){
        if(UserAccount::_instance != nullptr)
        {
            delete UserAccount::_instance;
            UserAccount::_instance = nullptr;
        }
    }

public:
    static UserAccount& get_instance(){
        if(UserAccount::_instance == nullptr)
        {
            UserAccount::_instance = create_instance();
        }

        return *(UserAccount::_instance);
    }

    QString get_username(){return _username;}
    void set_username(QString username){_username = username;}

    QString get_password(){return _password;}
    void set_password(QString password){_password = password;}

    QString get_session_id(){return _session_id;}
    void set_session_id(QString session_id){_session_id = session_id;}

    QString get_branch_name(){return _branch_name;}
    void set_branch_name(QString branch_name){_branch_name = branch_name;}

    QString get_branch_name_en(){return _branch_name_en;}
    void set_branch_name_en(QString branch_name_en){_branch_name_en = branch_name_en;}

    QString get_branch_id(){return _branch_id;}
    void set_branch_id(QString branch_id){_branch_id = branch_id;}

    UserStat get_user_state(){return _state;}
    void set_user_state(UserStat state){_state = state;}

signals:

public slots:

private:
    QString _username;
    QString _password;
    QString _session_id;

    QString _branch_name;
    QString _branch_name_en;
    QString _branch_id;

    UserStat _state;

    static UserAccount *_instance;
    static UserAccount *create_instance(){return new UserAccount();}
};

#endif // USERACCOUNT_H
