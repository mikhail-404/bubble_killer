#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "webcam_image.hpp"
#include "rect.hpp"

const int SAMPLES = 7;

class ImageUtils
{
public:
    ImageUtils();
    void calculatePalmColor(WebcamImage* m);
    void processImage(WebcamImage* m);

private:
    int lowerBound[3];
    int upperBound[3];
    int avgBGR[3];
    int avgColor[SAMPLES][3];
    vector <BorderedRect> rectangles;

    void normalizeColors();
    void getAverageColor(WebcamImage *m, BorderedRect& rect, int avg[3]);
    void produceBinaries(WebcamImage *m);
    int  getMedian(vector<int> val);
    void computeAverage(WebcamImage *m);
    void hsv2Bgr(int hsv[3], int bgr[3], Mat src);
    void initTrackbars();
};

#endif // IMAGE_UTILS_H
