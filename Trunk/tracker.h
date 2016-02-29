/* tracker.h
 * Author: Acura Tang
 * Description: Track object by color with camera
 */
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;


const int CV_MODE_PIN =11;

const int H_MIN = 114;
const int H_MAX = 195;
const int S_MIN = 117;
const int S_MAX = 256;
const int V_MIN = 87;
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
void drawObject(int x, int y,Mat &frame);
void morphOps(Mat &thresh);
float trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
void cvMode(void);
