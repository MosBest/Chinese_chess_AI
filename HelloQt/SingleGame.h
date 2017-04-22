#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "board.h"

class SingleGame : public Board  //SingleGame从Board中派生出来
{
    Q_OBJECT
public:
    SingleGame()
    {
        _level=4;
    }

    //~SingleGame();
    virtual void click(int clicked,int& row,int& col);//SingleGame重载click函数

    Step* getBestMove();//电脑来移动
    void getAllPossibleMove(QVector<Step*>& steps);
    void saveStep(int moveid,int killid, int row,int col, QVector<Step *> &steps);

    void fackMove(Step* step);
    void unfackMove(Step* step);
    int calcScore();
    void killStone(int killid);
    void moveStone(int moveid,int rowTo,int colTo);
    void reliveStone(int killid);

    int getMinScore(int level);
    int getMaxScore(int level);

    int _level;
public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
