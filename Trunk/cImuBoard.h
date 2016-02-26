#ifndef CIMUBOARD_H
#define CIMUBOARD_H


//#define DEBUG_IMU

enum eAccelRange{RANGE_2,RANGE_4,RANGE_8,RANGE_16};
enum eGyroRange	{RANGE_250, RANGE_500, RANGE_1000, RANGE_2000};

class cImuBoard
{
public:
	cImuBoard();

	int imuFd;

	int setup();
	int wake();
	
	int accelXRaw();
	int accelYRaw();
	int accelZRaw();
	int tempRaw();
	int gyroXRaw();
	int gyroYRaw();
	int gyroZRaw();

	float accelX();
	float accelY();
	float accelZ();
	float temp();
	float gyroX();
	float gyroY();
	float gyroZ();

	void zero();
	int beginLoop();
	int setAccelRange(eAccelRange range);
	int setGyroRange(eGyroRange range);


	int loopTime;
	float accelMul;
	float gyroMul;

	float pitch;
	float yaw;
	float roll;
};

extern cImuBoard* pImuPtr;

void *imuLoop (void *dummy);

#endif
