#ifndef SERIALIZATION
#define SERIALIZATION

#include <QDataStream>
#include <vector>

#include "balloon.hpp"

struct User
{
    int user_id;
    int score;
};

#define CMD_BALLON 0
#define CMD_USER 1
#define CMD_SEND_U_ID 2


QDataStream &operator>>(QDataStream &stream, Balloon &bal);
QDataStream &operator>>(QDataStream &stream, User &us);
QDataStream &operator>>(QDataStream &stream, std::vector<User> &users);
QDataStream &operator>>(QDataStream &stream, std::vector<Balloon> &balloons);


QDataStream &operator<<(QDataStream &stream, const Balloon &bal);
QDataStream &operator<<(QDataStream &stream, const User &us);
QDataStream &operator<<(QDataStream &stream, const std::vector<User> &users);
QDataStream &operator<<(QDataStream &stream, const std::vector<Balloon> &balloons);


#endif // SERIALIZATION

