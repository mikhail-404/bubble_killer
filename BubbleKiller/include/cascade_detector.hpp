#ifndef CASCADE_DETECTOR_HPP
#define CASCADE_DETECTOR_HPP

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class CascadeDetector {
public:
    CascadeDetector();
    void detectAndDraw(Mat& image);

private:
    CascadeClassifier cascade;
    CascadeClassifier nestedCascade;
    const String cascadeName = "/home/kirill/aGest.xml";
    const String cascadeName2 = "/home/kirill/haarcascade.xml";
};

#endif // CASCADE_DETECTOR_HPP

