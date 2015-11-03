#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "balloons_generator.hpp"
#include "balloon.hpp"

class Game
{
public:
    Game(int balloon_counts);
    ~Game();
    void start_game();


private:
    cv::Mat                m_frame;
    int                    m_balloon_count;
    std::vector <Balloon*> m_balloons;
    BalloonsGenerator      *m_generator;
};

#endif // GAME_HPP
