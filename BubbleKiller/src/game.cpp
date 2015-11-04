#include "game.hpp"
#include <iostream>

#include <thread>
#include <chrono>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>

#include "webcam_image.hpp"
#include "rect.hpp"
#include "hand_detector.hpp"
#include "imageop.hpp"

Game::Game(int balloon_counts)
    : m_balloon_count(balloon_counts)
{
    m_frame = cv::imread("/home/mikhail/Programming/DevDays/BubbleKiller/images/t.png");

    m_generator = new BalloonsGenerator(m_frame.size().width, m_frame.size().height);

    for(int i = 0; i < m_balloon_count; ++i)
    {
        m_balloons.push_back(m_generator->next_balloon());
    }
}

Game::~Game()
{
    for(Balloon* b : m_balloons)
        delete b;
    delete m_generator;
}

double euclideanDist(Point& p, Point& q)
{
    Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

void Game::start_game()
{
    WebcamImage m(0);
    m.cap >> m.src;
    cv::Size size = m.src.size();
    int y = size.height;
    ImageOp op;
    HandDetector detector(&m, &op);

    //out.open("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, m.src.size(), true);
    namedWindow("Bubbles", CV_WINDOW_FULLSCREEN);
    op.calculatePalmColor(&m);
    //

    while (true)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //temp = m_frame.clone();
                ///
        DetectorResult result = detector.updateFrame();
        //cout << result.recognized << " " << result.pos << endl;
        cv::Point finger_pos = result.pos;
        //

        for(auto it = m_balloons.begin(); it != m_balloons.end(); )
        {
            Balloon *b = *it;
            // remove balls if condition
            //if (cv::norm(finger_pos, cv::Point(b->x(), b->y())) <= b->radius())
            cv::Point p(b->x(), b->y());
            if (euclideanDist(finger_pos, p) <= b->radius())
            {
                int radius = b->radius() * 1.5;
                while (radius)
                {
                    cv::circle(m.src, cv::Point(b->x() + 2. * std::sin(b->y() * 2.), b->y()), radius, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                    radius >>= 1;
                }
                auto next_iter = it;
                ++next_iter;
                m_balloons.erase(it);
                delete b;
                it = next_iter;
                m_balloons.push_back(m_generator->next_balloon());
                continue;
            }
            // get color of balloon (bomb type or another)
            auto c = b->color();
            cv::circle(m.src, cv::Point(b->x() + 10 * std::sin(b->y() * 0.1), b->y()), b->radius(), cv::Scalar(std::get<0>(c), std::get<1>(c), std::get<2>(c)), 2, cv::LINE_8);
            // sift up
            b->next_position();
            // check overflow
            b->check_position(y);
            ++it;
        }

        cv::imshow("Bubbles", m.src);
        cv::waitKey(1);
    }

    destroyAllWindows();
    m.cap.release();
}
