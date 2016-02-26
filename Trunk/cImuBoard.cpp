#include "cImuBoard.h"
#include <iostream>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "utils.h"
#include <math.h>
#include <limits>

cImuBoard* pImuPtr;

cImuBoard::cImuBoard()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU constructor"<<std::endl;
	#endif

	imuFd = -1;
	if(setup() < 0)
	{
		std::cout<<"IMU board setup failure"<<std::endl;
	}
	zero();
	pImuPtr = this;
	setAccelRange(RANGE_2);
	setGyroRange(RANGE_250);
}

int cImuBoard::setup()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU setup"<<std::endl;
	#endif

	imuFd = wiringPiI2CSetup(IMU_ADDRESS);
    return imuFd;
}

int cImuBoard::wake()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU wake"<<std::endl;
	#endif

	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd,PWR_MGMT_1)<0)
	{
		std::cout<<"IMU board read failed | PWR_MGMT_1"<<std::endl;
		return -1;
	}
	regval = bitLow(6,regval);
	if(wiringPiI2CWriteReg8(imuFd,PWR_MGMT_1,regval)<0)
    {
        std::cout<<"IMU board write failed | PWR_MGMT_1"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
}

int cImuBoard::accelXRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelXRaw"<<std::endl;
	#endif

	int low;
	int high;
	low 	= wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_L);
	high 	= wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_H);
	if (low<0)
	{
		std::cout << "IMU board read failed | ACCEL_XOUT_L" << std::endl;
		return -1;
	}
	if (high<0)
	{
		std::cout << "IMU board read failed | ACCEL_XOUT_H" << std::endl;
		return -1;
	}
	#ifdef DEBUG_IMU
	std::cout<<"	-Low | High = "<<low<<" | "<<high<<std::endl;
	#endif
	return combineRegSigned(high, low);
}
int cImuBoard::accelYRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelYRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_L);
	high = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_H);

	if(low<0)
	{
		std::cout<<"IMU board read failed | ACCEL_YOUT_L"<<std::endl;
		return -1;
	}
	if(high<0)
	{
		std::cout<<"IMU board read failed | ACCEL_YOUT_H"<<std::endl;
		return -1;
	}

	#ifdef DEBUG_IMU
	std::cout<<"	-Low | High = "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::accelZRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelZRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_L);
	high = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_H);

	if(low <0)
	{
		std::cout<<"IMU board read failed | ACCEL_ZOUT_L"<<std::endl;
		return -1;
	}
	if(high <0)
	{
		std::cout<<"IMU board read failed | ACCEL_ZOUT_H"<<std::endl;
		return -1;
	}

	#ifdef DEBUG_IMU
	std::cout<<"	-Low | High = "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::tempRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU tempRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,TEMP_OUT_L);
	high = wiringPiI2CReadReg8(imuFd,TEMP_OUT_H);

	if(low <0)
	{
		std::cout<<"IMU board read failed | TEMP_OUT_L"<<std::endl;
		return -1;
	}
	if(high <0)
	{
		std::cout<<"IMU board read failed | TEMP_OUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroXRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU gyroXRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_H);

	if(low <0)
	{
		std::cout<<"IMU board read failed | GYRO_XOUT_L"<<std::endl;
		return -1;
	}
	if(high <0)
	{
		std::cout<<"IMU board read failed | GYRO_XOUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroYRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU gyroYRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_H);

	if(low <0)
	{
		std::cout<<"IMU board read failed | GYRO_YOUT_L"<<std::endl;
	}
	if(high <0)
	{
		std::cout<<"IMU board read failed | GYRO_YOUT_H"<<std::endl;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroZRaw()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU gyroZRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_H);

	if(low <0)
	{
		std::cout<<"IMU board read failed | GYRO_ZOUT_L"<<std::endl;
	}
	if(high <0)
	{
		std::cout<<"IMU board read failed | GYRO_ZOUT_H"<<std::endl;
	}

	return combineRegSigned(high,low);
}

float cImuBoard::accelX()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelX"<<std::endl;
	#endif

	float value = (float)accelXRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}
float cImuBoard::accelY()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelY"<<std::endl;
	#endif

	float value = (float)accelYRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}
float cImuBoard::accelZ()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU accelZ"<<std::endl;
	#endif

	float value = (float)accelZRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}

float cImuBoard::temp()
{
	return tempRaw()/340.0 + 36.53;
}
float cImuBoard::gyroX()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU gyroX"<<std::endl;
	#endif

	float value = (float)gyroXRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*gyroRange;
}

int cImuBoard::setAccelRange(eAccelRange range)
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU setAccelRange"<<std::endl;
	#endif

	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd,ACCEL_CONFIG)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_CONFIG"<<std::endl;
		return -1;
	}
	switch (range)
	{
		case RANGE_2:
			regval = bitLow(3,regval);
			regval = bitLow(4,regval);
			accelRange = 2;
			break;
		case RANGE_4:
			regval = bitHigh(3,regval);
			regval = bitLow(4,regval);
			accelRange = 4;
			break;
		case RANGE_8:
			regval = bitLow(3,regval);
			regval = bitHigh(4,regval);
			accelRange = 8;
			break;
		case RANGE_16:
			regval = bitHigh(3,regval);
			regval = bitHigh(4,regval);
			accelRange = 16;
			break;
	}
	if(wiringPiI2CWriteReg8(imuFd,ACCEL_CONFIG,regval)<0)
    {
        std::cout<<"IMU board write failed | ACCEL_CONFIG"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
}

int cImuBoard::setGyroRange(eGyroRange range)
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU setGyroRange"<<std::endl;
	#endif

	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd, GYRO_CONFIG)<0)
	{
		std::cout << "IMU board read failed | GYRO_CONFIG" << std::endl;
		return -1;
	}
	switch (range)
	{
	case RANGE_250:
		regval = bitLow(3, regval);
		regval = bitLow(4, regval);
		gyroRange = 250;
		break;
	case RANGE_500:
		regval = bitHigh(3, regval);
		regval = bitLow(4, regval);
		gyroRange = 500;
		break;
	case RANGE_1000:
		regval = bitLow(3, regval);
		regval = bitHigh(4, regval);
		gyroRange = 1000;
		break;
	case RANGE_2000:
		regval = bitHigh(3, regval);
		regval = bitHigh(4, regval);
		gyroRange = 2000;
		break;
	}
	if (wiringPiI2CWriteReg8(imuFd, GYRO_CONFIG, regval)<0)
	{
		std::cout << "IMU board write failed | GYRO_CONFIG" << std::endl;   //Write zero to start value lower byte
		return -1;
	}
}

void cImuBoard::zero()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU zero"<<std::endl;
	#endif

	pitch = 0;
	yaw = 0;
	roll = 0;
}

int cImuBoard::beginLoop()
{
	#ifdef DEBUG_IMU
	std::cout<<"IMU beginLoop"<<std::endl;
	#endif

	loopTime = micros();
	int x = piThreadCreate(imuLoop);
	if (x != 0)
	{
		std::cout << "Starting up the IMU thread failed" << std::endl;
	}
	return 0;
}

void *imuLoop(void* dummy)
{
	/*
	int lastTime = loopTime;
	loopTime = micros();
	int dt = loopTime - lastTime;
	int accelx, accely, accelz, gyrox, gyroy, gyroz;
	accelx	= pImuPtr->accelX();
	accely	= pImuPtr->accelY();
	accelz	= pImuPtr->accelZ();
	gyrox	= pImuPtr->gyroX();
	gyroy = pImuPtr->gyroY();
	gyroz = pImuPtr->gyroZ();
	*/
}

