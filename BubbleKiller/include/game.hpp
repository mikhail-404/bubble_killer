#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <list>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "balloons_generator.hpp"
#include "balloon.hpp"

struct User {
    int id;
    int score;
};

class Game
{
public:
    Game(int balloon_counts);
    ~Game();
    void start_game();
    void printScores(const cv::Mat &src);
    void setScores(std::vector<User>& scores);
    void updateBalloons(std::vector<Balloon> balloons);

private:
    cv::Mat                m_frame;
    int                    m_balloon_count;
    std::list <Balloon*>   m_balloons;
    BalloonsGenerator      *m_generator;
    std::vector<User> m_scores;
};

#endif // GAME_HPP
