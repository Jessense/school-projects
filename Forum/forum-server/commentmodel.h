#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H
#include <QAbstractListModel>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlDatabase>
#include <QSqlQuery>
//![0]
class Comment
{
public:
    Comment(const QString &author, const QString &time, const QString &content, int postId, int id);
    Comment(const Comment &comment);
    ~Comment();
//![0]

    Q_INVOKABLE QString author() const;
    Q_INVOKABLE QString time() const;
    Q_INVOKABLE QString content() const;
    Q_INVOKABLE int postId() const;
    Q_INVOKABLE int id() const;

    friend void operator << (QSqlDatabase db, Comment *comment);


public:
    QString m_author;
    QString m_time;
    QString m_content;
    int m_postid;
    int m_id;
//![1]
};

class CommentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CommentRoles {
        AuthorRole = Qt::UserRole + 1,
        TimeRole,
        ContentRole,
        PostIdRole,
        IdRole
    };

    CommentModel(QObject *parent = 0);
//![1]

    Q_INVOKABLE void addComment(const QString &author, const QString &content, int postId);
//    Q_INVOKABLE void appendRow(const QString &author, const QString &content);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
//    bool setData(const QVariant &value, int role);
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
public:
    QList<Comment*> m_comments;
//![2]
};
//![2]

#endif // COMMENTMODEL_H
