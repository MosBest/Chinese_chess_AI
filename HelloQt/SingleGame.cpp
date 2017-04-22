#include "SingleGame.h"
#include <QTimer>

void SingleGame::click(int clicked,int& row,int& col)//SingleGame重载click函数
{
    if(!this->_bRedTurn)
        return;  //如果是黑棋子，人点了也没用

    Board::click(clicked,row,col);

    if(!this->_bRedTurn)
    {

        //启动0.1秒定时器，在0.1秒期间让红棋刷新完，之后电脑思考。
        QTimer::singleShot(100,this,SLOT(computerMove()));
    }
}

void SingleGame::computerMove()
{
    Step* step=getBestMove();

    killStone(step->_killid);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
    delete step;
    update();
}

void SingleGame::saveStep(int moveid,int killid, int row,int col, QVector<Step *> &steps)
{
    Step* step=new Step;
    step->_colFrom=_s[moveid]._col;
    step->_rowFrom=_s[moveid]._row;

    step->_colTo=col;
    step->_rowTo=row;

    step->_moveid=moveid;
    step->_killid=killid;

    steps.append(step);

}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    int min=0,max=16;
    if(this->_bRedTurn)//如果是红棋走
    {
        min=16;
        max=32;
    }
    for(int i=min;i<max;i++)//0~15是黑色棋子 , 16~32是红色棋子
    {
        if(_s[i]._dead)
            continue;
        for(int row=0;row<=9;row++)
        {
            for(int col=0;col<=8;col++)//遍历棋盘所有的位置，存储所有黑棋可以走的位置到steps容器中
            {
                int killed =this->getStoneId(row,col);
                if(_s[killed]._red==_s[i]._red && killed!=-1)
                    continue;

                if(canMove(i,row,col,killed))
                {
                    saveStep(i,killed,row,col,steps);
                }
            }
        }
    }
}

void SingleGame::killStone(int killid)
{
    if(killid==-1)
        return;
    _s[killid]._dead=true;
}

void SingleGame::moveStone(int moveid,int rowTo,int colTo)
{
    _s[moveid]._row=rowTo;
    _s[moveid]._col=colTo;

    _bRedTurn = !_bRedTurn;
}

void SingleGame::reliveStone(int killid)
{
    if(killid==-1)
        return;
    _s[killid]._dead=false;
}

void SingleGame::fackMove(Step* step)
{
    killStone(step->_killid);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
}

void SingleGame::unfackMove(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid,step->_rowFrom,step->_colFrom);
}

//评价局面分 ，最简单的一种
int SingleGame::calcScore()
{
    //enum TYPE{JIANG , CHE , PAO , MA , BING , SHI , XIANG};
    static int chessScore[]={2000,100,50,50,20,10,10};

    int redTotalScore=0;
    int blackTotalScore=0;
    //红棋的分数
    for(int i=16;i<32;i++)
    {
        if(_s[i]._dead==true)
            continue;
        redTotalScore+=chessScore[_s[i]._type];
    }
    //黑棋的分数
    for(int i=0;i<16;i++)
    {
        if(_s[i]._dead==true)
            continue;
        blackTotalScore+=chessScore[_s[i]._type];
    }

    //总分=黑棋的分数-红棋的分数
    return blackTotalScore-redTotalScore;

}

int SingleGame::getMaxScore(int level)
{
    if(level==0)
        return calcScore();

    //1. 看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);//红棋的所有可能的走法
    int maxScore=-1000000;
    while(steps.count())
    {
        Step* step=steps.back();
        steps.removeLast();

        fackMove(step);//尝试走一步
        int score=getMinScore(level-1);//再假象人走一步后导致的最小局面分
        unfackMove(step);//从那一步回来

        if(score>maxScore)
        {
            maxScore=score;
        }
        delete step;
    }

    return maxScore;

}

int SingleGame::getMinScore(int level)
{
    if(level==0)
        return calcScore();
    //1. 看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);//红棋的所有可能的走法
    int minScore=1000000;
    while(steps.count())
    {
        Step* step=steps.back();
        steps.removeLast();

        fackMove(step);//尝试走一步
        int score=getMaxScore(level-1);//再假象人走一步后导致的最小局面分
        unfackMove(step);//从那一步回来

        if(score<minScore)
        {
            minScore=score;
        }
        delete step;
    }

    return minScore;

}

Step* SingleGame::getBestMove()
{
    //1. 看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //2.每一种可能尝试地走一下
    //3. 并且记住每一种可能的得分
    int maxScore=-100000;
    Step* ret=NULL;
    while(steps.count())
    {
        Step* step=steps.back();
        steps.removeLast();

        fackMove(step);//尝试走一步
        int score=getMinScore(_level-1);//再假象人走一步后导致的最小局面分
        unfackMove(step);//从那一步回来

        if(score>maxScore)
        {
            maxScore=score;
            if(ret)
                delete ret;
            ret=step;
        }
        else
        {
            delete step;
        }
    }

    //4. 取最好的结果作为参考
    return ret;
}
