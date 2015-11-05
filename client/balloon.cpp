#include "balloon.hpp"
#include <iostream>
#include <utility>

Balloon::Balloon(int id, Position start_position, double coef, double speed, int radius, Balloon::BALLOON_TYPE type)
    : m_id(id),
      m_start_position(start_position),
      m_coefficient(coef),
      m_speed(speed),
      m_radius(radius),
      m_balloon_type(type),
      m_center_point(start_position)
{
    m_color = std::make_tuple(0, 255, 0);
    if (m_balloon_type == BOMB)
        m_color = std::make_tuple(0, 0, 0);
}

Balloon::Balloon(const Balloon &bal)
{
    m_id = bal.m_id;
    m_start_position = bal.m_start_position;
    m_coefficient = bal.m_coefficient;
    m_speed = bal.m_speed;
    m_radius = bal.m_radius;
    m_center_point = bal.m_center_point;
    m_color = bal.m_color;
    m_balloon_type = bal.m_balloon_type;
}

int Balloon::id() const
{
    return m_id;
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

bool Balloon::check_position()
{
    return m_center_point.second >= 0;
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

void Balloon::print() const
{
    std::cout << m_id << std::endl;
}

bool Balloon::is_bang() const
{
    return false;
}

std::tuple <int, int, int> Balloon::color() const
{
    return m_color;
}
