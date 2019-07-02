#include "backend.h"
#include<QDateTime>

Backend::Backend(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    userModel = new UserModel();
    postModel = new PostModel();
    user = new User("", "", false, 1, 0, "Nothing");

    tcpServer = new QTcpServer();

    if (!tcpServer->listen(QHostAddress::Any, 6547))
    {
        qDebug() << "Error! The port is taken by some other service";
    }
    else
    {
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug() <<  "Server started, port is openned";

        db.setHostName("127.0.0.1");
        db.setDatabaseName("forum");
        db.setUserName("root");
        db.setPassword("123456");
        if (db.open()) {
            db >> userModel;
            db >> postModel;
        }
    }


}

void Backend::newConnection()
{

    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(Read()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(Disconnect()));

    clients << clientSocket;
    Write(clientSocket, "Server: Hi, Client!");
    qDebug() << "connection established";

}

void Backend::Read()
{
    QTcpSocket *clientSocket = (QTcpSocket*) sender();
    QByteArray buffer = clientSocket->readAll(); //TODO
    QString str(buffer);
    if (str[0] == '0') {
        QStringList paralist = str.split("\u200B");
        for (int index = 0; index < paralist.length(); index ++) {
            qDebug() << paralist[index];
        }
        login(clientSocket, paralist[1], paralist[2]);
    } else if (str[0] == '1') {
        logout(clientSocket, str);
    } else if (str[0] == '2') {
        sendPostModel(clientSocket);
    } else if (str[0] == '3') {
        receivePost(clientSocket, str);
    } else if (str[0] == '4') {
        deletePost(clientSocket, str);
    } else if (str[0] == '5') {
        receiveComment(clientSocket, str);
    } else if (str[0] == '6') {
        sendPost(clientSocket, str.mid(5, 5).replace(" ", "").toInt());
    } else if (str[0] == '7') {
        sendUserModel(clientSocket);
    } else if (str[0] == '8') {
        setBoardManager(clientSocket, str);
    } else if (str[0] == '9') {
        register1(clientSocket, str);
    }
}

quint16 Backend::Write(QTcpSocket *socket, QByteArray data)
{
    return socket->write(data);
}

void Backend::Disconnect()
{
    qDebug() << "disconnect";
    clients.removeAt(clients.indexOf((QTcpSocket*)sender()));

}

void Backend::login(QTcpSocket *socket, QString username, QString password)
{

    if (username == "anonymous") {
        return;
    }

    QMutableListIterator<User*> i(userModel->m_users);
    while (i.hasNext()) {
        User *temp = i.next();
        if (temp->username == username && temp->password == password) {
            temp->setLogined(true);
            QByteArray data("01");
            data.append(QChar(0x200B));
            data.append(temp->username);
            data.append(QChar(0x200B));
            data.append(temp->password);
            data.append(QChar(0x200B));
            data.append(QString::number(temp->type));
            data.append(QChar(0x200B));
            data.append(QString::number(temp->id));
            data.append(QChar(0x200B));
            data.append(temp->board_managed);

            Write(socket, data);
            return;
        }
    }
    QByteArray data("00");
    Write(socket, data);
    qDebug() << "login failed";
}

void Backend::logout(QTcpSocket *socket, QString str)
{
    QStringList paralist = str.split("\u200B");
    int userid = paralist[1].toInt();
    QMutableListIterator<User*> i(userModel->m_users);
    while (i.hasNext()) {
        User *temp = i.next();
        if (temp->id = userid) {
            temp->setLogined(false);
            return;
        }
    }
}

void Backend::register1(QTcpSocket *socket, QString str)
{
    QStringList paralist = str.split("\u200B");
    QString username = paralist[1];
    QString password = paralist[2];

    User* tempUser;
    QMutableListIterator<User*> i(userModel->m_users);
    while (i.hasNext()) {
        if (i.next()->username == username) {
            Write(socket, "90");
            return;
        }
    }
    int newId;
    if (userModel->m_users.length() == 0)
        newId = 0;
    else
        newId = userModel->m_users.last()->id + 1;
    tempUser = new NormalUser(username, password, false, 1, newId, "Nothing");

    userModel->m_users.append(tempUser);
    db << tempUser;
    Write(socket, "91");
}

void Backend::sendUserModel(QTcpSocket *socket)
{
//    username = str1;
//    password = str2;
//    logined = b;
//    type = t;
//    id = i;
//    board_managed = board;
    QMutableListIterator<User*> i(userModel->m_users);
    QByteArray data("7");
    data.append(QChar(0x200B));
    data.append(QString::number(userModel->m_users.length()));
    while (i.hasNext()) {
        i.next();
        User *temp = i.value();
        data.append(QChar(0x200B));
        data.append(temp->username);
        data.append(QChar(0x200B));
        data.append(temp->password);
        data.append(QChar(0x200B));
        data.append(temp->logined);
        data.append(QChar(0x200B));
        data.append(QString::number(temp->type));
        data.append(QChar(0x200B));
        data.append(QString::number(temp->id));
        data.append(QChar(0x200B));
        data.append(temp->board_managed);
    }

    Write(socket, data);

}

void Backend::sendPostModel(QTcpSocket *socket)
{
    QMutableListIterator<Post*> i(postModel->m_posts);
    QByteArray data("2");
    QString length2 = QString::number(postModel->m_posts.length()); //1-10: method, length
    data.append(QChar(0x200B));
    data.append(length2);
    while (i.hasNext()) {
        i.next();
        Post* temp = i.value();

        data.append(QChar(0x200B));
        data.append(QString::number(temp->id()));
        data.append(QChar(0x200B));
        data.append(temp->author());
        data.append(QChar(0x200B));
        data.append(temp->time());
        data.append(QChar(0x200B));
        data.append(temp->title());
        data.append(QChar(0x200B));
        data.append(temp->board());
        data.append(QChar(0x200B));
        data.append(temp->content());
        data.append(QChar(0x200B));
        data.append(QString::number(temp->m_commentModel->m_comments.length()));

        QMutableListIterator<Comment*> j(temp->m_commentModel->m_comments);
        while (j.hasNext()) {
//            QString m_author;
//            QString m_time;
//            QString m_content;
//            int m_postid;
//            int m_id;
            j.next();
            Comment *tempComment = j.value();


            data.append(QChar(0x200B));
            data.append(tempComment->author());
            data.append(QChar(0x200B));
            data.append(tempComment->time());
            data.append(QChar(0x200B));
            data.append(tempComment->content());
            data.append(QChar(0x200B));
            data.append(QString::number(tempComment->postId()));
            data.append(QChar(0x200B));
            data.append(QString::number(tempComment->id()));
        }

    }
    Write(socket, data);
}

void Backend::sendPost(QTcpSocket *socket, int postId)
{

    Post* temp = postModel->getPostC(postId);
    if (!temp) {
        Write(socket, "60");
        return;
    }
    QByteArray data("61");
    data.append(QChar(0x200B));
    data.append(QString::number(temp->id()));
    data.append(QChar(0x200B));
    data.append(temp->author());
    data.append(QChar(0x200B));
    data.append(temp->time());
    data.append(QChar(0x200B));
    data.append(temp->title());
    data.append(QChar(0x200B));
    data.append(temp->board());
    data.append(QChar(0x200B));
    data.append(temp->content());
    data.append(QChar(0x200B));
    data.append(QString::number(temp->m_commentModel->m_comments.length()));

    QMutableListIterator<Comment*> j(temp->m_commentModel->m_comments);
    while (j.hasNext()) {
//            QString m_author;
//            QString m_time;
//            QString m_content;
//            int m_postid;
//            int m_id;
        j.next();
        Comment *tempComment = j.value();


        data.append(QChar(0x200B));
        data.append(tempComment->author());
        data.append(QChar(0x200B));
        data.append(tempComment->time());
        data.append(QChar(0x200B));
        data.append(tempComment->content());
        data.append(QChar(0x200B));
        data.append(QString::number(tempComment->postId()));
        data.append(QChar(0x200B));
        data.append(QString::number(tempComment->id()));
    }
    Write(socket, data);
}

void Backend::receivePost(QTcpSocket *socket, QString str)
{

    QStringList paralist = str.split("\u200B");
    if (paralist.length() < 5) {
        Write(socket, "30");
        return;
    }
    int newId;
    if (postModel->m_posts.length() == 0)
        newId = 0;
    else
        newId = postModel->m_posts.last()->id() + 1;

    Post *tempPost = new Post(paralist[4], QDateTime::currentDateTime().toString(), paralist[1], paralist[2], newId, paralist[3]);
    postModel->addPost(tempPost);
    db << tempPost;
    Write(socket, "31");
}

void Backend::deletePost(QTcpSocket *socket, QString str)
{
    QStringList paralist = str.split("\u200B");
    if (paralist.length() < 2) {
        Write(socket, "40");
        return;
    }
    int postId = paralist[1].toInt();
    QMutableListIterator<Post*> i(postModel->m_posts);
    while (i.hasNext()) {
        i.next();
        Post* temp = i.value();
        if (temp->id() == postId) {
            qDebug() << "post " << temp->id() << " deleted";
            i.remove();
            delete temp;
            temp = nullptr;
            bool ok = db.open();
            if (ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM posts WHERE id = :id");
                query.bindValue(":id", postId);
                query.exec();

                query.prepare("DELETE FROM comment WHERE postid = :id");
                query.bindValue(":id", postId);
                query.exec();
            } else {
                qDebug() << "delete post failed";
            }
            Write(socket, "41");
            return;
        }

    }
    Write(socket, "40");

}

void Backend::receiveComment(QTcpSocket *socket, QString str)
{
    QStringList paralist = str.split("\u200B");
    if (paralist.length() < 4) {
        Write(socket, "50");
        return;
    }

    int newId;
    if (postModel->getPostC(paralist[1].toInt())->commentModelC()->m_comments.length() == 0)
        newId = 0;
    else
        newId = postModel->getPostC(paralist[1].toInt())->commentModelC()->m_comments.last()->id() + 1;

    Comment *tempComment = new Comment(paralist[2], QDateTime::currentDateTime().toString(), paralist[3], paralist[1].toInt(), newId);
    postModel->getPostC(paralist[1].toInt())->commentModelC()->m_comments.append(tempComment);
    db << tempComment;

    Write(socket, "51");
}

void Backend::setBoardManager(QTcpSocket *socket, QString str)
{
    QStringList paralist = str.split("\u200B");
    if (paralist.length() < 3) {
        Write(socket, "80");
        return;
    }
    int userId = paralist[1].toInt();
    QString boardname = paralist[2];
    for (int i = 0; i < userModel->m_users.length(); i++) {
        User *temp = userModel->m_users.at(i);
        qDebug() << temp->username << "," << temp->id << "," << temp->board_managed;
        if (temp->id == userId && temp->board_managed != boardname) {
            if (boardname == "Nothing") {
                userModel->m_users.replace(i, new NormalUser(temp->username, temp->password, temp->logined, 1, temp->id, "Nothing"));
                bool ok = db.open();
                if (ok) {
                    QSqlQuery query;
                    query.prepare("UPDATE users SET type=1, board='Nothing' WHERE id = :id");
                    query.bindValue(":id", userId);
                    query.exec();
                } else {
                    qDebug() << "update users failed";
                }
            }
            else {
                userModel->m_users.replace(i, new BoardManager(temp->username, temp->password, temp->logined, 2, temp->id, boardname));
                bool ok = db.open();
                if (ok) {
                    QSqlQuery query;
                    query.prepare("UPDATE users SET type=2, board = :board WHERE id = :id");
                    query.bindValue(":board", boardname);
                    query.bindValue(":id", userId);
                    query.exec();
                } else {
                    qDebug() << "update users failed";
                }
            }
            qDebug() << "setBM success";
            Write(socket, "81");
            return;
        }
    }
    Write(socket, "80");
}

QList<QTcpSocket*> Backend::getClients()
{
    return clients;
}


