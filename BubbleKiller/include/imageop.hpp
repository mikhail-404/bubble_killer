#ifndef IMAGEOP_H
#define IMAGEOP_H

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "webcam_image.hpp"
#include "rect.hpp"

#define NSAMPLES 7

class ImageOp
{
public:
    ImageOp();
    void col2origCol(int hsv[3], int bgr[3], Mat src);
    void calculatePalmColor(WebcamImage* m);

    void normalizeColors();
    void getAvgColor(WebcamImage *m, BorderedRect roi,int avg[3]);
    void produceBinaries(WebcamImage *m);
    int getMedian(vector<int> val);
    void computeAverage(WebcamImage *m);
    void initTrackbars();

private:
    int square_len;
    int avgColor[NSAMPLES][3];
    int c_lower[NSAMPLES][3];
    int c_upper[NSAMPLES][3];
    int avgBGR[3];
    int nrOfDefects;
    int iSinceKFInit;
    struct dim{int w; int h;} boundingDim;
    Mat edges;
    vector <BorderedRect> roi;
    vector <KalmanFilter> kf;
    vector <Mat_<float> > measurement;
};

#endif // IMAGEOP_H
