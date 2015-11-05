#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "client.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "enter the server ip" << std::endl;
        return 1;
    }

    QCoreApplication a(argc, argv);
    QString ip = argv[1];
    client c(ip);
    return a.exec();
}
