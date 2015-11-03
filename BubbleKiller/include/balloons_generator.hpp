#ifndef BALLOONS_GENERATOR
#define BALLOONS_GENERATOR

#include "balloon.hpp"

class BalloonsGenerator
{
public:
    // win size
    BalloonsGenerator(int balloon_count, int width, int geight);
    Balloon* next_balloon();

private:
    int m_balloon_count;
    int m_width;
    int m_height;
    int m_idx;
};

#endif // BALLOONS_GENERATOR
