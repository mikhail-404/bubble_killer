#include "game.hpp"
#include <iostream>

#include <thread>
#include <chrono>

Game::Game(int balloon_counts)
    : m_balloon_count(balloon_counts)
{
    m_frame = cv::imread("/home/mikhail/Programming/DevDays/BubbleKiller/images/t.png");

    m_generator = new BalloonsGenerator(m_balloon_count, m_frame.size().width, m_frame.size().height);

    for(int i = 0; i < m_balloon_count; ++i)
    {
        m_balloons.push_back(m_generator->next_balloon());
        m_balloons[m_balloons.size() - 1]->print();
    }
}

Game::~Game()
{
    for(Balloon* b : m_balloons)
        delete b;
    delete m_generator;
}

void check(int &y, int p)
{
    if (y < 0)
    {
        std::cout << y << std::endl;
        y = p;
    }
}

void Game::start_game()
{
    cv::Size size = m_frame.size();
    int x = size.width;
    int y = size.height;
    cv::Mat temp;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        temp = m_frame.clone();

        for(Balloon* b : m_balloons)
        {
            cv::circle(temp, cv::Point(b->x() + 2. * std::sin(b->y() * 2.), b->y()), b->radius(), cv::Scalar(0, 255, 0));
            b->next_position();
            b->check_position(y);
        }

        cv::imshow("Image", temp);
        cv::waitKey(1);
    }
}

