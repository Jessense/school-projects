#include "backend.h"

Backend::Backend(QObject *parent) : QObject(parent)
{
    user = new User("", "", false, 1, 0, "Nothing");
    postModel = new PostModel();
    userModel = new UserModel();

    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost("localhost", 6547);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(Disconnect()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(Read()));
    tcpSocket->open(QIODevice::ReadWrite);
    Write("Client: Hi, server!");



    connect(user, SIGNAL(sig_login(QByteArray)), this, SLOT(Write(QByteArray))); //user指向其他地址后需重新connect
    connect(user, SIGNAL(sig_logout(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_delete_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_comment(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_set_board_manager(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_register(QByteArray)), this, SLOT(Write(QByteArray)));

    connect(user, SIGNAL(sig_login_anonymous()), this, SLOT(login_anonymous()));


}

QObject *Backend::getUser()
{
    return user;
}

QObject *Backend::getPostModel()
{
    return postModel;
}

QObject *Backend::getUserModel()
{
    return userModel;
}

void Backend::pullUserModel()
{
    Write("7");
}

void Backend::pullPostModel()
{
    Write("2");
}

void Backend::pullPost(int postId)
{
    QByteArray data("6    ");
    QString postid = QString::number(postId);
    data.append(postid);
    for (int i = postid.length(); i < 5; i ++) {
        data.append(" ");
    }
    Write(data);
}




void Backend::logout()
{
    delete user;
    user = new User("", "", false, 1, 0, "Nothing");
    delete postModel;
    postModel = new PostModel();
    connect(user, SIGNAL(sig_login(QByteArray)), this, SLOT(Write(QByteArray))); //user指向其他地址后需重新connect
    connect(user, SIGNAL(sig_logout(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_delete_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_comment(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_set_board_manager(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_register(QByteArray)), this, SLOT(Write(QByteArray)));

    connect(user, SIGNAL(sig_login_anonymous()), this, SLOT(login_anonymous()));

    qDebug() << "Backend::logout()";
    Write("1");
}

void Backend::login_anonymous()
{
    user = new AnonymousUser("anonymous", "", true, 4, -1, "Nothing");
    connect(user, SIGNAL(sig_login(QByteArray)), this, SLOT(Write(QByteArray))); //重新connect
    connect(user, SIGNAL(sig_logout(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_delete_post(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_add_comment(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_set_board_manager(QByteArray)), this, SLOT(Write(QByteArray)));
    connect(user, SIGNAL(sig_register(QByteArray)), this, SLOT(Write(QByteArray)));

    connect(user, SIGNAL(sig_login_anonymous()), this, SLOT(login_anonymous()));

    pullPostModel();
}

void Backend::Disconnect()
{
    qDebug() << "disconnected";
//    Delete(this);
    //TODO
    emit hasDisconnected();

}

void Backend::Error(QAbstractSocket::SocketError)
{
    qDebug() << "connection error";
    qDebug() << tcpSocket->errorString();
//    Delete(this->socketDesc);
    //TODO

}

void Backend::Connected()
{
    emit hasConnected();
}

void Backend::Read()
{
    QByteArray buffer = tcpSocket->readAll(); //TODO
    QString str(buffer);
    if (str[0] == '0') { //login()
        if (str[1] == '1') {
            qDebug() << "client: login success";
            QStringList paralist = str.split("\u200B");

            QString username = paralist[1];
            QString password = paralist[2];
            int type = paralist[3].toInt();
            int id = paralist[4].toInt();
            QString board_managed = paralist[5];

            for (int i = 1; i < 6; i++) {
                qDebug() << paralist[i];
            }

            switch (type) {
                case 1:
                    user = new NormalUser(username, password, true, type, id, board_managed);
                    break;
                case 2:
                    user = new BoardManager(username, password, true, type, id, board_managed);
                    break;
                case 3:
                    user = new Administrator(username, password, true, type, id, board_managed);
                    break;
                default:
                    user = new NormalUser(username, password, true, type, id, board_managed);
                    qDebug() << "register error";
                    break;
            }
            connect(user, SIGNAL(sig_login(QByteArray)), this, SLOT(Write(QByteArray))); //重新connect
            connect(user, SIGNAL(sig_logout(QByteArray)), this, SLOT(Write(QByteArray)));
            connect(user, SIGNAL(sig_add_post(QByteArray)), this, SLOT(Write(QByteArray)));
            connect(user, SIGNAL(sig_delete_post(QByteArray)), this, SLOT(Write(QByteArray)));
            connect(user, SIGNAL(sig_add_comment(QByteArray)), this, SLOT(Write(QByteArray)));
            connect(user, SIGNAL(sig_set_board_manager(QByteArray)), this, SLOT(Write(QByteArray)));
            connect(user, SIGNAL(sig_register(QByteArray)), this, SLOT(Write(QByteArray)));

            connect(user, SIGNAL(sig_login_anonymous()), this, SLOT(login_anonymous()));


            pullPostModel(); //登陆后拉取最新post列表
            emit loginSuccess();
        } else {
            emit loginFailure();
        }
    } else if (str[0] == '2') { //pullPostModel()
//        QString m_author;
//        QString m_time;
//        QString m_title;
//        QString m_content;
//        int m_id;
//        QString m_board;
        QStringList paralist = str.split("\u200B");
        int length2 = paralist[1].toInt();
        postModel = new PostModel(); //清空postModel
        for (int i = 0, p = 1; i < length2 && p < paralist.length(); i ++) {
            Post *tempPost = new Post(paralist[p+2], paralist[p+3], paralist[p+4], paralist[p+6], paralist[p+1].toInt(), paralist[p+5]);
            postModel->addPost(tempPost);

            int commentCount = paralist[p+7].toInt();
            qDebug() << "commentCount:" << commentCount;
            p += 7;
            for (int j = 0; j < commentCount; j ++) {
                Comment *tempComment = new Comment(paralist[p+1], paralist[p+2], paralist[p+3], paralist[p+4].toInt(), paralist[p+5].toInt());
                tempPost->m_commentModel->m_comments.append(tempComment);
                p += 5;
            }
        }
        emit pullPostModelSuccess();

    } else if (str[0] == '3') {
        if (str[1] == '1') {
            emit addPostSuccess();
        }
    } else if (str[0] == '4') {
        if (str[1] == '1') {
            emit deletePostSuccess();
        }
    } else if (str[0] == '5') {
        if (str[1] == '1') {
            emit addCommentSuccess();
        }
    }
    else if (str[0] == '6') {
        if (str[1] == '0') {
            emit pullPostFailure();
            return;
        }
        QStringList paralist = str.split("\u200B");
        Post *tempPost = postModel->getPostC(paralist[1].toInt());
        tempPost->m_author = paralist[2];
        tempPost->m_board = paralist[5];
        tempPost->m_content = paralist[6];
        tempPost->m_time = paralist[3];
        tempPost->m_title = paralist[4];

        delete tempPost->m_commentModel;
        tempPost->m_commentModel = new CommentModel();

        int commentCount = paralist[7].toInt();
        qDebug() << "commentCount:" << commentCount;

        for (int j = 0, p = 7; j < commentCount && p < paralist.length(); j ++) {
            Comment *tempComment = new Comment(paralist[p+1], paralist[p+2], paralist[p+3], paralist[p+4].toInt(), paralist[p+5].toInt());
            tempPost->m_commentModel->m_comments.append(tempComment);
            p += 5;
        }
        emit pullPostSuccess();
    } else if (str[0] == '7') {
        QStringList paralist = str.split("\u200B");
        int userCount = paralist[1].toInt();
        qDebug() << "userCount:" << userCount;
        userModel = new UserModel();
        for (int i = 0, p = 1; i < userCount && p < paralist.length(); i++, p += 6) {
            userModel->addUser(new User(paralist[p+1], paralist[p+2], paralist[p+3].toInt(), paralist[p+4].toInt(), paralist[p+5].toInt(), paralist[p+6]));
        }
        emit pullUserModelSuccess();
    } else if (str[0] == '8') {
        if (str[1] == '1') {
            emit setBoardManagerSuccess();
        } else if (str[1] == '0') {
            emit setBoardManagerFailure();
        }
    } else if (str[0] == '9') {
        if (str[1] == '1') {
            emit registerSuccess();
        } else if (str[1] == '0') {
            emit registerFailure();
        }
    }
}


void Backend::Write(QByteArray data)
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        tcpSocket->write(data);
    else {
        emit hasDisconnected();
    }
}
