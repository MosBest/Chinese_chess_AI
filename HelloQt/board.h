#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Stone.h"
#include "Step.h"
#include <QVector>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    Stone  _s[32];//定义32颗棋子
    int _r;//棋子的半径
    int _selectid;
    bool _bRedTurn;

    QPoint center(int row , int col);//返回象棋棋盘行列对应的像素坐标
    QPoint center(int id);//函数重载
    void paintEvent(QPaintEvent *);//画图事件
    //void mousePressEvent(QMouseEvent *ev);//鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *ev);//鼠标释放事件

    bool getRowCol(QPoint pt,int& row,int& col);
    void drawStone(QPainter& painter ,int id);

    bool canMove(int moveid , int row , int col , int killed);

    bool canMoveJIANG(int moveid , int row , int col , int killed);
    bool canMoveSHI(int moveid , int row , int col , int killed);
    bool canMoveXIANG(int moveid , int row , int col , int killed);
    bool canMoveMA(int moveid , int row , int col , int killed);
    bool canMoveCHE(int moveid , int row , int col , int killed);
    bool canMovePAO(int moveid , int row , int col , int killed);
    bool canMoveBING(int moveid , int row , int col , int killed);

    int getStoneId(int rEye,int cEye);
    int getStoneCountAtLine(int row1,int col1,int row2,int col2);
    bool isBottomSide(int moveid);//moveid这个棋子是上面阵营的，还是下面阵营的

    virtual void click(int clicked,int& row,int& col);
    void click(QPoint pt);
    ~Board();

signals:

public slots:
};

#endif // BOARD_H
