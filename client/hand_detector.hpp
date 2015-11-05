#ifndef _HAND_DETECTOR_
#define _HAND_DETECTOR_

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "webcam_image.hpp"
#include "image_utils.hpp"

using namespace cv;
using namespace std;

struct DetectorResult {
    bool recognized;
    Point pos;
};

class HandDetector {
public:
    HandDetector(WebcamImage* m, ImageUtils *imageUtils);
    DetectorResult processFrame();
    bool checkBalloon(int x, int y, int radius);

private:
    WebcamImage *m;
    ImageUtils *imageUtils;
    int frameNumber;
    Mat prevFrame, movementImage, movementGray;
    vector<vector<Point> > contours;

    void analyseContours();
    int findBiggestContour();
    DetectorResult getResult(vector<Point> contour);
    Point getHighestPoint(vector<Point> contour);
    void showBinarized();
    void showContour(int index);
};

#endif
