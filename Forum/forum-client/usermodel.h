#ifndef USERMODEL_H
#define USERMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "user.h"


class UserModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UsernameRole,
        PasswordRole,
        LoginedRole,
        TypeRole,
        IdRole,
        BoardManagedRole
    };

    UserModel(QObject *parent = 0);
//![1]

//    Q_INVOKABLE QObject *curUser();
    Q_INVOKABLE QObject *getUserQ(int id);
//    Q_INVOKABLE User* getUserC(int id);
    void addUser(User *user);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
//    friend void operator >> (QSqlDatabase db, UserModel *userModel); //从数据库导入用户数据

protected:
    QHash<int, QByteArray> roleNames() const;
public:
    QList<User*> m_users;
//![2]
};
//![2]

#endif // USERMODEL_H
