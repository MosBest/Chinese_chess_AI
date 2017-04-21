#include "board.h"
#include <QPainter>
#include <QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
    for(int i=0;i<32;i++)
    {
        _s[i].init(i);
    }

    _selectid=-1;
    _bRedTurn=true;//一开始红棋走
}


void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//绘制QPainter 必须在paintEvent里面
    int d=40;
    _r=d/2;

    //画10条横线
    for(int i=1;i<=10;i++)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //画9条竖线
    for(int i=1;i<=9;i++)
    {
        if(i==1 || i==9)
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }

    //九宫格
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制32个棋子
    for(int i=0;i<32;i++)
    {
        drawStone(painter,i);//绘制棋子
    }

}

QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx()=(col+1)* _r*2;
    ret.ry()=(row+1)* _r*2;
    return ret;
}


QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

void Board::drawStone(QPainter& painter , int id) //painter是引用
{
    if(_s[id]._dead)
        return;
    QPoint c=center(id);
    QRect rect=QRect(c.x()-_r , c.y()-_r , _r*2 , _r*2);//矩形

    //设置画刷
    if(id==_selectid)
        painter.setBrush(QBrush(Qt::gray));//灰色,该棋子被鼠标点中
    else
        painter.setBrush(QBrush(Qt::yellow));//黄色，该棋子没有被鼠标点中

    //设置画笔
    painter.setPen(Qt::black);

    painter.drawEllipse(center(id) , _r , _r);

    if(_s[id]._red)
        painter.setPen(Qt::red);

    //设置字体
    painter.setFont(QFont("system",_r,700));
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));

}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint c=center(row,col);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<_r*_r)
                return true;
        }
    }

    return false;
}

bool Board::canMoveJIANG(int moveid , int row , int col , int killed)
{
    /*
        1. 首先目标位置在九宫格内
        2. 移动的步长是一个格子
    */

    if(!_s[moveid]._red)
    {
        if(row > 2)
           return false;
    }
    else
    {
        if(row<7)
            return false;
    }

    if(col<3)
        return false;
    if(col>5)
        return false;

    int dr=_s[moveid]._row -row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);
    if(d==1||d==10)
        return true;
    return false;
}

bool Board::canMoveSHI(int moveid , int row , int col , int killed)
{
    /*
        1. 首先目标位置在九宫格内
        2. 每次只能斜着走
    */

    if(!_s[moveid]._red)
    {
        if(row > 2)
           return false;
    }
    else
    {
        if(row<7)
            return false;
    }

    if(col<3)
        return false;
    if(col>5)
        return false;

    int dr=_s[moveid]._row -row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);
    if(d==11)
        return true;
    return false;
}

//我写的
int Board::getStoneId(int rEye,int cEye)//在(rEye,cEye)点如果有棋子，就返回改棋子的id，否则返回-1
{
    for(int i=0;i<32;i++)
    {
        if(_s[i]._row==rEye && _s[i]._col==cEye && _s[i]._dead==false)
        {
            return i;
        }
    }

    return -1;
}

bool Board::isBottomSide(int moveid)//moveid这个棋子是上面阵营的，还是下面阵营的
{

    if(_s[moveid]._red)
        return true;
    return false;
}

bool Board::canMoveXIANG(int moveid , int row , int col , int killed)
{
    //象走田字
    int dr=_s[moveid]._row -row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);
    if(d!=22) return false;

    //象的田字格中心不能够有棋子
    int rEye=(row+_s[moveid]._row)/2;
    int cEye=(col+_s[moveid]._col)/2;
    if(getStoneId(rEye,cEye)!= -1)//如果田字格正中间有棋子，那么象就不能够走
        return false;

    //象不能够过界

    if(isBottomSide(moveid))//moveid这个棋子是上面阵营的，还是下面阵营的
    {
        //moveid这个棋子是下面阵营的
        if(row < 4) return false;//不能跑到上面去
    }
    else
    {
        //moveid这个棋子是上面阵营的
        if(row > 5) return false;//不能跑到下面去
    }

    //最终，才开以走
    return true;
}

bool Board::canMoveMA(int moveid , int row , int col , int killed)
{
    int dr=_s[moveid]._row -row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);
    if(d!=12 && d!=21)
        return false;
    if(d == 12)//马横着走
    {
        if(getStoneId(_s[moveid]._row , (col+_s[moveid]._col)/2) != -1)//有棋子蹩马腿
            return false;
    }
    else//马竖着走
    {
        if(getStoneId((row+_s[moveid]._row)/2,_s[moveid]._col) != -1)//有棋子蹩马腿
            return false;
    }
    return true;
}

//得到两个点之间的有几个棋子,返回值为棋子的个数 ，如果返回-1，则表示两个点不在一条直线上面
int Board::getStoneCountAtLine(int row1,int col1,int row2,int col2)
{
    int ret=0;
    if(row1!=row2 && col1!=col2)
        return -1;
    if(row1==row2 && col1==col2)
        return -1;

    if(row1==row2)
    {
        int min = col1<col2 ? col1 : col2 ;
        int max = col1<col2 ? col2 : col1 ;
        for(int col=min+1;col<max;col++)
        {
            if(getStoneId(row1 ,col)!=-1)
                ++ret;
        }
    }
    else
    {
        int min = row1<row2 ? row1 : row2 ;
        int max = row1<row2 ? row2 : row1 ;
        for(int row=min+1;row<max;row++)
        {
            if(getStoneId(row ,col1)!=-1)
                ++ret;
        }
    }
    return ret;
}


bool Board::canMoveCHE(int moveid , int row , int col , int killed)
{
    int ret=getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);//得到两个点之间的有几个棋子
    if(ret==0)
        return true;
    return false;
}

bool Board::canMovePAO(int moveid , int row , int col , int killed)
{
    int ret=getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);//得到两个点之间的有几个棋子
    if(killed!=-1)
    {
        if(ret==1)
            return true;
    }
    else
    {
        if(ret==0)
            return true;
    }
    return false;
}

bool Board::canMoveBING(int moveid , int row , int col , int killed)
{
    int dr=_s[moveid]._row -row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);
    if(d!=1 && d!=10)
        return false;

    //看兵有没有过河

    if(isBottomSide(moveid))//moveid这个棋子是上面阵营的，还是下面阵营的
    {
       //moveid这个棋子是下面阵营的
        if(_s[moveid]._row < row) return false;//不能向后者
        if(_s[moveid]._row>=5 && _s[moveid]._row == row) return false;//在自己领地时，不能够左右走
    }
    else
    {
        //moveid这个棋子是上面阵营的
        if(_s[moveid]._row > row) return false;
        if(_s[moveid]._row<=4 && _s[moveid]._row == row) return false;//在自己领地时，不能够左右走
    }

    return true;
}




bool Board::canMove(int moveid, int row, int col, int killed)
{
    if(_s[moveid]._red==_s[killed]._red && killed!=-1)//killed和moveid是同一方
    {
        //换选择
        _selectid=killed;
        update();

        return false;
    }

    switch(_s[moveid]._type)
    {
    case Stone::BING:
        return canMoveBING(moveid, row, col, killed);
        break;
    case Stone::CHE:
        return canMoveCHE(moveid, row, col, killed);
        break;
    case Stone::JIANG:
        return canMoveJIANG(moveid, row, col, killed);
        break;
    case Stone::MA:
        return canMoveMA(moveid, row, col, killed);
        break;
    case Stone::PAO:
        return canMovePAO(moveid, row, col, killed);
        break;
    case Stone::SHI:
        return canMoveSHI(moveid, row, col, killed);
        break;
    case Stone::XIANG:
        return canMoveXIANG(moveid, row, col, killed);
        break;
    }


    return true;
}

void Board::click(int clicked,int& row,int& col)
{
    if(_selectid==-1)
    {
        if(clicked!=-1)
        {
            if(_bRedTurn == _s[clicked]._red)
            {
                _selectid=clicked;
                update();
            }
        }
    }
    else
    {
        if(canMove(_selectid , row , col , clicked ))//看是否能够走，约定象棋的规章制度
        {
            //走棋或者吃棋
            _s[_selectid]._row=row;
            _s[_selectid]._col=col;
            if(clicked!=-1)
            {
                _s[clicked]._dead=true;
            }
            _selectid=-1;
            _bRedTurn=!_bRedTurn;
            update();
        }

    }
}

void Board::click(QPoint pt)
{
    int row,col;
    bool bRet=getRowCol(pt,row,col);
    if(bRet==false)//表明鼠标点到了棋盘外面了
        return;
    int clicked=-1;

    int i;
    for(i=0;i<32;i++)
    {
        if(_s[i]._col==col && _s[i]._row==row && _s[i]._dead==false)
        {
            break;
        }
    }
    if(i<32)
    {
         clicked=i;
    }

    click(clicked , row, col);
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt=ev->pos();//用于得到鼠标点击的位置
    //将pt转化为象棋的行列值
    //判断这个行列值上面有没有棋子
    click(pt);
}



Board::~Board()
{

}
