#include "game.hpp"
#include <iostream>

#include <thread>
#include <chrono>
#include <ctime>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>

#include "webcam_image.hpp"
#include "rect.hpp"
#include "hand_detector.hpp"
#include "image_utils.hpp"

Game::Game(int balloon_counts)
    : m_balloon_count(balloon_counts)
{
    m_generator = new BalloonsGenerator(width, height);

    for(int i = 0; i < m_balloon_count; ++i)
        m_balloons.push_back(m_generator->next_balloon());
}

Game::~Game()
{
    for(Balloon* b : m_balloons)
        delete b;
    delete m_generator;
}

void Game::updateBalloons(std::vector<Balloon> balloons)
{
    for(Balloon balloon : balloons)
        m_balloons.push_back(&balloon);
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

string toString(User user)
{
    stringstream ss;
    ss << "Id: " << user.id << ", score: " << user.score;
    return ss.str();
}

void Game::printScores(const cv::Mat& src)
{
    Scalar fColor(0, 255, 0);
    int xpos = src.cols / 1.5;
    int ypos = src.rows / 1.4;
    float fontSize = 1.5;
    int lineChange = 30;
    int fontFace = FONT_HERSHEY_PLAIN;
    int thickness = 2;
    for(size_t i = 0; i < m_scores.size(); i++)
    {
        string info = toString(m_scores[i]);
        putText(src, info, Point(ypos,xpos), fontFace, fontSize, fColor, thickness);
        xpos -= lineChange;
    }
}

string balloonName(int i) {
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
    User user{1, 0};
    std::vector<User>(scores);
    scores.push_back(user);
    setScores(scores);

    int numBalloons = 5;
    Mat balloonImages[numBalloons];
    for (int i = 0; i < numBalloons; i++) {
       balloonImages[i] = imread(balloonName(i), -1);
    }
    Mat bombImage = imread("images/bomb.png", -1);
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
                while (radius)
                {
                    cv::circle(m.src, cv::Point(b->x() + 2. * std::sin(b->y() * 2.), b->y()), radius, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                    radius >>= 1;
                }
                auto next_iter = it;
                ++next_iter;
                m_balloons.erase(it);

                // score counting
                user.score += (b->balloon_type() == Balloon::BOMB) ? -2 : 1;
                vector<User> scores;
                scores.push_back(user);
                setScores(scores);
                //
                delete b;
                it = next_iter;
                balloons_insert_vector = m_generator->next_n_balloons(1);
                m_balloons.insert(m_balloons.end(), balloons_insert_vector.begin(), balloons_insert_vector.end());
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

            /*cv::circle(m.src, center, b->radius(),
                       cv::Scalar(std::get<0>(c), std::get<1>(c), std::get<2>(c)), 2, cv::LINE_8);*/
            imageUtils.overlayImage(m.src, resizedBalloon, corner);
            b->next_position();
            // sift up
            // check overflow
            if (!b->check_position())
            {
                // remove b objec
                auto next_iter = it;
                ++next_iter;
                m_balloons.erase(it);
                it = next_iter;
                delete b;
                m_balloons.push_back(m_generator->next_balloon());
            }
            else
                ++it;
        }
        m_generator->increase_balloon_speed();
        printScores(m.src);
        cv::imshow("Bubbles", m.src);
        cv::waitKey(1);
    }

    destroyAllWindows();
    m.cap.release();

    for(Balloon* b : balloons_insert_vector)
        delete b;
}
