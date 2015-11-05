#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "hand_detector.hpp"
#include "image_utils.hpp"

#define PI 3.1428

using namespace cv;
using namespace std;

HandDetector::HandDetector(WebcamImage* m, ImageUtils *op) :
    m(m), imageUtils(op)
{
    frameNumber=0;
}

DetectorResult HandDetector::processFrame() {
    frameNumber++;
    m->cap >> m->src;
    imageUtils->processImage(m);
    analyseContours();
    DetectorResult result;
    int biggestContour = findBiggestContour();
    if (biggestContour != -1) {
        result = getResult(contours[biggestContour]);
        //showContour(biggestContour);
    }
    //showBinarized();
    return result;
}

void HandDetector::showBinarized() {
    pyrDown(m->bw, m->bw);
    pyrDown(m->bw, m->bw);
    Rect upRight(Point(3*m->src.cols/4, 0), m->bw.size());
    vector<Mat> channels;
    Mat result;
    for(int i=0;i<3;i++) {
        channels.push_back(m->bw);
    }
    merge(channels,result);
    result.copyTo(m->src(upRight));
    imshow("Bubbles", m->src);
}

void HandDetector::showContour(int index) {
    drawContours(m->src, contours, index,
                 Scalar(0, 255,0), 2, 8, vector<Vec4i>(), 0, Point());
}

DetectorResult HandDetector::getResult(vector<Point> contour) {
    Point point = getHighestPoint(contour);
    circle(m->src, point, 5, Scalar(255, 0, 0), 4);
    return {true, point};
}

Point HandDetector::getHighestPoint(vector<Point> contour) {
    int minY = 1000;
    Point result;
    for (int i = 0; i < contour.size(); i++) {
        Point cur = contour[i];
        if (cur.y < minY) {
            minY = cur.y;
            result = cur;
        }
    }
    return result;
}

void HandDetector::analyseContours() {
    Mat aBw;
    pyrUp(m->bw, m->bw);
    m->bw.copyTo(aBw);
    findContours(aBw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}

int HandDetector::findBiggestContour() {
    int index = -1;
    int maxSize = 0;
    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > maxSize) {
            maxSize = contours[i].size();
            index = i;
        }
    }
    return index;
}
