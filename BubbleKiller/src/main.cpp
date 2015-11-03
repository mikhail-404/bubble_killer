#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <QCoreApplication>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>

#include "webcam_image.hpp"
#include "rect.hpp"
#include "hand_detector.hpp"
#include "imageop.hpp"

using namespace cv;
using namespace std;

VideoWriter out;

void doMain() {
    WebcamImage m(0);
    ImageOp op;
    HandDetector detector(&m, &op);
    //out.open("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, m.src.size(), true);
    namedWindow("Bubbles", CV_WINDOW_FULLSCREEN);
    op.calculatePalmColor(&m);
    for(;;) {
        detector.updateFrame();
        DetectorResult result = detector.getResult();
        cout << result.recognized << " " << result.numFingers <<
             result.pos << endl;

        //out << m.src;
        //imwrite("./images/final_result.jpg",m.src);
        if (cv::waitKey(30) == char('q')) {
            break;
        }
    }
    destroyAllWindows();
    //out.release();
    m.cap.release();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    doMain();
    return a.exec();
}
