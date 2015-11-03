#include "balloon.hpp"
#include <iostream>

Balloon::Balloon(int id, Position start_position, double coef, double speed, int radius, Balloon::BALLOON_TYPE type)
    : m_id(id),
      m_start_position(start_position),
      m_coefficient(coef),
      m_speed(speed),
      m_radius(radius),
      m_balloon_type(type),
      m_center_point(start_position)
{
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
    if (m_center_point.second < 0)
        m_center_point.second = pos;
}

int Balloon::x() const
{
    return m_center_point.first;
}

int Balloon::y() const
{
    return m_center_point.second;
}

void Balloon::print() const
{
    std::cout << m_center_point.first << " " << m_center_point.second << " " << m_speed << " " << m_coefficient << " " << m_radius << std::endl;
}
