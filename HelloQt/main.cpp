
#include <QApplication>
//#include "board.h"
#include "SingleGame.h"
#include "NetGame.h"
#include <QMessageBox>//用于弹出对话框

int main(int argc,char* argv[])
{

    QApplication app(argc,argv);
    //Board board;//1. 普通棋盘
    //SingleGame board;//2. 人机博弈，智能棋盘


    //3. 人人网络对战

    QMessageBox::StandardButton ret;//ret存储按钮按下后的返回值
    ret=QMessageBox::question(NULL,"server or client" , "是否作为服务器启动");//question（）用于弹出一个对话框，表示是作为服务器启动，还是作为客户端启动。下面有两个按钮，一个是yes,一个是no
    bool bServer=false;
    if(ret==QMessageBox::Yes)//按钮按下的是Yes
    {
        bServer=true;
    }

    NetGame board(bServer);//3. 人人网络对战

    board.show();
    return app.exec();
}


