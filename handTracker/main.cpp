#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "hand.h"


////default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
////max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
////minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

///Calibrate TrackBar var
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

void calibrate(){
    const std::string trackbarWindowName = "Calibration";
    //Create TrackBars
    cv::namedWindow(trackbarWindowName,0);
    cv::createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX);
    cv::createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX );
    cv::createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX );
    cv::createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX );
    cv::createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX );
    cv::createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX );
}

void morphOps(cv::Mat &thresh){
    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(8,8));

    cv::erode(thresh,thresh,erodeElement);
    cv::erode(thresh,thresh,erodeElement);

    cv::dilate(thresh,thresh,dilateElement);
    cv::dilate(thresh,thresh,dilateElement);
}


void drawObject(int x, int y,cv::Mat &frame){

    cv::circle(frame,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
    if(y-25>0)
    cv::line(frame,cv::Point(x,y),cv::Point(x,y-25),cv::Scalar(0,255,0),2);
    else line(frame,cv::Point(x,y),cv::Point(x,0),cv::Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    cv::line(frame,cv::Point(x,y),cv::Point(x,y+25),cv::Scalar(0,255,0),2);
    else cv::line(frame,cv::Point(x,y),cv::Point(x,FRAME_HEIGHT),cv::Scalar(0,255,0),2);
    if(x-25>0)
    cv::line(frame,cv::Point(x,y),cv::Point(x-25,y),cv::Scalar(0,255,0),2);
    else cv::line(frame,cv::Point(x,y),cv::Point(0,y),cv::Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    cv::line(frame,cv::Point(x,y),cv::Point(x+25,y),cv::Scalar(0,255,0),2);
    else cv::line(frame,cv::Point(x,y),cv::Point(FRAME_WIDTH,y),cv::Scalar(0,255,0),2);

    cv::putText(frame,"Hand",cv::Point(x+20,y+40),1,1,cv::Scalar(0,255,0),1);

}
void trackObj(int &x, int &y, cv::Mat thresh, cv::Mat &cam){
    cv::Mat temp;
    thresh.copyTo(temp);

    //these two vectors needed for output of findContours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //find contours of filtered image using openCV findContours function
    cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects < MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                cv::Moments moment = cv::moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;
                    refArea = area;
                }else objectFound = false;
            }
            //let user know you found an object
            if(objectFound ==true)
                //draw object location on screen
                drawObject(x,y,cam);

        }else
            cv::putText(cam,"TOO MUCH NOISE! ADJUST FILTER",cv::Point(0,50),1,2,cv::Scalar(0,0,255),1);
    }
}

int main(){
    //Pink Gloves
    Hand hand(cv::Scalar(142,94,32), cv::Scalar(191,196,126));

    bool calib = false;
    //Calibrate hand
    if (calib)
        calibrate();

    cv::VideoCapture capture;
    cv::Mat cameraFeed;
    cv::Mat HSVimg;
    cv::Mat thresholdImg;

    capture.open(0);

    ///Set frame size
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    while(true){
        capture.read(cameraFeed);
        if(calib){
            hand.setHSVmax(cv::Scalar(H_MAX,S_MAX,V_MAX));
            hand.setHSVmin(cv::Scalar(H_MIN,S_MIN,V_MIN));
        }

        ///HSV image
        cv::cvtColor(cameraFeed, HSVimg, cv::COLOR_BGR2HSV);

        ///Threshold Image
        cv::inRange(HSVimg,hand.getHSVmin(), hand.getHSVmax(), thresholdImg);
        morphOps(thresholdImg);
        int x = 0, y = 0;
        trackObj(x,y,thresholdImg,cameraFeed);

        ///Show Frames
        //cv::imshow("HSV", HSVimg);
        //cv::imshow("Threshold", thresholdImg);
        cv::imshow("Cam",cameraFeed);
        cv::waitKey(30);
    }
}
