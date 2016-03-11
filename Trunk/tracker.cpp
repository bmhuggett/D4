/* tracker.cpp
 * Author: Acura Tang
 * Description: Track object by color with camare
 * Reference: modifcation of Kyle Hounslow's objectTrackingTutorial.cpp
 * https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
 */
#include "tracker.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <wiringPi.h>

using namespace cv;
using namespace std;

const int CV_MODE_PIN =11;

//HSV in Lab

const int H_MIN = 0;
const int H_MAX = 96;
const int S_MIN = 130;
const int S_MAX = 214;
const int V_MIN = 177;
const int V_MAX = 256;

/*
//HSV in Halls
const int H_MIN = 144;
const int H_MAX = 195;
const int S_MIN = 117;
const int S_MAX = 256;
const int V_MIN = 87;
const int V_MAX = 256;
*/

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window


string tracker::intToString(int number)
{


    std::stringstream ss;
    ss << number;
    return ss.str();
}

void tracker::drawObject(int x, int y,Mat &frame)
{
    //use some of the openCV drawing functions to draw crosshairs
    //on your tracked image!

    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

    putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
}
void tracker::morphOps(Mat &thresh)
{

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    //erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    //dilate(thresh,thresh,dilateElement);




}

int tracker::trackFilteredObject(int &x, int &y,float &radius, Mat threshold, Mat &cameraFeed)
{

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    int mainObject =0;
    Point2f centerOfEnclosingCircle;
    if (hierarchy.size() > 0)
    {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0])
            {
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                //diamitor = sqrt(area/3.14);

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
                {
                    mainObject = index;
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;
                    refArea = area;
                }else objectFound = false;
            }
            //let user know you found an object
            if(objectFound ==true)
            {
                minEnclosingCircle(contours[mainObject],centerOfEnclosingCircle,radius);
                putText(cameraFeed,"Found Object",Point(0,50),2,1,Scalar(0,255,255),2);

                //drawContours(cameraFeed,contours,mainObject,Scalar(0,0,0),3);//drowing contours
                circle(cameraFeed,centerOfEnclosingCircle,radius,Scalar(0,0,0));//finding the minimum encloseing circle of the object
                drawObject(x,y,cameraFeed);//draw object location on screen

                cout<<"radius: "<<radius<<endl;
                return 1;
            }

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
    return -1;
}

char tracker::cvMode(void)
{
//some boolean variables for different functionality within this
//program

//Matrix to store each frame of the webcam feed
Mat cameraFeed;
//matrix storage for HSV image
//Mat HSV;
//matrix storage for binary threshold image
Mat threshold;
//x and y values for the location of the object
int x=0, y=0;
float radius;
//create slider bars for HSV filtering
//video capture object to acquire webcam feed
VideoCapture capture;
//open capture object at location zero (default location for webcam)
capture.open(0);
if(!capture.isOpened())  return 0;

//set height and width of capture frame
capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
//start an infinite loop where webcam feed is copied to cameraFeed matrix
//all of our operations will be performed within this loop

cout<<"Prass 'q' To Quit"<<endl;
while(waitKey(30) != 'q')//change this to digitalRead(CV_MODE_PIN) later
    {
    //store image to matrix
    capture.read(cameraFeed);
    //convert frame from BGR to HSV colorspace
     cvtColor(cameraFeed,threshold,COLOR_BGR2HSV);
     //filter HSV image between values and store filtered image to
     //threshold matrix
     inRange(threshold,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
     //perform morphological operations on thresholded image to eliminate noise
     //and emphasize the filtered object(s)
     morphOps(threshold);
     //pass in thresholded frame to our object tracking function
     //this function will return the x and y coordinates of the
     //filtered object
      if(trackFilteredObject(x,y,radius,threshold,cameraFeed)==1)
      {
          if(x<213 && x>0)//Turn Left condition
              {

                cout<<"Trun Left"<<endl;

                //return 'l';
              }
         if(x>427 && x<640)//Turn Right condition
              {
                cout<<"Turn Right"<<endl;

                //return 'r';
              }

         if(!((x-radius)<0 ||(x+radius)>640 ||(y-radius)<0 || (y+radius)>480))//not partialy out of frame condition
            {
                cout<<"NOT OUT OF FRAME"<<endl;
                /*
                 if(radius<     )
                 {
                   return 'f'
                 }
                 else if(radius >      )
                 {
                    return 'b'
                 }
                 */
            }
         return 1;
      }
      //show frames
      imshow("Tracker",cameraFeed);//for debuging
      imshow("Binary",threshold);//for debuging
    }
return 0;
}
