#ifndef BACKEND_H
#define BACKEND_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QTcpSocket>
#include "postmodel.h"
#include "usermodel.h"
#include "user.h"
#include "commentmodel.h"


class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE QObject *getUser();
    Q_INVOKABLE QObject *getPostModel();
    Q_INVOKABLE QObject *getUserModel();

    Q_INVOKABLE void pullUserModel();
    Q_INVOKABLE void pullPostModel();
    Q_INVOKABLE void pullPost(int postId);


signals:
    void Delete();
    void loginSuccess();
    void loginFailure();
    void registerSuccess();
    void registerFailure();
    void addCommentSuccess();
    void addPostSuccess();
    void deletePostSuccess();
    void setBoardManagerSuccess();
    void setBoardManagerFailure();



    void pullPostModelSuccess();
    void pullPostSuccess();
    void pullPostFailure();
    void pullUserModelSuccess();

    void hasDisconnected();
    void hasConnected();
//    void goHome();
//    void goPost();
public slots:
    void Read(); //客户端有数据发来时触发
    void Write(QByteArray data); //发送数据给客户端
    void Disconnect(); //客户端断开时触发
    void Error(QAbstractSocket::SocketError); //连接出现错误时触发
    void Connected();
    void logout();
    void login_anonymous();
public:
    User *user;
    PostModel *postModel;
    UserModel *userModel;

    QTcpSocket *tcpSocket;

};

#endif // BACKEND_H
