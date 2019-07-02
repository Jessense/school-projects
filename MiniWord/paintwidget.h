#ifndef HELLOWIDGET_H
#define HELLOWIDGET_H
#include <QWidget>
#include <QtGui>
class paintWidget : public QWidget
{
    Q_OBJECT
public:
    paintWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
};

#endif // HELLOWIDGET_H
