#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <vector>

#include "users_db.hpp"
#include "serialization.hpp"
#include "balloon.hpp"
#include "balloons_generator.hpp"


#define WIDTH 640
#define HEIGHT 480
#define TIME 150 //milliseconds


class server : public QObject
{
    Q_OBJECT
public:
    server();
    ~server();
    void get_balloons(int num_ballooons);
    void delete_balloon(int balloon_id, QTcpSocket *socket);
    void update(bool all, QTcpSocket *socket = nullptr);

signals:

public slots:
    void slotNewUser();
    void slotReadClient();
    void slotLeaveUser();
    void slotValidateBal();

private:
    const int m_nPort = 8080;
    QTcpServer *m_server;
    users_db m_users_db;
    BalloonsGenerator m_gen;
    std::list<Balloon> m_balloons;
    QTimer *m_timer_balloons;

    void sendToClient(QTcpSocket *socket, const QByteArray &data);
    void sendToAllClient(const QByteArray &data);
};

#endif // SERVER_H
