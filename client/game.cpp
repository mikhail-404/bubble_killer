#include "game.hpp"
#include <iostream>

#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>

#include "webcam_image.hpp"
#include "rect.hpp"
#include "hand_detector.hpp"
#include "image_utils.hpp"

Game::Game(int balloon_counts, QTcpSocket *tcpsocket, int id_user)
    : m_balloon_count(balloon_counts)
    , my_id(id_user)
{
    tcp = tcpsocket;
}

Game::~Game()
{
    for(Balloon* b : m_balloons)
        delete b;
}

void Game::updateBalloons(std::vector<Balloon> &balloons)
{
    for (int i = 0; i < m_balloons.size(); ++i)
    {
        bool flag = false;
        for (int j = 0; j < balloons.size(); ++j)
        {
            if(m_balloons[i]->id() == balloons[j].id())
                flag = true;
        }

        if (flag == false)
            m_balloons.erase(m_balloons.begin() + i);
     }

     for (int i = 0; i < balloons.size(); ++i)
     {
        if (balloons[i].y() == height)
        {
            Balloon *b = new Balloon(balloons[i]);
            m_balloons.push_back(b);
        }
     }
}

void Game::set_id(int id)
{
    my_id = id;
}

void Game::setScores(vector<User>& scores)
{
    m_scores = scores;
}

double distance(Point& p, Point& q)
{
    Point diff = p - q;
    return cv::sqrt(diff.x * diff.x + diff.y * diff.y);
}

string Game::toString(User user)
{
    stringstream ss;
    if (user.user_id == my_id)
    {
        ss << "I'm: " << user.user_id + 1 << std::setw(10) << ", score: " << user.score;
    }
    else
    {
        ss << "Player: " << user.user_id + 1 << std::setw(10) << ", score: " << user.score;
    }
    return ss.str();
}

void Game::printScores(const cv::Mat& src)
{
    Scalar fColor(0, 255, 0);
    Scalar fColor_my(0, 0, 255);
    int xpos = src.cols / 1.5;
    int ypos = 10;
    float fontSize = 1;
    int lineChange = 20;
    int fontFace = FONT_HERSHEY_PLAIN;
    int thickness = 2;
    for(size_t i = 0; i < m_scores.size(); i++)
    {
        string info = toString(m_scores[i]);
        if (m_scores[i].user_id == my_id)
            putText(src, info, Point(ypos,xpos), fontFace, fontSize, fColor_my, thickness);
        else
            putText(src, info, Point(ypos,xpos), fontFace, fontSize, fColor, thickness);
        xpos -= lineChange;
    }
}

string balloonName(int i)
{
    stringstream ss;
    ss << "images/balloon";
    ss << i;
    ss << ".png";
    return ss.str();
}

void Game::start_game()
{
    WebcamImage m(0, width, height);
    m.cap >> m.src;
    cv::Size size = m.src.size();
    int y = size.height;
    ImageUtils imageUtils;
    HandDetector detector(&m, &imageUtils);

    int numBalloons = 5;
    Mat balloonImages[numBalloons];
    for (int i = 0; i < numBalloons; i++) {
        balloonImages[i] = imread(balloonName(i), -1);
    }
    Mat bombImage = imread("images/bomb.png", -1);
    Mat bombExplosion = imread("images/bomb_explosion.png", -1);
    Mat balloonExplosion = imread("images/balloon_explosion.png", -1);
    Mat resizedBalloon;

    //out.open("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, m.src.size(), true);
    namedWindow("Bubbles", CV_WINDOW_FULLSCREEN);
    imageUtils.calculatePalmColor(&m);
    //
    std::vector <Balloon*> balloons_insert_vector;

    while (true)
    {
        DetectorResult result = detector.processFrame();
        cv::Point finger_pos = result.pos;

        for(auto it = m_balloons.begin(); it != m_balloons.end(); )
        {
            Balloon *b = *it;
            cv::Point p(b->x(), b->y());
            if (distance(finger_pos, p) <= b->radius())
            {
                int radius = b->radius() * 1.5;

                Point center = cv::Point(b->x() + 10 * std::sin(b->y() * 0.1), b->y());
                Point corner = Point(center.x - radius, center.y - radius);
                Size size = Size2i(3 * radius, 3 * radius);

                if (b->balloon_type() == Balloon::BOMB)
                {
                    resize(bombExplosion, resizedBalloon, size);
                }
                else
                {
                    resize(balloonExplosion, resizedBalloon, size);
                }
                imageUtils.overlayImage(m.src, resizedBalloon, corner);

                int id_balloon = b->id();

                QDataStream out(tcp);
                out << id_balloon;

                m_balloons.erase(it);

                // score counting
                for (int i = 0; i < m_scores.size(); ++i)
                {
                    if (m_scores[i].user_id == my_id)
                    {
                        m_scores[i].score += (b->balloon_type() == Balloon::BOMB) ? -2 : 1;
                        break;
                    }
                }

                delete b;
                continue;
            }
            // get color of balloon (bomb type or another)
            auto c = b->color();
            Point center = cv::Point(b->x() + 10 * std::sin(b->y() * 0.1), b->y());
            Point corner = Point(center.x - b->radius(), center.y - b->radius());
            Size size = Size2i(3 * b->radius(), 3 * b->radius());
            if (b->balloon_type() == Balloon::BOMB) {
                resize(bombImage, resizedBalloon, size);
            } else {
                int i = b->id() % numBalloons;
                resize(balloonImages[i], resizedBalloon, size);
            }
            imageUtils.overlayImage(m.src, resizedBalloon, corner);
            b->next_position();

            if (!b->check_position())
            {
                m_balloons.erase(it);
                delete b;
            }
            else
                ++it;
        }

        printScores(m.src);
        cv::imshow("Bubbles", m.src);
        cv::waitKey(1);
    }

    destroyAllWindows();
    m.cap.release();

    for(Balloon* b : balloons_insert_vector)
    delete b;
}
