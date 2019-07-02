#include "paintwidget.h"
#include "mainwindow.h"
paintWidget::paintWidget(QWidget *parent)
: QWidget(parent)
{
}
void paintWidget::paintEvent(QPaintEvent *event)
{
    /*tempx, tempy为当前点的坐标*/
    int tempx = BEGINX;
    int tempy = BEGINY;


    /*用于绘制文字、选择部分背景、光标的画笔，设置字体*/
    QFont font;
    font.setFamily("文泉驿等宽正黑");
    font.setPointSize(10);
    QPainter painter(this);
    painter.setFont(font);
    QFontMetrics metrics = painter.fontMetrics();
    int stringHeight = metrics.ascent() + metrics.descent(); //行高

    hasSelection = false;
    allText = "";
    selectedText = "";
    linePtr tempLine = article->headLine;
    charPtr tempChar = tempLine->headChar;
    if (tempChar == article->curLine->curChar) //绘制起始光标
        painter.drawLine(QPointF(tempx, tempy-stringHeight), QPointF(tempx, tempy));
    while (tempLine != NULL) {
        if (tempChar == NULL) {
            tempLine = tempLine->nextLine;
            if (tempLine == NULL)
                break;
            tempChar = tempLine->headChar;
            allText += "\n";
            if (tempChar->isSelected)
                selectedText += "\n";
            tempx = BEGINX; //横坐标置为行首
            tempy += stringHeight; //纵坐标下移一行
            if (tempChar == article->curLine->curChar) //绘制行首光标
                painter.drawLine(QPointF(tempx, tempy-stringHeight+1), QPointF(tempx, tempy+1));
        }

        tempChar = tempChar->nextChar;
        if (tempLine != NULL && tempChar != NULL) {
            allText += tempChar->ch;
            painter.drawText(QPointF(tempx, tempy), tempChar->ch); //绘制文字
            if (tempChar->isSelected) {
                painter.save();
                painter.setPen(QColor(0, 160, 230, 125));
                painter.setBrush(QColor(0, 160, 230, 125));
                painter.drawRect(tempx, tempy-stringHeight+1, metrics.width(tempChar->ch) + 3, stringHeight); //绘制选择部分标记
                painter.restore();
                hasSelection = true;
                selectedText += tempChar->ch;

            }
            tempx += metrics.width(tempChar->ch) + 3;

        }
        if (tempChar == article->curLine->curChar)
            painter.drawLine(QPointF(tempx, tempy-stringHeight+1), QPointF(tempx, tempy+1)); //绘制光标
        if(tempx > areaWidth - 40 || tempy > areaHeight - 40) {
            if (tempx > areaWidth - 40) {
                areaWidth += 40;
            }
            else if (tempy > areaHeight - 40) {
                areaHeight += 40;
            }
        }

    }
//    qDebug() << "alltext:" << allText;
//    qDebug() << "selectedtext:" << selectedText;
    qDebug() << "(" << tempx << "," << tempy << ")";
    qDebug() << "row=" << row << ",col=" << col;
}
