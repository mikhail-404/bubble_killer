#include "users_db.hpp"

users_db::users_db()
    : m_max_user_id(0)
{
}

int users_db::add_new_user(QTcpSocket* user_socket)
{
    int user_id = get_user_id();
    user us = {user_id, 0};
    m_users.insert(std::pair<QTcpSocket*, user>(user_socket, us));

    return user_id;
}

int users_db::delete_user(QTcpSocket* user_soket)
{

    auto iter = m_users.find(user_soket);
    int user_id = iter->second.user_id;
    ret_user_id(user_id);
    m_users.erase(iter);

    return user_id;
}

int users_db::get_user_id()
{
    if (m_queue_id.empty())
    {
       m_queue_id.push(m_max_user_id);
       ++m_max_user_id;
    }

    int user_id = m_queue_id.top();
    m_queue_id.pop();

    return user_id;
}

void users_db::ret_user_id(int id)
{
    m_queue_id.push(id);
}

void users_db::get_users_vector(std::vector<user> &users_vec)
{
    for (auto iter = m_users.begin(); iter != m_users.end(); ++iter)
    {
        users_vec.push_back(iter->second);
    }
}

void users_db::get_all_sockets(std::vector<QTcpSocket*> &soc_vector)
{
    for (auto iter = m_users.begin(); iter != m_users.end(); ++iter)
    {
        soc_vector.push_back(iter->first);
    }
}

void users_db::set_score(QTcpSocket* user_soket, int s)
{
    m_users.at(user_soket).score += s;
}

int users_db::num_users()
{
    m_users.size();
}
