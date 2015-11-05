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

    m_timer_balloons = new QTimer();
    connect(m_timer_balloons, SIGNAL(timeout()), this, SLOT(slotValidateBal()));
    m_timer_balloons->setInterval(TIME);

    std::cout << "Server started" << std::endl;

    get_balloons(20);
}
server::~server()
{
    delete m_server;
    delete m_timer_balloons;
}

void server::get_balloons(int num_ballooons)
{
    m_gen.increase_balloon_speed();

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
            std::cout << "Delete balloon, id: " << balloon_id << std::endl;
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

    m_timer_balloons->start();
}

void server::slotValidateBal()
{
    bool change = false;
    int num_bal = 0;
    for (auto iter = m_balloons.begin(); iter != m_balloons.end();)
    {
        iter->next_position();

        if (!iter->check_position())
        {
            auto next_iter = iter;
            ++next_iter;
            std::cout << "Timer: delete balloon, id " << iter->get_id() << std::endl;
            m_balloons.erase(iter);
            iter = next_iter;

            change = true;
            ++num_bal;
        }
        else
        {
            ++iter;
        }
    }
    if (change == true)
    {
        get_balloons(num_bal);
        update(true);
    }
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

    if (m_users_db.num_users() == 0)
    {
        m_timer_balloons->stop();
    }
}

void server::slotReadClient()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());

    while (socket->bytesAvailable())
    {
        int size = m_balloons.size();
        QDataStream in(socket);
        int balloon_id = 0;
        in >> balloon_id;
        delete_balloon(balloon_id, socket);
        if (m_balloons.size() < size)
        {
            get_balloons(1);
            update(true);
        }
    }
}

void server::sendToClient(QTcpSocket *socket, const QByteArray &data)
{
    socket->write(data);
    std::cout << "Sended data" << std::endl;
}
