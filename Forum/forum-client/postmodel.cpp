#include "postmodel.h"
#include <QDateTime>
#include <QAbstractListModel>
#include <QDebug>
#include <QSqlError>
#include <QString>


Post::Post(QObject *parent):
    QObject(parent)
{
    m_commentModel = new CommentModel;
}

Post::Post(const QString &author, const QString &time, const QString &title, const QString &content, int id, const QString &board, QObject *parent):
    QObject (parent)
{
    this->m_author = author;
    this->m_time = time;
    this->m_title = title;
    this->m_content = content;
    this->m_id = id;
    this->m_board = board;
    m_commentModel = new CommentModel;
}


Post::~Post()
{

}

QString Post::author() const
{
    return m_author;
}

QString Post::time() const
{
    return m_time;
}

QString Post::title() const
{
    return m_title;
}

QString Post::content() const
{
    return m_content;
}

int Post::id() const
{
    return m_id;
}

QString Post::board() const
{
    return m_board;
}


CommentModel *Post::commentModelC()
{
    return m_commentModel;
}

int Post::commentCount()
{
    qDebug() << "commentCount:" << m_commentModel->m_comments.length();
    return m_commentModel->m_comments.length();
}

QObject* Post::commentModelQ()
{
    return m_commentModel;
}

//void operator <<(QSqlDatabase db, Post *post)
//{
//    bool ok = db.open();
//    if (ok) {
//        QSqlQuery query;
//        query.prepare("INSERT INTO posts (id, author, pubDate, title, content, board) "
//                      "VALUES (:id, :author, :pubDate, :title, :content, :board)");
//        query.bindValue(":id", post->m_id);
//        query.bindValue(":author", post->m_author);
//        query.bindValue(":pubDate", post->m_time);
//        query.bindValue(":title", post->m_title);
//        query.bindValue(":content", post->m_content);
//        query.bindValue(":board", post->m_board);
//        query.exec();
//    } else {
//        qDebug() << "insert post failed";
//        throw 233;
//    }

//}

PostModel::PostModel(QObject *parent)
    : QAbstractListModel(parent)
{
}


void PostModel::addPost(Post *post)
{

//    qDebug() << "addPost addr:" << post;
    m_posts.append(post);
}

Post *PostModel::getPostC(int id)
{
    QMutableListIterator<Post*> i(m_posts);
    while (i.hasNext()) {
        Post* tempPost = i.next();
        if (tempPost->id() == id) {
            return tempPost;
        }

    }
    return nullptr;
}

QObject* PostModel::getPostQ(int id)
{
    QMutableListIterator<Post*> i(m_posts);
    while (i.hasNext()) {
        Post* tempPost = i.next();
        if (tempPost->id() == id) {
            qDebug() << "returned post id:" << tempPost->id() << ", comments: " << tempPost->commentModelC()->m_comments.length();
//            qDebug() << "tempPost addr:" << tempPost;
            return tempPost;
        }

    }
    return nullptr;
}


//void operator >>(QSqlDatabase db, PostModel *postModel)
//{
//    bool ok = db.open();
//    if (ok) {
//        QSqlQuery query;
//        query.exec("SELECT * FROM posts");
//        while (query.next()) {
//            postModel->addPost(new Post(query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(0).toInt(), query.value(5).toString()));
//        }
//        for (int i = 0; i < postModel->m_posts.length(); i ++) {
//            query.exec("SELECT * FROM comment WHERE postid=" + QString::number(postModel->m_posts.at(i)->id()));
//            while (query.next()) {
//                postModel->m_posts.at(i)->commentModelC()->m_comments.append(new Comment(query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toInt(), query.value(5).toInt()));
//            }
//        }
//    } else {
//        throw 233;
//    }

//}


int PostModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_posts.count();
}

QVariant PostModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_posts.count())
        return QVariant();

    Post *post = m_posts[index.row()];
    if (role == AuthorRole)
        return post->author();
    else if (role == TimeRole)
        return post->time();
    else if (role == TitleRole)
        return post->title();
    else if (role == ContentRole)
        return post->content();
    else if (role == IdRole)
        return post->id();
    else if (role == BoardRole)
        return post->board();
    return QVariant();
}

//![0]
QHash<int, QByteArray> PostModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AuthorRole] = "author";
    roles[TimeRole] = "time";
    roles[TitleRole] = "title";
    roles[ContentRole] = "content";
    roles[IdRole] = "id";
    roles[BoardRole] = "board";
    return roles;
}
//![0]

