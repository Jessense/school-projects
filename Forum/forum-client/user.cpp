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

void User::login(const QString &username1,  const QString &password1)
{
    qDebug() << "User::login()";
    if (username1 == "anonymous") {
        emit sig_login_anonymous();
        return;
    }
    if (username1.length() <= 10 && password1.length() <= 10) {
        QByteArray data("0");
        data.append(QChar(0x200B));
        data.append(username1);
        data.append(QChar(0x200B));
        data.append(password1);
        emit sig_login(data);
    }
}

bool User::register1(const QString &username1, const QString &password1)
{
    if (username1.length() <= 10 && password1.length() <= 10) {
        QByteArray data("9");
        data.append(QChar(0x200B));
        data.append(username1);
        data.append(QChar(0x200B));
        data.append(password1);
        emit sig_register(data);
    }
    return false;
}

void User::logout()
{
    this->logined = false;
    qDebug() << "User::logout()";
    QByteArray data("1");
    data.append(QChar(0x200B));
    emit sig_logout(data);
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

void User::setBoardManager(int userId, QString boardname)
{

}

void User::addPost(const QString &title, const QString &content, const QString &board)
{
    qDebug() << "User::addPost";
}

bool User::canDeletePost(QString author, QString board, int commentCount)
{
    return false;
}

bool User::deletePost(int postId)
{
    return false;
}

void User::addComment(const QString &content, int postId)
{
    qDebug() << "User::addComment()";
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

void NormalUser::addPost(const QString &title, const QString &content, const QString &board)
{
    qDebug() << "NormalUser::addPost";
    QByteArray data("3");
    data.append(QChar(0x200B));
    data.append(title);
    data.append(QChar(0x200B));
    data.append(content);
    data.append(QChar(0x200B));
    data.append(board);
    data.append(QChar(0x200B));
    data.append(username);

    qDebug() << "NormalUser::addPost2";
    emit sig_add_post(data);

}

bool NormalUser::canDeletePost(QString author, QString board, int commentCount)
{
    if (username == author && commentCount == 0) {
        return true;
    }
    return false;
}

bool NormalUser::deletePost(int postId)
{
    QByteArray data("4");
    data.append(QChar(0x200B));
    data.append(QString::number(postId));
    emit sig_add_post(data);
    return false;
}

void NormalUser::addComment(const QString &content, int postId)
{
    QByteArray data("5");
    data.append(QChar(0x200B));
    data.append(QString::number(postId));
    data.append(QChar(0x200B));
    data.append(username);
    data.append(QChar(0x200B));
    data.append(content);


    emit sig_add_comment(data);
}




BoardManager::BoardManager(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    NormalUser (str1, str2, b, t, id, board, parent)
{
    board_managed = board;
}

BoardManager::~BoardManager()
{

}

bool BoardManager::canDeletePost(QString author, QString board, int commentCount)
{
    if (board == board_managed || (author == username && commentCount == 0))
        return true;
    return false;
}

bool BoardManager::deletePost(int postId)
{
    QByteArray data("4");
    data.append(QChar(0x200B));
    data.append(QString::number(postId));
    emit sig_add_post(data);
    return false;
}



Administrator::Administrator(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    User (str1, str2, b, t, id, board, parent)
{

}

Administrator::~Administrator()
{

}

void Administrator::setBoardManager(int userId, QString boardname)
{
    QByteArray data("8");
    data.append(QChar(0x200B));
    data.append(QString::number(userId));
    data.append(QChar(0x200B));
    data.append(boardname);
    emit sig_set_board_manager(data);
}

bool Administrator::canDeletePost(QString author, QString board, int commentCount)
{
    return false;
}

AnonymousUser::AnonymousUser(QString str1, QString str2, bool b, int t, int id, QString board, QObject *parent):
    User (str1, str2, b, t, id, board, parent)
{

}

AnonymousUser::~AnonymousUser()
{

}

bool AnonymousUser::canDeletePost(QString author, QString board, int commentCount)
{
    return false;
}
