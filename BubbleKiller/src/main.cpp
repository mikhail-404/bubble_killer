#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "game.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Game game(5);
    game.start_game();
    return a.exec();
}
