#ifndef USER_H
#define USER_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>


class User: public QObject
{
    Q_OBJECT
public:
//    explicit User(QObject *parent = nullptr);
//    User(QObject *parent);
    User(QString str1, QString str2, bool b, int t, int id, QString board,QObject *parent=0);
    User(const User &user);
    ~User();
    Q_INVOKABLE void login(const QString &username1, const QString &password1);
    Q_INVOKABLE bool register1(const QString &username1, const QString &password1);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void setLogined(bool b);
    Q_INVOKABLE void setType(int t);

    Q_INVOKABLE QString getUserame();
    Q_INVOKABLE QString getPassword();
    Q_INVOKABLE int getType();
    Q_INVOKABLE bool getLogined();
    Q_INVOKABLE int getId();
    Q_INVOKABLE QString getBoardManaged();


    Q_INVOKABLE virtual void setBoardManager(int userId, QString boardname); //设置版主
    Q_INVOKABLE virtual void addPost(const  QString &title,const QString &content, const QString &board); //发帖
    Q_INVOKABLE virtual bool canDeletePost(QString author, QString board, int commentCount); //判断是否可以删除特定帖子
    Q_INVOKABLE virtual bool deletePost(int postId); //删帖
    Q_INVOKABLE virtual void addComment(const QString &content, int postId); //评论

//    friend void operator << (QSqlDatabase db, User *user); //导出用户数据到数据库


    QString username;
    QString password;
    bool logined;
    int type;
    int id;
    QString board_managed;
signals:
    void sig_login(QByteArray data);
    void sig_login_anonymous();
    void sig_register(QByteArray data);
    void sig_logout(QByteArray data);
    void sig_add_post(QByteArray data);
    void sig_delete_post(QByteArray data);
    void sig_add_comment(QByteArray data);
    void sig_set_board_manager(QByteArray data);


public slots:


private:

};

class NormalUser: public User
{
    Q_OBJECT
public:
    NormalUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~NormalUser();
    Q_INVOKABLE virtual void addPost(const  QString &title,const QString &content, const QString &board);
    Q_INVOKABLE virtual bool canDeletePost(QString author, QString board, int commentCount);
    Q_INVOKABLE virtual bool deletePost(int postId);
    Q_INVOKABLE virtual void addComment(const QString &content, int postId);


};

class BoardManager: public NormalUser
{
    Q_OBJECT
public:
    BoardManager(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~BoardManager();
    Q_INVOKABLE virtual bool canDeletePost(QString author, QString board, int commentCount);
    Q_INVOKABLE virtual bool deletePost(int postId);

};

class Administrator: public User
{
    Q_OBJECT
public:
    Administrator(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~Administrator();
    Q_INVOKABLE virtual void setBoardManager(int userId, QString boardname);
    Q_INVOKABLE virtual bool canDeletePost(QString author, QString board, int commentCount);
};

class AnonymousUser: public User
{
    Q_OBJECT
public:
    AnonymousUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent=0);
    ~AnonymousUser();
    Q_INVOKABLE virtual bool canDeletePost(QString author, QString board, int commentCount);
};

#endif // USER_H
