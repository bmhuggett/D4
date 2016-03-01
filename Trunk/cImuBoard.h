/*	cImuBoard.h
*	Author: George
*	Description: Defines the cImuBoard class which controls the
*	MPU-6050 IMU board. Also implements a pi thread which
*	constantly processes the accelerometer and gyro inputs
*	to derive pitch/yaw/roll
*/

#ifndef CIMUBOARD_H
#define CIMUBOARD_H



enum eAccelRange{RANGE_2,RANGE_4,RANGE_8,RANGE_16};				//Range enums
enum eGyroRange	{RANGE_250, RANGE_500, RANGE_1000, RANGE_2000};

class cImuBoard
{
public:
	cImuBoard();	//Default constructor

	int imuFd;		//Stores the wiringPiI2C filehandler

	int setup();	//Stars the wiringPiI2C channel, wakes the device
	int wake();		//0's the sleep pin in PWR_MGMT_1, which defaults to 1
	
	int accelXRaw();//Raw value of the measurement registers
	int accelYRaw();
	int accelZRaw();
	int tempRaw();
	int gyroXRaw();
	int gyroYRaw();
	int gyroZRaw();

	int setAccelRange(eAccelRange range);	//Set's the measurement ranges
	int setGyroRange(eGyroRange range);

	float accelMul;		//Stores the multiplier relevant to the current range
	float gyroMul;

	float accelX();//Measurement values turned into physical measurements
	float accelY();
	float accelZ();
	float temp();
	float gyroX();
	float gyroY();
	float gyroZ();

	void zero();	//Zeros the pitch, yaw and roll readings (mostly for yaw)

	int beginLoop();	//Starts the thread for the PYR updating loop
	
	int loopTime;		//Stores the timecount of the last loop execution
	
	float pitch;
	float yaw;
	float roll;
};

extern cImuBoard* pImuPtr;	//Points to the cImuBoard class instance, 
							//	for use by the loop

void *imuLoop (void *dummy);	//Loop function

#endif
