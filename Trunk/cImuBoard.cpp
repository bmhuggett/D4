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
	imuFd = -1;
	if(setup() < 0)
	{
		std::cout<<"IMU board setup failure"<<std::endl;
	}
	zero();
	pImuPtr = this;
	setAccelRange(TWO);
}

int cImuBoard::setup()
{
	imuFd = wiringPiI2CSetup(IMU_ADDRESS);
    return imuFd;
}

int cImuBoard::accelXRaw()
{
	int low;
	int high;

	if (low = wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_L)<0)
	{
		std::cout << "IMU board read failed | ACCEL_XOUT_L" << std::endl;
		return -1;
	}
	if (high = wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_H)<0)
	{
		std::cout << "IMU board read failed | ACCEL_XOUT_H" << std::endl;
		return -1;
	}

	return combineRegSigned(high, low);
}
int cImuBoard::accelYRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_L)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_YOUT_L"<<std::endl;
		return -1;
	}
	if(high = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_H)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_YOUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::accelZRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_L)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_ZOUT_L"<<std::endl;
		return -1;
	}
	if(high = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_H)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_ZOUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::tempRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,TEMP_OUT_L)<0)
	{
		std::cout<<"IMU board read failed | TEMP_OUT_L"<<std::endl;
		return -1;
	}
	if(high = wiringPiI2CReadReg8(imuFd,TEMP_OUT_H)<0)
	{
		std::cout<<"IMU board read failed | TEMP_OUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroXRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_L)<0)
	{
		std::cout<<"IMU board read failed | GYRO_XOUT_L"<<std::endl;
		return -1;
	}
	if(high = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_H)<0)
	{
		std::cout<<"IMU board read failed | GYRO_XOUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroYRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_L)<0)
	{
		std::cout<<"IMU board read failed | GYRO_YOUT_L"<<std::endl;
	}
	if(high = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_H)<0)
	{
		std::cout<<"IMU board read failed | GYRO_YOUT_H"<<std::endl;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::gyroZRaw()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_L)<0)
	{
		std::cout<<"IMU board read failed | GYRO_ZOUT_L"<<std::endl;
	}
	if(high = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_H)<0)
	{
		std::cout<<"IMU board read failed | GYRO_ZOUT_H"<<std::endl;
	}

	return combineRegSigned(high,low);
}

float cImuBoard::accelX()
{
	float value = (float)accelXRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}
float cImuBoard::accelY()
{
	float value = (float)accelYRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}
float cImuBoard::accelY()
{
	float value = (float)accelYRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*accelRange;
}
float cImuBoard::gyroX()
{
	float value = (float)gyroXRaw() / (float)std::numeric_limits<int>::max(); //Get fraction through int range
	return value*gyroRange;
}

int cImuBoard::setAccelRange(eAccelRange range)
{
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
	pitch = 0;
	yaw = 0;
	roll = 0;
}

int cImuBoard::beginLoop()
{
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



}

