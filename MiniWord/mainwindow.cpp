#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <fstream>
#include <QDebug>
#include <QLayout>
#include <QFont>

/*初始化全局变量*/
docPtr article = (docPtr)malloc(sizeof(doc));
int areaWidth = 400;
int areaHeight = 400;
int col = 0;
int row = 1;
int last_arrow = 0;
QString allText = "";
QString preText = "";
QString selectedText = "";
QString register_text = "";
bool hasSelection = false;

/*初始化*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    /*初始化文档、第一个行结点、第一个字符结点*/
    article->headLine = (linePtr)malloc(sizeof(struct line));
    article->headLine->headChar = (charPtr)malloc(sizeof(struct character));
    article->headLine->headChar->prevChar = article->headLine->headChar;
    article->headLine->headChar->nextChar = NULL;
    article->headLine->curChar = article->headLine->headChar;
    article->headLine->length = 0;
    article->headLine->prevLine = article->headLine;
    article->headLine->nextLine = NULL;
    article->curLine = article->headLine;
    article->linum = 1;

    /*文件和窗口*/
    curFile = "新文件.txt";
    setWindowTitle(curFile);
    isUntitled = true;

    /*键盘与聚焦*/
    ui->setupUi(this);
//    this->setFocusPolicy(Qt::StrongFocus);
    this->setAttribute(Qt::WA_InputMethodEnabled, true);
    this->setMouseTracking(true);
    this->setWindowTitle("MiniWord");


    /*滚动区域*/
    area= new paintWidget;
    setCentralWidget(area);
    area->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//    setFixedSize(1200,700);//设定窗口初始可视大小为1200*700
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Light); //scrollArea对象的背景色设为Light
    scrollArea->setWidget(area); //将画布添加到滚动条范围中
    scrollArea->widget()->setMinimumSize(areaWidth, areaHeight);//滚动条初始化大小设为2000*800
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(scrollArea);//将滚动条加到中心区
//    scrollArea->verticalScrollBar()->setValue(100);
    scrollArea->setFocusPolicy(Qt::NoFocus);
    scrollArea->setAttribute(Qt::WA_InputMethodEnabled, true);

    /*查找替换对话框*/
    fandcDlg = new QDialog(this);
    fandcDlg->setWindowTitle(tr("查找与替换"));
    findLineEdit = new QLineEdit(fandcDlg);
    findLineEdit->setPlaceholderText(tr("请输入要查找的内容"));
    changeLineEdit = new QLineEdit(fandcDlg);
    changeLineEdit->setPlaceholderText(tr("请输入要替换的内容"));
    QPushButton *btn1= new QPushButton(tr("查找"), fandcDlg);
    QPushButton *btn2= new QPushButton(tr("替换"), fandcDlg);
    QVBoxLayout *layout= new QVBoxLayout(fandcDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(changeLineEdit);
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    connect(btn1, SIGNAL(clicked()), this, SLOT(findText()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(changeText()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********事件管理：输入法事件、键盘事件、鼠标事件**********/
/*输入法事件*/
void MainWindow::inputMethodEvent(QInputMethodEvent *e)
{
    QString temp_str = e->commitString();
    insert(temp_str);
    qDebug() << "inputmethod";
}
/*键盘事件*/
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int index;
    if (e->key() >= 0x20 && e->key() <= 0x7e) { //输入可打印字符
        QString str1 = e->text();
        newChar(str1[0]);
    }
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //回车
        QString temp_str = "\n";
        insert(temp_str);
    }
    else if (e->modifiers() == Qt::ShiftModifier){ //shift+方向键选择文本
        if (e->key() == Qt::Key_Left) {
            if (!hasSelection)
                last_arrow = 0;
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
            cursorLeft(); //向左移动一个
            qDebug() << "shift select";
            showText();
        }
        else if (e->key() == Qt::Key_Right) {
            if (!hasSelection)
                last_arrow = 1;
            cursorRight();
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
            showText();

        }

        else if (e->key() == Qt::Key_Up) {
            if (row > 1) {
                if (!hasSelection)
                    last_arrow = 0;
                int target_col = col;
                int target_row = row - 1;
                if (col > article->curLine->prevLine->length)
                    target_col = article->curLine->prevLine->length;
                while (row != target_row || col != target_col) {
                    article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
                    cursorLeft();
                }
                showText();
            }
        }
        else if (e->key() == Qt::Key_Down) {
            if (row < article->linum) {
                if (hasSelection)
                    last_arrow = 1;
                int target_row = row + 1;
                int target_col = col;
                if (col > article->curLine->nextLine->length)
                    target_col = article->curLine->nextLine->length;
                while (row != target_row || col != target_col) {
                    cursorRight();
                    article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
                    showText();
                }
            }
        }
        else if (e->key() == Qt::Key_Home) {
            while (col > 0) {
                article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
                cursorLeft();
                showText();
            }

        }
        else if (e->key() == Qt::Key_End) {
            while (col < article->curLine->length) {
                cursorRight();
                article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
                showText();
            }
        }
    }
    else {
        switch (e->key()) {
            /*退格键，删除光标前的一个字符*/
            case Qt::Key_Backspace: {
                if (hasSelection)
                    delSelect();
                delChar();
                break;
            }
            case Qt::Key_Tab: {
                insert("  ");
                break;

            }

            case Qt::Key_Left: {
                if (hasSelection) {
                    if (last_arrow == 1) {
                        for (index = 0; index < selectedText.length(); index++) {
                            cursorLeft();
                        }
                    }

                    resetSelect();
                }
                else
                    cursorLeft();
                break;

            }
            case Qt::Key_Right: {
                if (hasSelection) {
                    if (last_arrow == 0) {
                        for (index = 0; index < selectedText.length(); index++) {
                            cursorRight();
                        }
                    }

                    resetSelect();
                }
                else
                    cursorRight();
                break;

            }
            case Qt::Key_Up: {
                if (hasSelection)
                    resetSelect();
                cursorUp();
                break;

            }
            case Qt::Key_Down: {
                if (hasSelection)
                    resetSelect();
                cursorDown();
                break;

            }
            case Qt::Key_Home: {
                if (hasSelection)
                    resetSelect();
                cursorHome();
                break;

            }
            case Qt::Key_End: {
                if (hasSelection)
                    resetSelect();
                cursorEnd();
                break;

            }
        case Qt::Key_Delete: { //delete键的作用，若有选择部分则删除选择部分，否则删除光标后一个字符
            if(hasSelection) {
                delSelect();
            }
            else {
                delNextChar();
            }
        }
            default:
                break;
        }
    }
    showText();

}
/*鼠标点击定位*/
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (hasSelection)
            resetSelect();

        int mx = e->x() - BEGINX + scrollArea->horizontalScrollBar()->value();
        int my = e->y() - BEGINY - 45 + scrollArea->verticalScrollBar()->value();
        qDebug() << "(" << mx << "," << my << ")";
        int target_row;
        int target_col;
        if (my < 0)
            return;
        target_row = my/20 + 1;
        if (target_row > article->linum)
            target_row = article->linum;
        while (row < target_row) {
            cursorRight();
        }
        while (row > target_row) {
            cursorLeft();
        }

        target_col = (mx+10)/20;
        if (target_col > article->curLine->length) {
            target_col = article->curLine->length;
        }
        else if (target_col < 0) {
            target_col = 0;
        }

        while (col < target_col) {
            cursorRight();
        }
        while (col > target_col) {
            cursorLeft();
        }
        showText();
    }
}
/*鼠标移动选择*/
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

    if(e->buttons() & Qt::LeftButton) {
        int mx = e->x() - BEGINX + scrollArea->horizontalScrollBar()->value();
        int my = e->y() - BEGINY - 45 + scrollArea->verticalScrollBar()->value();
        int temp_row = row;
        int temp_col = col;
        int target_row;
        int target_col;
        if (my < 0)
            return;
        target_row = my/20 + 1;
        if (target_row > article->linum)
            target_row = article->linum;
        while (row < target_row) {
            if (!hasSelection)
                last_arrow = 1;
            cursorRight();
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
        }
        while (row > target_row) {
            if (!hasSelection)
                last_arrow = 0;
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
            cursorLeft();
        }

        target_col = (mx+10)/20;
        if (target_col > article->curLine->length) {
            target_col = article->curLine->length;
        }
        else if (target_col < 0) {
            target_col = 0;
        }

        while (col < target_col) {
            if (!hasSelection)
                last_arrow = 1;
            cursorRight();
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
        }
        while (col > target_col) {
            if (!hasSelection)
                last_arrow = 0;
            article->curLine->curChar->isSelected = !article->curLine->curChar->isSelected;
            cursorLeft();
        }
        if (row != temp_row || col != temp_col)
            showText();
    }
}
/*鼠标右键菜单*/
void MainWindow::contextMenuEvent(QContextMenuEvent *e) //鼠标右键菜单
{
    QMenu *menu = new QMenu();
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(ui->actionCopy_C);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(ui->actionCut_X);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(ui->actionPaste_V);
    menu->addSeparator();
    menu->addSeparator();
    menu->exec(e->globalPos());
    delete menu;
}


/**********接口，调用paintwidget.cpp文件中的repaint，将文本、光标、蓝色选择背景刷新至屏幕***********/
void MainWindow::showText()
{
    int cursorx = BEGINX + 20*col;
    int cursory = BEGINY + 20*row;
    int scrollx = scrollArea->horizontalScrollBar()->value();
    int scrolly  = scrollArea->verticalScrollBar()->value();
    area->repaint();
    scrollArea->widget()->setMinimumSize(areaWidth, areaHeight); //动态增加滚动区域大小
    /*控制自动滚动*/
    if (cursorx < scrollx + 40) {
        scrollArea->horizontalScrollBar()->setValue(cursorx - 40);
    }
    if (cursorx > this->width() + scrollx - 40) {
        scrollArea->horizontalScrollBar()->setValue(cursorx - this->width() + 40);
    }
    if (cursory < scrolly + 40) {
        scrollArea->verticalScrollBar()->setValue(cursory - 40);
    }
    if (cursory > this->height() + scrolly - 100) {
        scrollArea->verticalScrollBar()->setValue(cursory - this->height() + 100);
        scrolly -= 20;
    }
    qDebug() << "scrollx:" << scrollx << "; scrolly:" << scrolly << "; cursorx" << cursorx << "; cursory:" << cursory << "; W:" <<this->width() << "; H:" << this->height();
}

/**********当前指针控制（光标会同步移动）**********/
/*光标下移，curChar同步下移*/
void MainWindow::cursorDown()
{
    if (row == article->linum)
        return;
    if (article->curLine->nextLine != NULL) {
        row++;
    }
    if (col > article->curLine->nextLine->length) { //如果列号大于下一行长度，则列号为下一行长度
        col = article->curLine->nextLine->length;
    }
    article->curLine = article->curLine->nextLine;
    article->curLine->curChar = article->curLine->headChar;
    for (int index = 0; index < col; index++) {
        article->curLine->curChar = article->curLine->curChar->nextChar;
    }

}
/*光标上移，curChar同步上移*/
void MainWindow::cursorUp()
{
    if (row == 1)
        return;
    if (article->curLine->prevLine != NULL)
        row--;
    if (col > article->curLine->prevLine->length) { //如果列号大于上一行长度，则列数为上一行长度
        col = article->curLine->prevLine->length;
    }
    article->curLine = article->curLine->prevLine;
    article->curLine->curChar = article->curLine->headChar;
    for (int index = 0; index < col; index++) {
        article->curLine->curChar = article->curLine->curChar->nextChar;
    }

}
/*光标左移，curChar同步左移*/
void MainWindow::cursorLeft()
{
    if (row == 1 && col == 0)
        return;
    if (article->curLine->curChar == article->curLine->headChar && article->curLine->prevLine != NULL) { //如果已经在行首，但不是在第一行,则转到上一行行尾
        row--;
        col = article->curLine->prevLine->length;
        article->curLine = article->curLine->prevLine;
        article->curLine->curChar = article->curLine->headChar;
        while(article->curLine->curChar->nextChar != NULL) {
            article->curLine->curChar = article->curLine->curChar->nextChar;
        }
    }
    else if (article->curLine->curChar != article->curLine->headChar) { //如果不在行首
        col--;
        article->curLine->curChar = article->curLine->curChar->prevChar;
    }
}
/*光标右移，curChar同步右移*/
void MainWindow::cursorRight()
{
    if (row == article->linum && col == article->curLine->length)
        return;
    if (article->curLine->curChar->nextChar == NULL && article->curLine->nextLine != NULL) { //如果在行尾，但不是最后一行
        row++;
        col = 0;
        article->curLine = article->curLine->nextLine;
        article->curLine->curChar = article->curLine->headChar;

    }
    else if (article->curLine->curChar->nextChar != NULL) { //如果不在行尾
        col++;
        article->curLine->curChar = article->curLine->curChar->nextChar;
    }
}
/*光标和curChar均移到行首*/
void MainWindow::cursorHome()
{
    for (int index = col; index > 0; index--) {
        cursorLeft();
    }
    col = 0;
}
/*光标和curChar均移到行尾*/
void MainWindow::cursorEnd()
{
    for (int index = col; index < article->curLine->length; index++) {
        cursorRight();
    }
    col = article->curLine->length;
}


/**********编辑功能：插入/删除字符/行/选择部分，复制粘贴查找替换**********/
/*转成全角*/
void MainWindow::toFullWidth(QString &str)
{
    for (int index1 = 0; index1 < str.length(); index1 ++) {
        ushort tempcode;
        if (str[index1] == " ") {
            tempcode = 0X3000;
            QChar char1(tempcode);
            str[index1] = char1;
        }
        else if (str[index1].unicode() >= 0X0021 && str[index1].unicode() <= 0X007E){
            tempcode = str[index1].unicode() + 0XFEE0;
            QChar char2(tempcode);
            str[index1] = char2;
        }
    }
}
/*在光标后插入一个字符*/
void MainWindow::newChar(QChar a)
{
    if (hasSelection)
        delSelect();
    QString temp_str = a;

    if (article->curLine->curChar->nextChar == NULL) { //如果当前行未满且光标已位于行尾
        if (a == "\n") { //如果是分段符，则换行
            newLine();
            return;
        }
        toFullWidth(temp_str); //转化为全角
        a = temp_str[0];
        charPtr temp = article->curLine->curChar; //指向当前字符结点，用于移花接木
        /*新建字符结点并初始化*/
        article->curLine->curChar->nextChar = (charPtr)malloc(sizeof(character));
        article->curLine->curChar = article->curLine->curChar->nextChar;
        article->curLine->curChar->ch = a;
        article->curLine->curChar->isSelected = false;
        /*将新结点连接至链表*/
        article->curLine->curChar->prevChar = temp;
        article->curLine->curChar->nextChar = NULL;
        article->curLine->length ++;
        col++;

    }
    else if(article->curLine->curChar->nextChar != NULL) {//光标不在行尾，则插入该字符
        insert(temp_str);
    }
}
/*删除光标前一个字符*/
void MainWindow::delChar()
{
    if (col == 0) { //光标位于行首
        delLine();
    }
    else if (col > 0){ //如果不在行首
        /*将字符结点从链表中删除*/
        article->curLine->curChar->prevChar->nextChar = article->curLine->curChar->nextChar;
        charPtr temppre = article->curLine->curChar->prevChar;
        if (article->curLine->curChar->nextChar != NULL)
            article->curLine->curChar->nextChar->prevChar = article->curLine->curChar->prevChar;
        free(article->curLine->curChar);
        article->curLine->curChar = temppre;
        article->curLine->length --;

        col--;
    }

}

/*删除光标后一字符*/
void MainWindow::delNextChar()
{
    if (row == article->linum && article->curLine->curChar->nextChar == NULL) {
        return;
    }
    if (article->curLine->curChar->nextChar == NULL) { //边界处理：行尾
        cursorRight();
    }
    cursorRight();
    delChar();

}

/*新建空行*/
void MainWindow::newLine()
{
    linePtr temp = article->curLine; //指向当前行，用于移花接木
    linePtr tempnext = temp->nextLine;
    /*新建一行并初始化*/
    article->curLine->nextLine = (linePtr)malloc(sizeof(line));
    article->curLine = article->curLine->nextLine;
    article->curLine->headChar = (charPtr)malloc(sizeof(character));
    article->curLine->headChar->nextChar = NULL;
    article->curLine->headChar->prevChar = article->curLine->headChar;
    article->curLine->curChar = article->curLine->headChar;
    article->curLine->curChar->isSelected = false;
    /*将新行插入到文档*/
    article->curLine->prevLine = temp;
    article->curLine->nextLine = tempnext;
    if (tempnext != NULL)
        tempnext->prevLine = article->curLine;

    article->curLine->length = 0;

    article->linum ++;

    row ++;
    col = 0;

}
/*删除当前行*/
void MainWindow::delLine()
{
    if (row == 1 && article->curLine->length == 0) //如果是第一行开头，则返回
        return;
    linePtr temp_line = article->curLine->nextLine;
    charPtr temp_char = article->curLine->curChar->nextChar;
    free(article->curLine);
    int leftlen = article->curLine->length;
    cursorLeft();
    article->curLine->curChar->nextChar = temp_char;
    if (temp_char != NULL)
        temp_char->prevChar = article->curLine->curChar;
    article->curLine->nextLine = temp_line;
    if (temp_line != NULL)
        temp_line->prevLine = article->curLine;
    article->curLine->length += leftlen;
    article->linum --;

}

/*删除选中文本*/
void MainWindow::delSelect()
{
    int index = 0;
    int select_len = selectedText.length(); //选择的文本（含界面上的行末分段符\u2029，链表中是没有的）
    resetSelect();

    if (last_arrow == 0) { //如果是从右往左选择的
        for (index = 0; index < select_len; index ++) { //删除光标后的选择部分
            cursorRight();
        }
    }
    for (index = 0; index < select_len; index ++) { //删除光标前的选择部分
        delChar();
    }
    showText();
}

/*取消选择部分*/
void MainWindow::resetSelect()
{
    linePtr tempLine = article->headLine;
    charPtr tempChar = tempLine->headChar;
    while (tempLine != NULL) {
        if (tempChar == NULL) {
            tempLine = tempLine->nextLine;
            if (tempLine == NULL)
                break;
            tempChar = tempLine->headChar;
            tempChar->isSelected = false;
        }

        tempChar = tempChar->nextChar;
        if (tempLine != NULL && tempChar != NULL && tempChar->isSelected)
            tempChar->isSelected = false;
    }
    hasSelection = false;
    selectedText = "";
    showText();
}
/*在curChar后插入字符或字符串或段落*/
void MainWindow::insert(QString str)
{
    if (str.length() == 0)
        return;
    if (hasSelection)
        delSelect();
    charPtr tempChar = article->curLine->curChar->nextChar;
    linePtr tempLine = article->curLine->nextLine;
    article->curLine->curChar->nextChar = NULL;
    article->curLine->nextLine = NULL;
    int leftlen = article->curLine->length - col;
    article->curLine->length = col;

    if (tempChar != NULL)
        tempChar->prevChar = NULL;
    if (tempLine != NULL)
        tempLine->prevLine = NULL;
    for (int index = 0; index < str.length(); index ++) {
        newChar(str[index]);
    }
    article->curLine->curChar->nextChar = tempChar;
    article->curLine->nextLine = tempLine;
    article->curLine->length += leftlen;
    if (tempChar != NULL)
        tempChar->prevChar = article->curLine->curChar;
    if (tempLine != NULL)
        tempLine->prevLine = article->curLine;
    showText();
}
/*复制*/
void MainWindow::copy()
{
    register_text = selectedText;
}

/*剪切*/
void MainWindow::cut()
{
    if (hasSelection) {
        register_text = selectedText;
        delSelect();
    }
}
/*粘贴*/
void MainWindow::paste()
{
    insert(register_text);
}
/*从光标后开始查找第一个结果，并选中第一个结果*/
bool MainWindow::findStr(QString &str2)
{
    int target_row;
    int target_col;
    int index1 = 1;
    int index2 = 0;
    linePtr temp_line = article->curLine;
    charPtr temp_char = article->curLine->curChar->nextChar;
    toFullWidth(str2); //转换为全角
    for (index1 = row; temp_line != NULL; index1++) {
        QString aline = "";
        for (index2 = 0; temp_char != NULL; index2 ++) {
            aline += temp_char->ch;
            temp_char = temp_char->nextChar;
        }
        int next[str2.length()];
        findGetNextVal(str2, next);
        int result = findKMP(aline, str2, next);
        if (result != -1) {
            if (temp_line == article->curLine)
                target_col = result + col + 1;
            else
                target_col = result + 1;
            target_row = index1;
            while (row != target_row || col != target_col) {
                cursorRight();

            }
            cursorLeft();
            for (int index3 = 0; index3 < str2.length(); index3++) { //选中第一个结果
                cursorRight();
                article->curLine->curChar->isSelected = true;
                last_arrow = 1;
            }
            showText();
            return true;
        }

        temp_line = temp_line->nextLine;
        if (temp_line != NULL)
            temp_char = temp_line->headChar->nextChar;

    }
    return false;
}
/*KMP确定next数组*/
void MainWindow::findGetNextVal(QString &str2, int next[])
{
    int len1 = str2.length();
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < len1 - 1) {
        if (k == -1 || str2[j] == str2[k]) {
            ++j;
            ++k;
            if (str2[j] != str2[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else {
            k = next[k];
        }
    }
}
/*KMP查找*/
int MainWindow::findKMP(QString &str1, QString &str2, int next[])
{
    int i = 0;
    int j = 0;
    int len2 = str1.length();
    int len1 = str2.length();
    while (i < len2 && j < len1) {
        if (j == -1 || str1[i] == str2[j]) {
            i++;
            j++;
        }
        else {
            j = next[j];
        }
    }
    if (j == len1)
        return i - j;
    else
        return -1;
}

/*替换选中的文本*/
bool MainWindow::changeStr(QString str) //TODO
{
    if (!hasSelection)
        return false;
    delSelect();
    insert(str);
    return true;
}
/*查找功能*/
void MainWindow::findText()
{
    if (hasSelection)
        resetSelect();
    QString str = findLineEdit->text();
    if(!findStr(str)){
        QMessageBox::warning(this, tr("查找"),tr("找不到%1").arg(str));
    }
}
/*替换功能*/
void MainWindow::changeText()
{
    QString str1 = findLineEdit->text();
    QString str2 = changeLineEdit->text();
    if (hasSelection) {
        QMessageBox box;
        box.setWindowTitle("提示");
        box.setText(tr("是否替换？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
            changeStr(str2);
    }
    else {
        if(!findStr(str1)){
            QMessageBox::warning(this, tr("查找"),tr("找不到%1").arg(str1));
        }
        else{
            QMessageBox box;
            box.setWindowTitle("提示");
            box.setText(tr("是否替换？"));
            QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                                QMessageBox::YesRole);
            box.addButton(tr("否(&N)"), QMessageBox::NoRole);
            box.exec();
            if (box.clickedButton() == yesBtn)
                changeStr(str2);
        }
    }
}

/**********文件操作**********/
/*新建文件*/
void MainWindow::newFile()
{
    if ( maybeSave() ) {
        isUntitled =true;
        curFile = tr("新文件.txt");
        setWindowTitle(curFile);
        while (row != article->linum || col != article->curLine->length) {
            cursorRight();
        }
        while (row != 1 || col != 0) {
            delChar();
        }
        preText = "";
        allText = "";
        showText();
    }
}

/*是否已保存，若未保存则提示进行保存*/
bool MainWindow::maybeSave()
{
    if(allText != preText) {
        QMessageBox box;
        box.setWindowTitle("警告");
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr("尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("取消"),
                                               QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
            return save();
        else if (box.clickedButton() == cancelBtn)
            return false;
    }

    return true;
}

/*保存*/
bool MainWindow::save()
{
    if (isUntitled)
        return saveAs();
    else
        return saveFile(curFile);

}
/*另存为*/
bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("另存为"), curFile);  //ToKnow
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
/*保存文件至fileName文件*/
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("文档编辑器"),
                             tr("无法写入文件 %1: /n %2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    saveData(out);
    file.close();
    QApplication::restoreOverrideCursor();
    isUntitled = false;

    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

/*打开fielName文件*/
bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法读取文件 %1: /n %2").arg(fileName).arg(file.errorString()));
        return false;
    }

    while (row != article->linum || col != article->curLine->length) {
        cursorRight();
    }
    while (row != 1 || col != 0) {
        delChar();
    }
    preText = "";
    allText = "";

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    loadData(in);
    QApplication::restoreOverrideCursor();

    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    isUntitled = false;

    showText(); //刷新屏幕
    preText = allText;
    return true;
}

/*将数据结构内容写入文件流*/
void MainWindow::saveData(QTextStream &fout)
{
    linePtr tempLine = article->headLine;
    charPtr tempChar = tempLine->headChar;
    while (tempLine != NULL) {
        if (tempChar == NULL) {
            tempLine = tempLine->nextLine;
            if (tempLine == NULL)
                break;
            tempChar = tempLine->headChar;
            fout << endl;
        }

        tempChar = tempChar->nextChar;
        if (tempLine != NULL && tempChar != NULL)
            fout << tempChar->ch;
    }
    showText();
    preText = allText;
}

/*将文件流内容读入至数据结构*/
void MainWindow::loadData(QTextStream &fin)
{

    QChar tempChar;
    while(!fin.atEnd()) {
        fin >> tempChar;
        if (tempChar == '\t') {
            QChar tempChar = ' ';
            newChar(tempChar);
            newChar(tempChar);
            continue;
        }
        if (tempChar == '\n') {
            newLine();
            continue;
        }
        newChar(tempChar);
    }

}

/**********菜单Action**********/
/*新建*/
void MainWindow::on_actionNew_N_triggered()
{
    newFile();
}

/*打开*/
void MainWindow::on_actionOpen_O_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

/*保存*/
void MainWindow::on_actionSave_S_triggered()
{
    save();
}

/*另存为*/
void MainWindow::on_actionSaveAs_A_triggered()
{
    saveAs();
}
/*退出*/
void MainWindow::on_actionExit_Q_triggered()
{
    if (maybeSave())
        qApp->quit();
}

/*复制*/
void MainWindow::on_actionCopy_C_triggered()
{
    copy();
}

/*剪切*/
void MainWindow::on_actionCut_X_triggered()
{
    cut();
}

/*粘贴*/
void MainWindow::on_actionPaste_V_triggered()
{
    paste();
}

/*弹出查找替换对话框*/
void MainWindow::on_actionFind_F_triggered()
{
    fandcDlg->show();
}

/*关于*/
void MainWindow::on_actionAbout_I_triggered()
{
    QMessageBox box;
    box.setWindowTitle("关于");
    box.setText("MiniWord\n作者：张帅帅 刘孟秦 高泽宇");
    box.exec();

}

/*关闭窗口*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}


