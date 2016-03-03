/* tracker.h
 * Author: Acura Tang
 * Description: Track object by color with camare
 * Reference: modifcation of Kyle Hounslow's objectTrackingTutorial.cpp
 * https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
 */
#include <opencv2/core/core.hpp>


using namespace cv;



class tracker
{
public:


    string intToString(int number);
    void drawObject(int x, int y,Mat &frame);//need I say more?
    void morphOps(Mat &thresh);///morphological operation to reduse noise from image
    int trackFilteredObject(int &x, int &y,float &radius, Mat threshold, Mat &cameraFeed);//find objects from binary image, retunes the radius of the minimum enclosing circle and wirte the coordinates into &x and &y
    void cvMode(void);//call this function to enter computer vision mode
};
