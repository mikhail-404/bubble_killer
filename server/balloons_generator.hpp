#ifndef BALLOONS_GENERATOR
#define BALLOONS_GENERATOR

#include "balloon.hpp"

class BalloonsGenerator
{
public:
    // win size
    BalloonsGenerator(int width, int geight);
    Balloon next_balloon();
    void increase_balloon_speed();
    void reset_speed();

private:
    int m_width;
    int m_height;
    int m_idx;
    double m_speed;
};

#endif // BALLOONS_GENERATOR
