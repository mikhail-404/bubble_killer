#include "server.hpp"
#include <iostream>
#include <string>
#include <QDataStream>

server::server()
    :m_gen(WIDTH, HEIGHT)
{
    m_server = new QTcpServer(this);
    if (! m_server->listen(QHostAddress::Any, m_nPort))
    {
        std::cout << "Unable to start the server!" << std::endl;
        m_server->close();
        return;
    }

    connect(m_server, SIGNAL(newConnection()), this, SLOT(slotNewUser()));

    std::cout << "Server started" << std::endl;

    get_balloons(20);
}

void server::get_balloons(int num_ballooons)
{
    for (int i = 0; i < num_ballooons; ++i)
    {
        m_balloons.push_back(m_gen.next_balloon());
    }
}

void server::delete_balloon(int balloon_id, QTcpSocket *socket)
{
    for (auto it = m_balloons.begin(); it != m_balloons.end(); ++it)
    {
        if (it->get_id() == balloon_id)
        {
            if (it->balloon_type() == Balloon::NORMAL)
            {
                m_users_db.set_score(socket, 1);
            }
            else
            {
                m_users_db.set_score(socket, -1);
            }

            m_balloons.erase(it);
            break;
        }
    }
}


void server::update(bool all, QTcpSocket *socket)
{
    QByteArray bal;
    QDataStream out_bal(&bal, QIODevice::WriteOnly);
    out_bal << m_balloons;

    QByteArray users;
    QDataStream out_users(&users, QIODevice::WriteOnly);
    std::vector<user> us_vec;
    m_users_db.get_users_vector(us_vec);
    out_users << us_vec;

    if (all == true)
    {
        sendToAllClient(bal);
        sendToAllClient(users);
    }
    else
    {
        sendToClient(socket, bal);
        sendToClient(socket, users);
    }
}

void server::slotNewUser()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), this, SLOT(slotLeaveUser()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    int user_id = m_users_db.add_new_user(socket);

    std::cout << "Added new user, id: " << user_id << std::endl;

    QByteArray id;
    QDataStream out_id(&id, QIODevice::WriteOnly);
    out_id << CMD_SEND_U_ID;
    out_id << user_id;
    sendToClient(socket, id);

    update(true);
}

void server::sendToAllClient(const QByteArray &data)
{
    std::vector<QTcpSocket *> sockets;
    m_users_db.get_all_sockets(sockets);

    int size = sockets.size();
    for (int i = 0; i < size; ++i)
    {
        sendToClient(sockets[i], data);
    }
}

void server::slotLeaveUser()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());

    int user_id = m_users_db.delete_user(socket);

    socket->deleteLater();

    std::cout << "Leave user, id: " << user_id << std::endl;

    QByteArray users;
    QDataStream out_users(&users, QIODevice::WriteOnly);
    std::vector<user> us_vec;
    m_users_db.get_users_vector(us_vec);
    out_users << us_vec;

    sendToAllClient(users);
}

void server::slotReadClient()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());

    while (socket->bytesAvailable())
    {
        int size = m_balloons.size(); //я пытался
        QDataStream in(socket);
        int balloon_id = 0;
        in >> balloon_id;
        delete_balloon(balloon_id, socket);
        if (m_balloons.size() < size)
            get_balloons(1);

        std::cout << "Delete balloon, id: " << balloon_id << std::endl;

        update(true);
    }
}

void server::sendToClient(QTcpSocket *socket, const QByteArray &data)
{
    socket->write(data);
    std::cout << "Sended data" << std::endl;
}
