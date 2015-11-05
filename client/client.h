#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QObject>

#include <vector>
#include <string>
using namespace std;

#include "serialization.hpp"
#include "game.hpp"

#define CMD_BALLON 0
#define CMD_USER 1
#define CMD_SEND_U_ID 2

class client : public QObject
{
Q_OBJECT
public:
    client(QString host);
    ~client();

    void getBallons();

private:
    int nPort = 8080;
    QTcpSocket* m_pTcpSocket;

    vector<Balloon> balons;
    vector<User> users;

    int u_id;

    Game *game;
    bool start_game = false;
    int number = 0;

private slots:
    void slotError();
    void slotReadyRead();
    void slotConnected();

};

#endif // MYSERVER_H
