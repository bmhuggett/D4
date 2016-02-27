
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int H_MIN = 114;
int H_MAX = 195;
int S_MIN = 117;
int S_MAX = 256;
int V_MIN = 87;
int V_MAX = 256;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window


string intToString(int number);
void drawObject(int x, int y,Mat &frame);
void morphOps(Mat &thresh);
float trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);