
#include <QApplication>
//#include "board.h"
#include "SingleGame.h"

int main(int argc,char* argv[])
{

    QApplication app(argc,argv);
    //Board board;
    SingleGame board;
    board.show();
    return app.exec();
}


