#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "hand_detector.hpp"
#include "imageop.hpp"

#define PI 3.1428

using namespace cv;
using namespace std;

HandDetector::HandDetector(WebcamImage* m, ImageOp *op) :
    m(m), op(op) {
	frameNumber=0;
	nrNoFinger=0;
	fontFace = FONT_HERSHEY_PLAIN;
}

DetectorResult HandDetector::updateFrame() {
    frameNumber++;
    m->cap >> m->src;
    flip(m->src,m->src,1);
    pyrDown(m->src, m->srcLR);
    blur(m->srcLR, m->srcLR, Size(3,3));
    cvtColor(m->srcLR, m->srcLR, CV_BGR2HLS);
    op->produceBinaries(m);
    cvtColor(m->srcLR, m->srcLR, CV_HLS2BGR);
    int biggestContour = makeContours(m);
    DetectorResult result = {false, Point()};
    if (biggestContour != -1) {
        result = getResult(contours[biggestContour]);
    }
    getFingerNumber();
    showWindows();
    return result;
}

void HandDetector::showWindows() {
    pyrDown(m->bw,m->bw);
    pyrDown(m->bw,m->bw);
    Rect roi( Point( 3*m->src.cols/4,0 ), m->bw.size());
    vector<Mat> channels;
    Mat result;
    for(int i=0;i<3;i++)
        channels.push_back(m->bw);
    merge(channels,result);
    result.copyTo( m->src(roi));
    imshow("Bubbles",m->src);
}

DetectorResult HandDetector::getResult(vector<Point> contour) {
    Point point = getMaxPoint(contour);
    circle(m->src, point, 5, Scalar(255, 0, 0), 4);
    return {true, point};
}

void HandDetector::initVectors() {
	hullI=vector<vector<int> >(contours.size());
	hullP=vector<vector<Point> >(contours.size());
	defects=vector<vector<Vec4i> > (contours.size());	
}

string HandDetector::bool2string(bool tf){
	if(tf)
		return "true";
	else
		return "false";
}

string HandDetector::intToString(int number){
		stringstream ss;
		ss << number;
		string str = ss.str();
		return str;
}

void HandDetector::printGestureInfo(Mat src) {
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245,200,200);
	int xpos=src.cols/1.5;
	int ypos=src.rows/1.6;
	float fontSize=0.7f;
	int lineChange=14;
	string info= "Figure info:";
	putText(src,info,Point(ypos,xpos),fontFace,fontSize,fColor);
	xpos+=lineChange;
	info=string("Number of defects: ") + string(intToString(nrOfDefects)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
	xpos+=lineChange;
    info=string("bounding box height, width ") + string(intToString(bRect.height)) +
            string(" , ") +  string(intToString(bRect.width)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
	xpos+=lineChange;
	info=string("Is hand: ") + string(bool2string(isHand));
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
}

bool HandDetector::detectIfHand() {
    double h = bRect.height;
    double w = bRect.width;
	isHand=true;
	if(fingerTips.size() > 5 ){
		isHand=false;
	}else if(h==0 || w == 0){
		isHand=false;
	}else if(h/w > 4 || w/h >4){
		isHand=false;	
	}else if(bRect.x<20){
		isHand=false;	
	}	
	return isHand;
}

float HandDetector::distanceP2P(Point a, Point b){
	float d= sqrt(fabs( pow(a.x-b.x,2) + pow(a.y-b.y,2) )) ;  
	return d;
}

Point HandDetector::getMaxPoint(vector<Point> contour) {
    int minY = 1000;
    Point result;
    for (int i = 0; i < contour.size(); i++) {
        Point cur = contour[i];
        if (cur.y < minY) {
            minY = cur.y;
            result = cur;
        }
    }
    return result;
}

// remove fingertips that are too close to 
// eachother
void HandDetector::removeRedundantFingerTips() {
	vector<Point> newFingers;
	for(int i=0;i<fingerTips.size();i++){
		for(int j=i;j<fingerTips.size();j++){
			if(distanceP2P(fingerTips[i],fingerTips[j])<10 && i!=j){
			}else{
				newFingers.push_back(fingerTips[i]);	
				break;
			}	
		}	
	}
	fingerTips.swap(newFingers);
}

void HandDetector::computeFingerNumber(){
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int frequentNr;	
	int thisNumberFreq=1;
	int highestFreq=1;
	frequentNr=fingerNumbers[0];
	for(int i=1;i<fingerNumbers.size(); i++){
		if(fingerNumbers[i-1]!=fingerNumbers[i]){
			if(thisNumberFreq>highestFreq){
				frequentNr=fingerNumbers[i-1];	
				highestFreq=thisNumberFreq;
			}
			thisNumberFreq=0;	
		}
		thisNumberFreq++;	
	}
	if(thisNumberFreq>highestFreq){
		frequentNr=fingerNumbers[fingerNumbers.size()-1];	
	}
	mostFrequentFingerNumber=frequentNr;	
}

void HandDetector::addFingerNumberToVector(){
	int i=fingerTips.size();	
	fingerNumbers.push_back(i);
}

// calculate most frequent numbers of fingers 
// over 20 frames
void HandDetector::getFingerNumber() {
	removeRedundantFingerTips();
    if(bRect.height > m->src.rows/2 && nrNoFinger>12 && isHand ){
		numberColor=Scalar(0,200,0);
		addFingerNumberToVector();
        if(frameNumber > 12) {
			nrNoFinger=0;
			frameNumber=0;	
            computeFingerNumber();
			fingerNumbers.clear();
        } else {
			frameNumber++;
		}
    } else {
		nrNoFinger++;
		numberColor=Scalar(200,200,200);
    }
}

float HandDetector::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f,s);
	float l2 = distanceP2P(f,e);
	float dot=(s.x-f.x)*(e.x-f.x) + (s.y-f.y)*(e.y-f.y);
	float angle = acos(dot/(l1*l2));
	angle=angle*180/PI;
	return angle;
}

void HandDetector::eleminateDefects(WebcamImage* m){
    int tolerance =  bRect.height / 5;
	float angleTol=95;
	vector<Vec4i> newDefects;
	int startidx, endidx, faridx;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
        if(distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance &&
                getAngle(ptStart, ptFar, ptEnd  ) < angleTol) {
			if( ptEnd.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else if( ptStart.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else {
				newDefects.push_back(v);		
			}
		}	
		d++;
	}
	nrOfDefects=newDefects.size();
	defects[cIdx].swap(newDefects);
    removeRedundantEndPoints(defects[cIdx]);
}

// remove endpoint of convexity defects if they are at the same fingertip
void HandDetector::removeRedundantEndPoints(vector<Vec4i> newDefects){
    float tolerance=bRect.width / 6;
    int startidx, endidx;
	int startidx2, endidx2;
    int defectsSize = newDefects.size();
    for(int i = 0; i < defectsSize; i++) {
        for(int j=i; j < defectsSize; j++){
	    	startidx=newDefects[i][0]; Point ptStart(contours[cIdx][startidx] );
	   		endidx=newDefects[i][1]; Point ptEnd(contours[cIdx][endidx] );
	    	startidx2=newDefects[j][0]; Point ptStart2(contours[cIdx][startidx2] );
	   		endidx2=newDefects[j][1]; Point ptEnd2(contours[cIdx][endidx2] );
			if(distanceP2P(ptStart,ptEnd2) < tolerance ){
				contours[cIdx][startidx]=ptEnd2;
				break;
			}if(distanceP2P(ptEnd,ptStart2) < tolerance ){
				contours[cIdx][startidx2]=ptEnd;
			}
		}
	}
}

// convexity defects does not check for one finger
// so another method has to check when there are no
// convexity defects
void HandDetector::checkForOneFinger(WebcamImage *m){
    int yTol= bRect.height / 6;
	Point highestP;
    highestP.y=m->src.rows;
	vector<Point>::iterator d=contours[cIdx].begin();
	while( d!=contours[cIdx].end() ) {
   	    Point v=(*d);
		if(v.y<highestP.y){
			highestP=v;
            //cout<<highestP.y<<endl;
		}
		d++;	
	}int n=0;
	d=hullP[cIdx].begin();
	while( d!=hullP[cIdx].end() ) {
   	    Point v=(*d);
            //cout<<"x " << v.x << " y "<<  v.y << " highestpY " << highestP.y<< "ytol "<<yTol<<endl;
		if(v.y<highestP.y+yTol && v.y!=highestP.y && v.x!=highestP.x){
			n++;
		}
		d++;	
    }
    if(n==0){
		fingerTips.push_back(highestP);
	}
}

void HandDetector::drawFingerTips(WebcamImage *m){
	Point p;
	int k=0;
	for(int i=0;i<fingerTips.size();i++){
		p=fingerTips[i];
		putText(m->src,intToString(i),p-Point(0,30),fontFace, 1.2f,Scalar(200,200,200),2);
   		circle( m->src,p,   5, Scalar(100,255,100), 4 );
   	 }
}

void HandDetector::getFingerTips(WebcamImage *m) {
	fingerTips.clear();
    /*int i=0;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    int startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		int endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    int faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
		if(i==0){
			fingerTips.push_back(ptStart);
			i++;
		}
		fingerTips.push_back(ptEnd);
		d++;
		i++;
    }*/
	if(fingerTips.size()==0){
        checkForOneFinger(m);
	}
}

int HandDetector::makeContours(WebcamImage *m) {
    Mat aBw;
    pyrUp(m->bw,m->bw);
    m->bw.copyTo(aBw);
    findContours(aBw, contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    initVectors();
    cIdx=findBiggestContour();
    if(cIdx!=-1) {
        bRect=boundingRect(Mat(contours[cIdx]));
        convexHull(Mat(contours[cIdx]),hullP[cIdx],false,true);
        convexHull(Mat(contours[cIdx]),hullI[cIdx],false,false);
        approxPolyDP( Mat(hullP[cIdx]), hullP[cIdx], 18, true );
        if(contours[cIdx].size()>3 ){
            convexityDefects(contours[cIdx], hullI[cIdx], defects[cIdx]);
            eleminateDefects(m);
        }
        bool isHand= detectIfHand();
        //printGestureInfo(m->src);
        if(isHand) {
            getFingerTips(m);
            //drawFingerTips(m);
            //myDrawContours(m);
        }
    }
    return cIdx;
}


int HandDetector::findBiggestContour() {
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++){
        if(contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}

void HandDetector::myDrawContours(WebcamImage *m) {
    drawContours(m->src, hullP, cIdx,
                 cv::Scalar(200,0,0), 2, 8, vector<Vec4i>(), 0, Point());
    rectangle(m->src, bRect.tl(), bRect.br(), Scalar(0,0,200));
    vector<Vec4i>::iterator d= defects[cIdx].begin();

    vector<Mat> channels;
    Mat result;
    for(int i=0;i<3;i++)
        channels.push_back(m->bw);
    merge(channels,result);
    drawContours(result, hullP, cIdx, cv::Scalar(0,0,250),
                 10, 8, vector<Vec4i>(), 0, Point());

    while( d!=defects[cIdx].end() ) {
        Vec4i& v=(*d);
        int startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
        int endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
        int faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
        float depth = v[3] / 256;
        circle( result, ptFar,   9, Scalar(0,205,0), 5 );
        d++;
    }
}
