#include "client.h"

#include <iostream>

#include <QDataStream>

client::client(QString host)
{
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(host, nPort);

    //    m_pTcpSocket->connectToHost("10.42.0.1", nPort);

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));

    game = NULL;
//    game = new Game(15);
//    game->start_game();
}

client::~client() {
    m_pTcpSocket->close();
    delete[] m_pTcpSocket;

    if (game != NULL) {
        delete[] game;
    }
}

void client::slotConnected()
{
    cout << "connected" << endl;

    game = new Game(5);
    game->start_game();
}

void client::slotError()
{
    cout << "error" << endl;
}

void client::slotReadyRead()
{
    QTcpSocket *tcpClt =   (QTcpSocket *) sender();
    QDataStream stream(tcpClt);

    while (tcpClt->bytesAvailable()) {
        int type = 0;
        stream >> type;

        switch (type) {
            case CMD_BALLON: {
//                cout << "CMD_BALLON" << endl;
                stream >> balons;
                break;
            }
            case CMD_USER: {
//                cout << "CMD_USER" << endl;
                stream >> users;
                break;
            }
            case CMD_SEND_U_ID: {
//                cout << "CMD_SEND_U_ID" << endl;
                stream >> u_id;
                break;
            }

            default:
                break;
        }
    }
}
