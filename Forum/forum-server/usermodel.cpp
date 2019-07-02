#include <QAbstractListModel>
#include <QDateTime>
#include <QDebug>
#include "usermodel.h"

UserModel::UserModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QObject *UserModel::getUserQ(int id)
{
    if (id == -1)
        return new AnonymousUser("anonymouse", "", true, 4, -1, "Nothing");
    QMutableListIterator<User*> i(m_users);
    while (i.hasNext()) {
        User* tempUser = i.next();
        if (tempUser->getId() == id && tempUser->getLogined() == true) {
            switch (tempUser->type) {
                case 1:
                    return new NormalUser(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                case 2:
                    return new BoardManager(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                case 3:
                    return new Administrator(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                default:
                    qDebug() << "login error";
                    break;
            }
        }

    }
    return nullptr;
}

User *UserModel::getUserC(int id)
{
    if (id == -1)
        return new AnonymousUser("anonymouse", "", true, 4, -1, "Nothing");
    QMutableListIterator<User*> i(m_users);
    while (i.hasNext()) {
        User* tempUser = i.next();
        if (tempUser->getId() == id && tempUser->getLogined() == true) {
            switch (tempUser->type) {
                case 1:
                    return new NormalUser(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                case 2:
                    return new BoardManager(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                case 3:
                    return new Administrator(tempUser->username, tempUser->password, true, tempUser->type, tempUser->id, tempUser->board_managed);
                default:
                    qDebug() << "login error";
                    break;
            }
        }

    }
    return nullptr;
}


void UserModel::addUser(User *user)
{
    m_users.append(user);
}


int UserModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_users.count();
}

QVariant UserModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_users.count())
        return QVariant();

    const User *user = m_users[index.row()];
    if (role == UsernameRole)
        return user->username;
    else if (role == PasswordRole)
        return user->password;
    else if (role == LoginedRole)
        return user->logined;
    else if (role == TypeRole)
        return user->type;
    else if (role == IdRole)
        return user->id;
    else if (role == BoardManagedRole)
        return user->board_managed;
    return QVariant();
}

void operator >>(QSqlDatabase db, UserModel *userModel)
{
    bool ok = db.open();
    if (ok) {
        QSqlQuery query;
        query.exec("SELECT * FROM users");
        while (query.next()) {
//            qDebug() << "sql:"<< query.value(1);
            userModel->addUser(new User(query.value(1).toString(), query.value(2).toString(), query.value(3).toBool(), query.value(4).toInt(), query.value(0).toInt(), query.value(5).toString()));
        }
    }

}

//![0]
QHash<int, QByteArray> UserModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[UsernameRole] = "username";
    roles[PasswordRole] = "password";
    roles[LoginedRole] = "logined";
    roles[TypeRole] = "type";
    roles[IdRole] = "id";
    roles[BoardManagedRole] = "board_managed";
    return roles;
}
//![0]
