#ifndef _MYIMAGE_
#define _MYIMAGE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class WebcamImage {
public:
    WebcamImage(int webCamera, int width, int height);
    Mat srcLR;
    Mat src;
    Mat bw;
    vector<Mat> bwList;
    VideoCapture cap;
};

#endif
