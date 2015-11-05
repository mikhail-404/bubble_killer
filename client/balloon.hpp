#ifndef BALLOON_HPP
#define BALLOON_HPP

#include <tuple>
#include <QDataStream>
typedef std::pair <int, int> Position;

class Balloon
{
public:
    enum BALLOON_TYPE {BOMB, NORMAL};

    // id of balloon, rate movements, balloon type
    Balloon(int id, Position start_pos, double coef, double speed, int radius, BALLOON_TYPE type);
    //
    void set_position(Position pos);
    // center of balloon
    const Position position() const;
    //
    void next_position();
    //
    int radius() const;
    //
    void check_position(const int &pos);
    //
    int x() const;
    //
    int y() const;
    //
    BALLOON_TYPE balloon_type() const;
    //
    std::tuple <int, int, int> color() const;
    //

    Balloon(){};

    int get_id() const;

    friend QDataStream &operator>>(QDataStream &stream, Balloon &bal);
    friend QDataStream &operator<<(QDataStream &stream, const Balloon &bal);

private:
    int                        m_id;
    Position                   m_center_point;
    double                     m_coefficient;
    double                     m_speed;
    int                        m_radius;
    std::tuple <int, int, int> m_color;
    BALLOON_TYPE               m_balloon_type;
};

#endif // BALLOON_HPP
