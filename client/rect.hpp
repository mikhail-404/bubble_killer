#ifndef ROI 
#define ROI

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class BorderedRect {
public:
    Mat rect;

    BorderedRect();

    BorderedRect(Point upperCorner, Point lowerCorner,Mat src);

    void draw_rectangle(Mat src);
private:
    Point upperCorner, lowerCorner;
    int border_thickness;
    Scalar color;
};

#endif
