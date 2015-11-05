#include "image_utils.hpp"

ImageUtils::ImageUtils() {
    lowerBound[0] = 12;
    lowerBound[1] = 30;
    lowerBound[2] = 80;
    upperBound[0] = 7;
    upperBound[1] = 40;
    upperBound[2] = 80;
}

void ImageUtils::processImage(WebcamImage* m) {
    pyrDown(m->src, m->srcLR);
    blur(m->srcLR, m->srcLR, Size(3, 3));
    cvtColor(m->srcLR, m->srcLR, CV_BGR2HLS);
    produceBinaries(m);
    cvtColor(m->srcLR, m->srcLR, CV_HLS2BGR);
}

void printText(Mat src, string text) {
    int fontFace = FONT_HERSHEY_SIMPLEX;
    putText(src, text, Point(src.cols / 3, src.rows / 10), fontFace, 0.8f,
            Scalar(255, 255, 255), 2);
}

void ImageUtils::calculatePalmColor(WebcamImage* m) {
    m->cap >> m->src;
    flip(m->src,m->src,1);

    int dist = 20;
    rectangles.push_back(BorderedRect(
                             Point(m->src.cols / 3, m->src.rows / 6),
                             Point(m->src.cols / 3 + dist, m->src.rows / 6 + dist),
                             m->src));
    rectangles.push_back(BorderedRect(
                             Point(m->src.cols / 4, m->src.rows / 2),
                             Point(m->src.cols / 4 + dist, m->src.rows / 2 + dist),
                             m->src));
    rectangles.push_back(BorderedRect(
                             Point(m->src.cols / 3, m->src.rows / 1.5),
                             Point(m->src.cols / 3+dist, m->src.rows / 1.5+dist),
                             m->src));

    for(int i = 0; i < 50; i++) {
        m->cap >> m->src;
        flip(m->src,m->src,1);
        for(int j = 0; j < SAMPLES; j++){
            rectangles[j].draw_rectangle(m->src);
        }
        string imgText = string("Put your hand over rectangles");
        printText(m->src, imgText);

        imshow("Bubbles", m->src);
        if(cv::waitKey(30) >= 0) {
            break;
        }
    }
    computeAverage(m);
    //initTrackbars();
}

int ImageUtils::getMedian(vector<int> val) {
    size_t size = val.size();
    sort(val.begin(), val.end());
    if (size % 2 == 0)  {
        return val[size / 2 - 1] ;
    } else {
        return val[size / 2];
    }
}

void ImageUtils::getAverageColor(WebcamImage *m, BorderedRect& rect, int avg[3]) {
    Mat r;
    rect.rect.copyTo(r);
    vector<int> hm, sm, lm;
    for(int i = 2; i < r.rows - 2; i++) {
        for(int j = 2; j < r.cols - 2; j++) {
            hm.push_back(r.data[r.channels() * (r.cols * i + j) + 0]) ;
            sm.push_back(r.data[r.channels() * (r.cols * i + j) + 1]) ;
            lm.push_back(r.data[r.channels() * (r.cols * i + j) + 2]) ;
        }
    }
    avg[0] = getMedian(hm);
    avg[1] = getMedian(sm);
    avg[2] = getMedian(lm);
}

void ImageUtils::computeAverage(WebcamImage *m) {
    for(int i=0;i<30;i++){
        m->cap >> m->src;
        flip(m->src, m->src,1);
        cvtColor(m->src, m->src, CV_BGR2HLS);
        for(int j = 0; j < SAMPLES;j++) {
            getAverageColor(m, rectangles[j], avgColor[j]);
            rectangles[j].draw_rectangle(m->src);
        }
        cvtColor(m->src, m->src, CV_HLS2BGR);
        string imgText = string("Calibrating hand color");
        printText(m->src, imgText);
        imshow("Bubbles", m->src);
        if(cv::waitKey(30) >= 0) {
            break;
        }
    }
}

void ImageUtils::initTrackbars() {
    namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    createTrackbar("lower1","trackbars",&lowerBound[0],255);
    createTrackbar("lower2","trackbars",&lowerBound[1],255);
    createTrackbar("lower3","trackbars",&lowerBound[2],255);
    createTrackbar("upper1","trackbars",&upperBound[0],255);
    createTrackbar("upper2","trackbars",&upperBound[1],255);
    createTrackbar("upper3","trackbars",&upperBound[2],255);
}

void ImageUtils::normalizeColors() {
    for(int i = 0; i < SAMPLES; i++) {
        if ((avgColor[i][0] - lowerBound[0]) < 0) {
            lowerBound[0] = avgColor[i][0];
        }
        if ((avgColor[i][1] - lowerBound[1]) < 0){
            lowerBound[1] = avgColor[i][1];
        }
        if((avgColor[i][2] - lowerBound[2]) <0){
            lowerBound[2] = avgColor[i][2];
        }
        if ((avgColor[i][0] + upperBound[0]) >255) {
            upperBound[0] = 255 - avgColor[i][0];
        }
        if ((avgColor[i][1] + upperBound[1]) >255) {
            upperBound[1] = 255 - avgColor[i][1];
        }
        if ((avgColor[i][2] + upperBound[2]) >255){
            upperBound[2] = 255 - avgColor[i][2];
        }
    }
}

void ImageUtils::produceBinaries(WebcamImage *m) {
    Scalar lower, upper;
    for(int i = 0; i < SAMPLES; i++) {
        normalizeColors();
        lower=Scalar(avgColor[i][0] - lowerBound[0],
                avgColor[i][1] - lowerBound[1], avgColor[i][2] - lowerBound[2]);
        upper=Scalar(avgColor[i][0] + upperBound[0] ,
                avgColor[i][1] + upperBound[1], avgColor[i][2] + upperBound[2]);
        m->bwList.push_back(Mat(m->srcLR.rows, m->srcLR.cols, CV_8U));
        inRange(m->srcLR, lower, upper, m->bwList[i]);
    }
    m->bwList[0].copyTo(m->bw);
    for(int i = 1; i < SAMPLES; i++){
        m->bw += m->bwList[i];
    }
    for (auto mat: m->bwList) {
        mat.release();
    }
    m->bwList.clear();
    medianBlur(m->bw, m->bw, 7);
}


