#include "balloons_generator.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <memory>

BalloonsGenerator::BalloonsGenerator(int width, int height)
    : m_width(width),
      m_height(height)
{
    m_idx = 0;
    m_speed = 0.0;
}

// danger *** warning
//     |
//     V
Balloon* BalloonsGenerator::next_balloon()
{
    // sin coef: 1..10
    double rand_coef = 1.0 * (std::rand() % 11 + 1);
    // speed of trace: 10..30
    double speed = 0.2 * (std::rand() % 11 + 10) + m_speed;
    // radius of balloon: 15..45
    int radius = std::rand() % 31 + 15;
    // random start position: m_radius .. m_width - m_radius
    int random_x_pos = std::rand() % (m_width - radius + 1) + radius;
    //
    Position start_position = std::make_pair <int, int>(random_x_pos - 0, m_height - 0);
    //
    return new Balloon(m_idx++, start_position, rand_coef, speed, radius, (rand() % 5 ? Balloon::NORMAL : Balloon::BOMB));
}

void BalloonsGenerator::increase_balloon_speed()
{
    m_speed += 0.0015;
}

std::vector<Balloon*> BalloonsGenerator::next_n_balloons(const int count)
{
    std::vector <Balloon*> ballons(count);
    for(size_t i = 0; i < count; ++i)
        ballons[i] = next_balloon();
    return ballons;
}
