#include "imageop.hpp"

ImageOp::ImageOp() {
    square_len = 20;
    iSinceKFInit = 0;
}

// change a color from one space to another
void ImageOp::col2origCol(int hsv[3], int bgr[3], Mat src) {
    Mat avgBGRMat=src.clone();
    for(int i=0;i<3;i++){
        avgBGRMat.data[i]=hsv[i];
    }
    cvtColor(avgBGRMat,avgBGRMat,CV_BGR2HLS);
    for(int i=0;i<3;i++){
        bgr[i]=avgBGRMat.data[i];
    }
}

void printText(Mat src, string text){
    int fontFace = FONT_HERSHEY_PLAIN;
    putText(src,text,Point(src.cols/2, src.rows/10),fontFace, 1.2f,Scalar(200,0,0),2);
}

void ImageOp::calculatePalmColor(WebcamImage* m) {
    m->cap >> m->src;
    flip(m->src,m->src,1);
    roi.push_back(BorderedRect(Point(m->src.cols/3, m->src.rows/6),Point(m->src.cols/3+square_len, m->src.rows/6+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/4, m->src.rows/2),Point(m->src.cols/4+square_len, m->src.rows/2+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/3, m->src.rows/1.5),Point(m->src.cols/3+square_len, m->src.rows/1.5+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/2, m->src.rows/2),Point(m->src.cols/2+square_len, m->src.rows/2+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/2.5, m->src.rows/2.5),Point(m->src.cols/2.5+square_len, m->src.rows/2.5+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/2, m->src.rows/1.5),Point(m->src.cols/2+square_len, m->src.rows/1.5+square_len), m->src));
    roi.push_back(BorderedRect(Point(m->src.cols/2.5, m->src.rows/1.8),Point(m->src.cols/2.5+square_len, m->src.rows/1.8+square_len), m->src));

    for(int i = 0;i < 50;i++) {
        m->cap >> m->src;
        flip(m->src,m->src,1);
        for(int j=0;j<NSAMPLES;j++){
            roi[j].draw_rectangle(m->src);
        }
        string imgText=string("Cover rectangles with palm");
        printText(m->src,imgText);

        if(i==30){
            //	imwrite("./images/waitforpalm1.jpg",m->src);
        }

        imshow("Bubbles", m->src);
        //*out << m->src;
        if(cv::waitKey(1) >= 0) break;
    }
    computeAverage(m);
    initTrackbars();
}

int ImageOp::getMedian(vector<int> val) {
    int median;
    size_t size = val.size();
    sort(val.begin(), val.end());
    if (size  % 2 == 0)  {
        median = val[size / 2 - 1] ;
    } else{
        median = val[size / 2];
    }
    return median;
}

void ImageOp::getAvgColor(WebcamImage *m, BorderedRect roi,int avg[3]){
    Mat r;
    roi.roi_ptr.copyTo(r);
    vector<int>hm;
    vector<int>sm;
    vector<int>lm;
    // generate vectors
    for(int i=2; i<r.rows-2; i++){
        for(int j=2; j<r.cols-2; j++){
            hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]) ;
            sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]) ;
            lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]) ;
        }
    }
    avg[0]=getMedian(hm);
    avg[1]=getMedian(sm);
    avg[2]=getMedian(lm);
}

void ImageOp::computeAverage(WebcamImage *m) {
    for(int i=0;i<30;i++){
        m->cap >> m->src;
        flip(m->src,m->src,1);
        cvtColor(m->src,m->src, CV_BGR2HLS);
        for(int j=0;j<NSAMPLES;j++){
            getAvgColor(m,roi[j],avgColor[j]);
            roi[j].draw_rectangle(m->src);
        }
        cvtColor(m->src,m->src, CV_HLS2BGR);
        string imgText=string("Finding average color of hand");
        printText(m->src,imgText);
        imshow("Bubbles", m->src);
        if(cv::waitKey(30) >= 0) {
            break;
        }
    }
}

void ImageOp::initTrackbars() {
    namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    c_lower[0][0]=12;
    c_upper[0][0]=7;
    c_lower[0][1]=30;
    c_upper[0][1]=40;
    c_lower[0][2]=80;
    c_upper[0][2]=80;
    createTrackbar("lower1","trackbars",&c_lower[0][0],255);
    createTrackbar("lower2","trackbars",&c_lower[0][1],255);
    createTrackbar("lower3","trackbars",&c_lower[0][2],255);
    createTrackbar("upper1","trackbars",&c_upper[0][0],255);
    createTrackbar("upper2","trackbars",&c_upper[0][1],255);
    createTrackbar("upper3","trackbars",&c_upper[0][2],255);
}

void ImageOp::normalizeColors() {
    // copy all boundries read from trackbar
    // to all of the different boundries
    for(int i=1;i<NSAMPLES;i++){
        for(int j=0;j<3;j++){
            c_lower[i][j]=c_lower[0][j];
            c_upper[i][j]=c_upper[0][j];
        }
    }
    // normalize all boundries so that
    // threshold is whithin 0-255
    for(int i=0;i<NSAMPLES;i++){
        if((avgColor[i][0]-c_lower[i][0]) <0){
            c_lower[i][0] = avgColor[i][0] ;
        }
        if((avgColor[i][1]-c_lower[i][1]) <0){
            c_lower[i][1] = avgColor[i][1] ;
        }
        if((avgColor[i][2]-c_lower[i][2]) <0){
            c_lower[i][2] = avgColor[i][2] ;
        }
        if((avgColor[i][0]+c_upper[i][0]) >255){
            c_upper[i][0] = 255-avgColor[i][0] ;
        }
        if((avgColor[i][1]+c_upper[i][1]) >255){
            c_upper[i][1] = 255-avgColor[i][1] ;
        }
        if((avgColor[i][2]+c_upper[i][2]) >255){
            c_upper[i][2] = 255-avgColor[i][2] ;
        }
    }
}

void ImageOp::produceBinaries(WebcamImage *m) {
    Scalar lowerBound;
    Scalar upperBound;
    for(int i = 0; i < NSAMPLES; i++) {
        normalizeColors();
        lowerBound=Scalar(avgColor[i][0] - c_lower[i][0] ,
                avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2] );
        upperBound=Scalar( avgColor[i][0] + c_upper[i][0] ,
                avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2] );
        m->bwList.push_back(Mat(m->srcLR.rows,m->srcLR.cols,CV_8U));
        inRange(m->srcLR,lowerBound,upperBound,m->bwList[i]);
    }
    m->bwList[0].copyTo(m->bw);
    for(int i=1;i<NSAMPLES;i++){
        m->bw+=m->bwList[i];
    }
    medianBlur(m->bw, m->bw,7);
}


