#include "cImuBoard.h"
#include <iostream>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "utils.h"
#include <math.h>

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
}

int cImuBoard::setup()
{
	imuFd = wiringPiI2CSetup(IMU_ADDRESS);
    return imuFd;
}

int cImuBoard::accelX()
{
	int low;
	int high;

	if(low = wiringPiI2CReadReg8(imuFd,ACCEL_XOUT_L)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_XOUT_L"<<std::endl;
		return -1;
	}
	if(high = wiringPiI2CReadReg8(imuFd,ACCEL_XOUT_H)<0)
	{
		std::cout<<"IMU board read failed | ACCEL_XOUT_H"<<std::endl;
		return -1;
	}

	return combineRegSigned(high,low);
}
int cImuBoard::accelY()
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
int cImuBoard::accelZ()
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
int cImuBoard::temp()
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
int cImuBoard::gyroX()
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
int cImuBoard::gyroY()
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
int cImuBoard::gyroZ()
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
		case TWO:
			regval = bitLow(3,regval);
			regval = bitLow(4,regval);
			break;
		case FOUR:
			regval = bitHigh(3,regval);
			regval = bitLow(4,regval);
			break;
		case EIGHT:
			regval = bitLow(3,regval);
			regval = bitHigh(4,regval);
			break;
		case SIXTEEN:
			regval = bitHigh(3,regval);
			regval = bitHigh(4,regval);
			break;
	}
	if(wiringPiI2CWriteReg8(imuFd,ACCEL_CONFIG,regval)<0)
    {
        std::cout<<"IMU board write failed | ACCEL_CONFIG"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
}

void cImuBoard::zero()
{
	pitch = 0;
	yaw = 0;
	roll = 0;
}

void *imuLoop(void* dummy)
{
	while(1)
	{
		pImuPtr->pitch++;

		std::cout<< pImuPtr->pitch <<std::endl;
		delay(1000);
	}
}

int cImuBoard::beginLoop()
{
	int x = piThreadCreate (imuLoop);
	if (x!=0)
	{
		std::cout<<"Starting up the IMU thread failed"<<std::endl;
	}
	return 0;
}