#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "rect.hpp"
#include "webcam_image.hpp"

using namespace cv;
using namespace std;

BorderedRect::BorderedRect() {
    upperCorner = Point(0, 0);
    lowerCorner = Point(0, 0);
}

BorderedRect::BorderedRect(Point u_corner, Point l_corner, Mat src) {
    upperCorner = u_corner;
    lowerCorner = l_corner;
    color = Scalar(0, 255, 0);
    border_thickness = 2;
    rect = src(Rect(u_corner.x, u_corner.y, l_corner.x-u_corner.x, l_corner.y-u_corner.y));
}

void BorderedRect::draw_rectangle(Mat src) {
    rectangle(src, upperCorner, lowerCorner, color, border_thickness);
}
