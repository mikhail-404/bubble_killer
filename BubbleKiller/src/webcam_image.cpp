#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "webcam_image.hpp"

using namespace cv;

WebcamImage::WebcamImage(int camera, int width, int height) {
    cap = VideoCapture(camera);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
}

