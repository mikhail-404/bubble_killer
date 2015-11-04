#ifndef ROI 
#define ROI

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class BorderedRect {
public:
    BorderedRect();
    BorderedRect(Point upper_corner, Point lower_corner,Mat src);
    Point upper_corner, lower_corner;
    Mat roi_ptr;
    Scalar color;
    int border_thickness;
    void draw_rectangle(Mat src);
};

#endif
