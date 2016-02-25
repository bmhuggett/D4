#ifndef CIMUBOARD_H
#define CIMUBOARD_H

#define IMU_ADDRESS 	0x68

#define CONFIG 			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define FIFO_EN			0x23
#define ACCEL_XOUT_H 	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L		0x42
#define GYRO_XOUT_H 	0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48

#define USER_CONTROL	0x6A


enum eAccelRange{TWO,FOUR,EIGHT,SIXTEEN};

class cImuBoard
{
public:
	cImuBoard();

	int imuFd;

	int setup();
	int accelX();
	int accelY();
	int accelZ();
	int temp();
	int gyroX();
	int gyroY();
	int gyroZ();
	void zero();
	int beginLoop();
	int setAccelRange(eAccelRange range);

	float pitch;
	float yaw;
	float roll;
};

extern cImuBoard* pImuPtr;

void *imuLoop (void *dummy);


#endif
