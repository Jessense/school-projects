#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QTimer>
#include "postmodel.h"
#include "usermodel.h"


class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    QList<QTcpSocket*> getClients();


signals:
    void sig_login(QTcpSocket *socket, QString username, QString password);
    void finished_login(QByteArray reply);


public slots:
    virtual void newConnection();
    void Read();
    quint16 Write(QTcpSocket *socket, QByteArray data);
    void Disconnect();
public:
    void login(QTcpSocket *socket, QString username1, QString password1);
    void logout(QTcpSocket *socket, QString str);
    void register1(QTcpSocket *socket, QString str);
    void sendUserModel(QTcpSocket *socket);
    void sendPostModel(QTcpSocket *socket);
    void sendPost(QTcpSocket *socket, int postId);
    void receivePost(QTcpSocket *socket, QString str);
    void deletePost(QTcpSocket *socket, QString str);
    void receiveComment(QTcpSocket *socket, QString str);
    void setBoardManager(QTcpSocket *socket, QString str);


public:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
    PostModel *postModel;
    UserModel *userModel;
    User *user;
    QSqlDatabase db;
};

#endif // BACKEND_H
