#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "paintwidget.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QTextCursor>
#include <QTextStream>
#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLineEdit>
#include <QPainter>
#include <QInputMethodEvent>

//#define LINESIZE 100 //一行字符容量
#define BEGINX 10 //文字区域左上角起点横坐标
#define BEGINY 20 //文字区域左上角起点纵坐标


/*全局变量*/
extern int areaWidth; //滚动区域宽度
extern int areaHeight; //滚动区域高度
extern int row; //当前行号
extern int col; //当前列号，从0-length，0位置在首字符前
extern int last_arrow;//表示选择文字块时最后一个方向键是左（0）还是右（1）
extern QString allText; //当前的全部文本
extern QString preText; //上次保存的文本
extern QString selectedText; //选择的文本
extern QString register_text; //已复制/待粘贴的文本
extern bool hasSelection; //判断是否有文字被选择

namespace Ui {
class MainWindow;
}

typedef struct character { //字符结点
    QChar ch;
    struct character *prevChar;
    struct character *nextChar;
    bool isSelected;
}character, *charPtr;

typedef struct line {
    charPtr headChar; //行首字符结点，不存储字符
    charPtr curChar;
    int length;
    struct line *prevLine;
    struct line *nextLine;
}line, *linePtr;

typedef struct doc {
    linePtr headLine; //代表首行的行结点，存储第一行
    linePtr curLine;
    int linum; //总的行数
}doc, *docPtr;

extern docPtr article; //全局变量， 文档


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLineEdit *findLineEdit; //查找输入框
    QLineEdit *changeLineEdit; //替换输入框
    QDialog *fandcDlg; //查找替换对话框
    paintWidget *area; //滚动区域内widget
    QScrollArea *scrollArea; //滚动条区域

    bool isUntitled; //判断文件是否已命名
    QString curFile; //文件名


public:
    void showText(); //将当前数据结构中的内容刷新显示到屏幕
    void newFile(); // 新建操作
    bool maybeSave(); // 判断是否需要保存
    bool loadFile(const QString &fileName);
    void loadData(QTextStream &fin); //将文本文件读入到链表
    bool save();  //保存
    bool saveAs(); //另存为
    bool saveFile(const QString &fileName); // 保存文件
    void saveData(QTextStream &fout); //将链表导出到文本文件



    void keyPressEvent(QKeyEvent *e); //根据键盘事件进行相应操作
    void inputMethodEvent(QInputMethodEvent *e);
    void mousePressEvent(QMouseEvent *e);        //单击
    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);         //移动
    void contextMenuEvent(QContextMenuEvent *e); //右键菜单
//    void wheelEvent(QWheelEvent *e);             //滑轮

    void toFullWidth(QString &str); //转换为全角
    void newLine(); //在一行的curChar后新建一个空行
    void delLine(); //删除行
    void newChar(QChar a); //在curChar后插入一个字符
    void delChar(); //backspace键的作用，删除光标前的一个字符，即删除curChar
    void delNextChar(); //delete键的作用，删除光标后一个字符
    void delSelect(); //删除所选部分文字
    void resetSelect(); //取消所选
    bool findStr(QString &str2); //查找
    int findKMP(QString &str1, QString &str2, int next[]); //KMP匹配算法
    void findGetNextVal(QString &str2, int next[]); //KMP匹配算法中确定next数组
    bool changeStr(QString str); //替换
    void copy(); //复制
    void cut(); //剪切
    void paste(); //粘贴
    void insert(QString str); //插入一个字符或一个段落

    void cursorDown(); //curChar下移
    void cursorUp(); //curChar上移
    void cursorLeft(); //curChar左移
    void cursorRight(); //curChar右移
    void cursorHome(); //curChar移至行首
    void cursorEnd(); //curChar移至行尾

private slots:


    void on_actionNew_N_triggered(); //新建

    void on_actionOpen_O_triggered(); //打开

    void on_actionSave_S_triggered(); //保存

    void on_actionSaveAs_A_triggered(); //另存为

    void on_actionExit_Q_triggered(); //退出

    void on_actionCopy_C_triggered(); //复制

    void on_actionCut_X_triggered(); //剪切

    void on_actionPaste_V_triggered(); //粘贴

    void on_actionFind_F_triggered(); //查找

    void on_actionAbout_I_triggered();

    void findText(); //查找
    void changeText(); //替换


};


#endif // MAINWINDOW_H
