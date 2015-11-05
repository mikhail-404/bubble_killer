#ifndef BALLOON_HPP
#define BALLOON_HPP

#include <tuple>

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
    int id();
    //
    void next_position();
    //
    int radius() const;
    //
    bool check_position();
    //
    int x() const;
    //
    int y() const;
    //
    BALLOON_TYPE balloon_type() const;
    //
    std::tuple <int, int, int> color() const;
    //

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
