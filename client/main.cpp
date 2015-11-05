#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "game.hpp"

#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    Game game(5);
//    game.start_game();

//    client c("localhost");

    client c("10.42.0.1");

    return a.exec();
}
