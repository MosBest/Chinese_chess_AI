
#include <QApplication>
#include "board.h"

int main(int argc,char* argv[])
{

    QApplication app(argc,argv);
    Board board;
    board.show();
    return app.exec();
}


