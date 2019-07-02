#ifndef MODEL_H
#define MODEL_H
#include <QAbstractListModel>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "commentmodel.h"
//![0]
class Post: public QObject
{
    Q_OBJECT
public:
    Post(QObject *parent=0);
    Post(const QString &author, const QString &time, const QString &title, const QString &content, int id, const QString &board, QObject *parent=0);
//    Post(const Post &post);
    ~Post();
//![0]

    Q_INVOKABLE QString author() const;
    Q_INVOKABLE QString time() const;
    Q_INVOKABLE QString title() const;
    Q_INVOKABLE QString content() const;
    Q_INVOKABLE int id() const;
    Q_INVOKABLE QString board() const;
    Q_INVOKABLE QObject* commentModelQ();
    Q_INVOKABLE CommentModel* commentModelC();
    Q_INVOKABLE int commentCount();

//    friend void operator << (QSqlDatabase db, Post *post); //将帖子导出至数据库

public:
    QString m_author;
    QString m_time;
    QString m_title;
    QString m_content;
    int m_id;
    QString m_board;
public:
    CommentModel* m_commentModel;
//![1]
};

class PostModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PostRoles {
        AuthorRole = Qt::UserRole + 1,
        TimeRole,
        TitleRole,
        ContentRole,
        IdRole,
        BoardRole,
    };

    PostModel(QObject *parent = 0);
//![1]

    Q_INVOKABLE void addPost(Post *post);
    Q_INVOKABLE QObject *getPostQ(int id);
    Q_INVOKABLE Post *getPostC(int id);
//    friend void operator >> (QSqlDatabase db, PostModel *postModel); //从数据库导入帖子数据

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
public:
    QList<Post*> m_posts;
//![2]
};
//![2]

#endif // POSTMODEL_H
