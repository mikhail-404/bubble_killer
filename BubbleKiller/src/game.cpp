#include "game.hpp"
#include <iostream>

#include <thread>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

Game::Game(int balloon_counts)
    : m_balloon_count(balloon_counts)
{
    m_frame = cv::imread("/home/mikhail/Programming/DevDays/BubbleKiller/images/t.png");

    m_generator = new BalloonsGenerator(m_frame.size().width, m_frame.size().height);

    for(int i = 0; i < m_balloon_count; ++i)
        m_balloons.push_back(m_generator->next_balloon());
}

Game::~Game()
{
    for(Balloon* b : m_balloons)
        delete b;
    delete m_generator;
}

void Game::start_game()
{
    cv::Size size = m_frame.size();
    int x = size.width;
    int y = size.height;
    cv::Mat temp;

    while (true)
    {
        temp = m_frame.clone();
        for(auto it = m_balloons.begin(); it != m_balloons.end(); )
        {
            Balloon *b = *it;
            if (b->y() <= y / 2)
            {
                int radius = b->radius() * 2;
                while (radius)
                {
                    cv::circle(temp, cv::Point(b->x() + 2. * std::sin(b->y() * 2.), b->y()), radius, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                    radius >>= 1;
                }
                auto next_iter = it;
                ++next_iter;
                m_balloons.erase(it);
                it = next_iter;
                m_balloons.push_back(m_generator->next_balloon());
                continue;
            }
            cv::circle(temp, cv::Point(b->x() + 2. * std::sin(b->y() * 2.), b->y()), b->radius(), cv::Scalar(0, 255, 0), 2, cv::LINE_8);
            b->next_position();
            b->check_position(y);
            ++it;
        }

        cv::imshow("Image", temp);
        cv::waitKey(1);
    }
}

