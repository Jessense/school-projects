#ifndef USER_H
#define USER_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>


class User: public QObject
{
    Q_OBJECT
public:
    User(QString str1, QString str2, bool b, int t, int id, QString board,QObject *parent=0);
    User(const User &user);
    ~User();
    void setLogined(bool b);
    void setType(int t);

    QString getUserame();
    QString getPassword();
    int getType();
    bool getLogined();
    int getId();
    QString getBoardManaged();

    friend void operator << (QSqlDatabase db, User *user); //导出用户数据到数据库

    QString username;
    QString password;
    bool logined;
    int type;
    int id;
    QString board_managed;

};

class NormalUser: public User
{
    Q_OBJECT
public:
    NormalUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~NormalUser();
};

class BoardManager: public NormalUser
{
    Q_OBJECT
public:
    BoardManager(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~BoardManager();

};

class Administrator: public User
{
    Q_OBJECT
public:
    Administrator(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~Administrator();
};

class AnonymousUser: public User
{
    Q_OBJECT
public:
    AnonymousUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~AnonymousUser();
};

#endif // USER_H
