#include "commentmodel.h"
#include <QDateTime>
#include <QDebug>

Comment::Comment(const QString &author, const QString &time, const QString &content, int postId, int id)
    : m_author(author), m_time(time), m_content(content), m_postid(postId), m_id(id)
{
}

Comment::Comment(const Comment &comment)
{
    this->m_author = comment.m_author;
    this->m_time = comment.m_time;
    this->m_content = comment.m_content;
    this->m_postid = comment.m_postid;
    this->m_id = comment.m_id;
}

Comment::~Comment()
{

}

QString Comment::author() const
{
    return m_author;
}

QString Comment::time() const
{
    return m_time;
}


QString Comment::content() const
{
    return m_content;
}

int Comment::postId() const
{
    return m_postid;
}

int Comment::id() const
{
    return m_id;
}

void operator <<(QSqlDatabase db, Comment *comment)
{
    bool ok = db.open();
    if (ok) {
        QSqlQuery query;
        query.prepare("INSERT INTO comment (author, pubDate, content, postid, commentid) "
                      "VALUES (:author, :pubDate, :content, :postid, :commentid)");
        query.bindValue(":author", comment->m_author);
        query.bindValue(":pubDate", comment->m_time);
        query.bindValue(":content", comment->m_content);
        query.bindValue(":postid", comment->m_postid);
        query.bindValue(":commentid", comment->m_id);
        query.exec();
    } else {
        qDebug() << "insert post failed";
    }
}


CommentModel::CommentModel(QObject *parent)
    : QAbstractListModel(parent)
{
}


void CommentModel::addComment(const QString &author, const QString &content, int postId)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_comments << new Comment(author, QDateTime::currentDateTime().toString(), content, postId, rowCount());
    endInsertRows();
    //    m_comments.append(new Comment(author, QDateTime::currentDateTime().toString(), content, rowCount()));
}


int CommentModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_comments.count();
}

QVariant CommentModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_comments.count())
        return QVariant();

    const Comment *comment = m_comments[index.row()];
    if (role == AuthorRole)
        return comment->author();
    else if (role == TimeRole)
        return comment->time();
    else if (role == ContentRole)
        return comment->content();
    else if (role == PostIdRole)
        return comment->postId();
    else if (role == IdRole)
        return comment->id();
    return QVariant();
}

//![0]
QHash<int, QByteArray> CommentModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AuthorRole] = "author";
    roles[TimeRole] = "time";
    roles[ContentRole] = "content";
    roles[PostIdRole] = "postId";
    roles[IdRole] = "id";
    return roles;
}
//![0]

