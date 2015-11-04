#ifndef USER_H
#define USER_H

#define DEFAULT_MAX_ID 4

#include <QtNetwork/QTcpSocket>
#include <map>
#include <queue>
#include <utility>
#include <vector>

struct user
{
   int user_id;
   int score;
};

class users_db
{

public:

    users_db();

    int add_new_user(QTcpSocket* user_socket);
    int delete_user(QTcpSocket* user_soket);
    int get_user_id();
    void ret_user_id(int id);
    void get_users_vector(std::vector<user> &users_vec);
    void get_all_sockets(std::vector<QTcpSocket*> &soc_vector);
    void set_score(QTcpSocket* user_soket, int s);

private:
    std::map<QTcpSocket*, user> m_users;

    int m_max_user_id;
    std::priority_queue<int, std::vector<int>, std::greater<int>> m_queue_id;
};

#endif // USER_H
