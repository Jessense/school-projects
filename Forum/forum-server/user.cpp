#include <QDebug>
#include <QDateTime>
#include <QAbstractListModel>
#include <QDebug>
#include <QString>
#include "user.h"


User::User(QString str1, QString str2, bool b, int t, int i, QString board,QObject *parent):
    QObject (parent)
{
    username = str1;
    password = str2;
    logined = b;
    type = t;
    id = i;
    board_managed = board;
}



User::User(const User &user)
{
    this->username = user.username;
    this->password = user.password;
    this->logined = user.logined;
    this->type = user.type;
    this->id = user.id;
    this->board_managed = user.board_managed;
}


void User::setLogined(bool b)
{
    logined = b;
}

void User::setType(int t)
{
    type = t;
}


QString User::getUserame()
{
    return username;
}

QString User::getPassword()
{
    return password;
}

int User::getType()
{
    return type;
}

bool User::getLogined()
{
    return logined;
}

int User::getId()
{
    return id;
}


void operator <<(QSqlDatabase db, User *user)
{
    bool ok = db.open();
    if (ok) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (id, username, password, logined, type, board) "
                      "VALUES (:id, :username, :password, :logined, :type, :board)");
        query.bindValue(":id", user->id);
        query.bindValue(":username", user->username);
        query.bindValue(":password", user->password);
        query.bindValue(":logined", user->logined);
        query.bindValue(":type", user->type);
        query.bindValue(":board", user->board_managed);
        query.exec();
    } else {
        qDebug() << "insert failed";
    }
}

QString User::getBoardManaged()
{
    return board_managed;
}

User::~User()
{

}


NormalUser::NormalUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    User (str1, str2, b, t, id, board, parent)
{

}

NormalUser::~NormalUser()
{

}

BoardManager::BoardManager(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    NormalUser (str1, str2, b, t, id, board, parent)
{
    board_managed = board;
}

BoardManager::~BoardManager()
{

}


Administrator::Administrator(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    User (str1, str2, b, t, id, board, parent)
{

}

Administrator::~Administrator()
{

}



AnonymousUser::AnonymousUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    User (str1, str2, b, t, id, board, parent)
{

}

AnonymousUser::~AnonymousUser()
{

}
