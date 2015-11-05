#include "serialization.hpp"
#include <iostream>
#include <utility>
using namespace std;

QDataStream &operator>>(QDataStream &stream, Balloon &bal)
{
    int x =0;
    int y = 0;
    int z = 0;
    int type = 0;

    stream >> bal.m_id;
    stream >> x;
    stream >> y;
    bal.m_start_position = Position(x, y);
    stream >> bal.m_coefficient;
    stream >> bal.m_speed;
    stream >> bal.m_radius;
    stream >> x;
    stream >> y;
    bal.m_center_point = Position(x, y);
    stream >> type;
    bal.m_balloon_type = type == 0 ? Balloon::BOMB : Balloon::NORMAL;

    stream >> x;
    stream >> y;
    stream >> z;

    bal.m_color = std::make_tuple(x, y, z);

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Balloon &bal)
{
    stream << bal.m_id;
    stream << bal.m_start_position.first;
    stream << bal.m_start_position.second;
    stream << bal.m_coefficient;
    stream << bal.m_speed;
    stream << bal.m_radius;
    stream << bal.m_center_point.first;
    stream << bal.m_center_point.second;
    stream << bal.m_balloon_type;
    stream << std::get<0>(bal.m_color);
    stream << std::get<1>(bal.m_color);
    stream << std::get<2>(bal.m_color);

    return stream;
}

QDataStream &operator>>(QDataStream &stream, User &us)
{
    stream >> us.user_id;
    stream >> us.score;

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const User &us)
{
    stream << us.user_id;
    stream << us.score;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, std::vector<User> &users)
{
    int size = 0;

    stream >> size;
    users.resize(size);

    for (int i = 0; i < size; ++i)
    {
        stream >> users[i];
    }

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const std::vector<User> &users)
{
    int size = users.size();
    stream << (int)CMD_USER;
    stream << size;

    for (int i = 0; i < size; ++i)
    {
        stream << users[i];
    }

    return stream;
}

QDataStream &operator>>(QDataStream &stream, std::vector<Balloon> &balloons)
{
    int size = 0;
    stream >> size;
    balloons.resize(size);

    for (int i = 0; i < size; ++i)
    {
        stream >> balloons[i];
    }

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const std::vector<Balloon> &balloons)
{
    int size = balloons.size();
    stream << CMD_BALLON;
    stream << size;

    for (int i = 0; i < size; ++i)
    {
        stream << balloons[i];
    }

    return stream;
}



