#ifndef NETGAME_H
#define NETGAME_H

#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>//用于弹出对话框
/*
 *服务器与客户端可能需要传送的信息
 * 1. 服务器要求客户端是用红棋，还是用黑棋 。 服务器发送，客户端接受
 *        那么第一个字节固定为1，第二个字节是1或者0。1表示客户端用红棋，0表示客户端用黑棋
 * 2. 传送点击信息 。 服务器要向客户端发，客户端也要向服务器发。因为自己走棋后，都要更新对方的棋盘
 *        第一个字节固定为2，第二个字节是row,第三个字节为col,第四个字节为点击的棋子的id.
*/


class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool server);

    void initChess(char c);
    void click(int clicked, int &row, int &col);

    QTcpServer* _server;
    QTcpSocket* _socket;
    ~NetGame();
public slots:

    void slotNewConnection();
    void slotRecv();

};

#endif // NETGAME_H
