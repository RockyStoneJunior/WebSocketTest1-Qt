#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <QObject>

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

    QString get_user_name(){return _user_name;}
    void set_user_name(QString user_name){_user_name = user_name;}

    QString get_pass_word(){return _pass_word;}
    void set_pass_word(QString pass_word){_pass_word = pass_word;}

    QString get_session_id(){return _session_id;}
    void set_session_id(QString session_id){_session_id = session_id;}

    QString get_branch_name(){return _branch_name;}
    void set_branch_name(QString branch_name){_branch_name = branch_name;}

signals:

public slots:

private:
    QString _user_name;
    QString _pass_word;
    QString _session_id;

    QString _branch_name;

    static UserAccount *_instance;
    static UserAccount *create_instance(){return new UserAccount();}
};

#endif // USERACCOUNT_H
