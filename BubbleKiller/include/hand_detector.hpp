#ifndef _HAND_DETECTOR_
#define _HAND_DETECTOR_

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "webcam_image.hpp"
#include "imageop.hpp"

using namespace cv;
using namespace std;

struct DetectorResult {
    bool recognized;
    int numFingers;
    Vec2i pos;
};

class HandDetector {
public:
    HandDetector(WebcamImage* m, ImageOp *op);
    void updateFrame();
    void showWindows();
    DetectorResult getResult();

private:
    WebcamImage *m;
    ImageOp *op;
    vector<vector<Point> > contours;
    vector<vector<int> >hullI;
    vector<vector<Point> >hullP;
    vector<vector<Vec4i> > defects;
    vector <Point> fingerTips;
    Rect rect;
    void printGestureInfo(Mat src);
    int cIdx;
    int frameNumber;
    int mostFrequentFingerNumber;
    int nrOfDefects;
    Rect bRect;
    bool isHand;
    bool detectIfHand();
    void initVectors();
    void getFingerNumber();
    void eleminateDefects(WebcamImage* m);
    void getFingerTips(WebcamImage* m);
    void drawFingerTips(WebcamImage* m);
    int findBiggestContour();

    string bool2string(bool tf);
    int fontFace;
    int prevNrFingerTips;
    void checkForOneFinger(WebcamImage* m);
    float getAngle(Point s,Point f,Point e);
    vector<int> fingerNumbers;
    string intToString(int number);
    void computeFingerNumber();
    void drawNewNumber(WebcamImage* m);
    vector<int> numbers2Display;
    void addFingerNumberToVector();
    Scalar numberColor;
    int nrNoFinger;
    float distanceP2P(Point a,Point b);
    void removeRedundantEndPoints(vector<Vec4i> newDefects);
    void removeRedundantFingerTips();
    void makeContours(WebcamImage* m);
    void myDrawContours(WebcamImage *m);
};

#endif
