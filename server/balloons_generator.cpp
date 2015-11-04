#include "balloons_generator.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

BalloonsGenerator::BalloonsGenerator(int width, int height)
    : m_width(width),
      m_height(height)
{
    m_idx = 0;
}

Balloon* BalloonsGenerator::next_balloon()
{
    // sin coef: 1..10
    double rand_coef = 1.0 * (std::rand() % 11 + 1);
    // speed of trace: 10..30
    double speed = 1.0 * (std::rand() % 21 + 10);
    // radius of balloon: 15..45
    int radius = std::rand() % 31 + 15;
    // random start position: m_radius .. m_width - m_radius
    int random_x_pos = std::rand() % (m_width - radius + 1) + radius;
    //
    Position start_position = std::make_pair <int, int>(random_x_pos - 0, m_height - 0);
    //
    return new Balloon(m_idx++, start_position, rand_coef, speed, radius, (rand() % 2 ? Balloon::NORMAL : Balloon::BOMB)); //position_center = (0,0)
}
