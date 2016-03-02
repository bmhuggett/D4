/* tracker.h
 * Author: Acura Tang
 * Description: Track object by color with camare
 * Reference: modifcation of Kyle Hounslow's objectTrackingTutorial.cpp
 * https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
 */

#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/core/core.hpp>


using namespace cv;


const int CV_MODE_PIN =11;

const int H_MIN = 0;
const int H_MAX = 96;
const int S_MIN = 130;
const int S_MAX = 214;
const int V_MIN = 177;
const int V_MAX = 256;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window


string intToString(int number);
void drawObject(int x, int y,Mat &frame);//need I say more?
void morphOps(Mat &thresh);///morphological operation to reduse noise from image
float trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);//find objects from binary image, retunes the radius of the minimum enclosing circle and wirte the coordinates into &x and &y
void cvMode(void);//call this function to enter computer vision mode

#endif