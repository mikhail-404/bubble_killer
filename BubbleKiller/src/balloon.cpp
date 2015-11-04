#include "balloon.hpp"
#include <iostream>
#include <utility>

Balloon::Balloon(int id, Position start_position, double coef, double speed, int radius, Balloon::BALLOON_TYPE type)
    : m_id(id),
      m_center_point(start_position),
      m_coefficient(coef),
      m_speed(speed),
      m_radius(radius),
      m_balloon_type(type)

{
    // if balloon has normal type
    m_color = std::make_tuple(0, 255, 0);
    // ... or bomb type
    if (m_balloon_type == BOMB)
        m_color = std::make_tuple(0, 0, 0);
}

void Balloon::set_position(Position pos)
{
    m_center_point = pos;
}

const Position Balloon::position() const
{
    return m_center_point;
}

void Balloon::next_position()
{
    m_center_point.second -= m_speed;
}

int Balloon::radius() const
{
    return m_radius;
}

void Balloon::check_position(const int &pos)
{
    // for cycle trace
    if (m_center_point.second < 0)
        m_center_point.second = pos;
    // if you don't want such a cycle then remove current object
}

int Balloon::x() const
{
    return m_center_point.first;
}

int Balloon::y() const
{
    return m_center_point.second;
}

Balloon::BALLOON_TYPE Balloon::balloon_type() const
{
    return m_balloon_type;
}

std::tuple <int, int, int> Balloon::color() const
{
    return m_color;
}
