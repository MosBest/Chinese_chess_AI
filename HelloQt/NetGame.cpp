#include "NetGame.h"
#include <QMessageBox>//用于弹出对话框

NetGame::NetGame(bool server)
{
    _server=NULL;
    _socket=NULL;

    if(server)//如果是服务器启动
    {
        _server=new QTcpServer(this);
        _server->listen(QHostAddress::Any,2000);//监听，参数分别为hostname（任意都可以） 和 端口号

        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));//表示当监听到有客户端连接时，那么就新建连接，并且调用slotNewConnection()函数来运行。

    }
    else
    {

        _socket=new QTcpSocket(this);//该_socket是客户端的socket,用于与服务器进行通信
        _socket->connectToHost(QHostAddress("127.0.0.1"),2000);//该socket与服务器连接，参数分别为 hostname 和 端口号
        connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));


    }

}

NetGame::~NetGame()
{

}

void NetGame::click(int clicked, int &row, int &col)
{
    if(_selectid==-1 && clicked != -1)
    {
        if(_s[clicked]._red != _redSide)
            return;
    }
    //在自己的棋盘上面走
    Board::click(clicked,row,col);

    //把信息发送给对方，让对方的棋盘可以显示出来
    char buf[4];
    buf[0]=2;
    buf[1]=9-row;
    buf[2]=8-col;
    buf[3]=clicked-16;
    _socket->write(buf,4);


}

void NetGame::slotRecv()
{
    QByteArray ba = _socket->readAll();
    char cmd=ba[0];
    if(cmd==1)
    {
        //初始化
        char data=ba[1];
        if(data==1)
        {
            initChess(1);//如果cmd表示1，则是服务器向客户端发送的buf[2]，那么这个一定是客户端的接受，所以这一句就是初始化客户端的棋盘
            _redSide=false;
        }
        else
        {
            initChess(0);//如果cmd表示1，则是服务器向客户端发送的buf[2]，那么这个一定是客户端的接受，所以这一句就是初始化客户端的棋盘
            _redSide=true;
        }
    }
    else if(cmd==2)
    {
        int row=ba[1];
        int col=ba[2];
        int id=ba[3];
        Board::click(id,row,col);
    }

}

void NetGame::slotNewConnection()
{
    if(_socket) //服务器的socket仅仅只能和一个客户端连接
        return;
    //这个_socket是服务器的socket,用于与客户端连接的。
    _socket=_server->nextPendingConnection();//服务器的socket与客户端建立起了新连接

    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    //服务器向客户端发送数据，要求客户端用黑棋，还是用红棋
    char buf[2];
    buf[0]=1;
    buf[1]=1;//默认自己用红棋,对方用黑棋
    _redSide=true;

    _socket->write(buf,2);//服务器向客户端发送buf[2]
    initChess(0);

}

void NetGame::initChess(char c)//传0走红棋，传1走黑棋
{
    for(int i=0;i<32;i++)
    {
        _s[i].init(i,c);
    }
}

