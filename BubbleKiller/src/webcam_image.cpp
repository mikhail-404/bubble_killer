#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "webcam_image.hpp"

using namespace cv;

WebcamImage::WebcamImage() {
    cameraSrc = 0;
    cap = VideoCapture(cameraSrc);
}

WebcamImage::WebcamImage(int webCamera) {
	cameraSrc=webCamera;
	cap=VideoCapture(webCamera);
}

