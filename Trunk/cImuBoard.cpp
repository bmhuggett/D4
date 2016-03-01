/*	cImuBoard.cpp
*	Author: George
*	Description: Implements the class described in cImuBoard.h	
*/

#include "cImuBoard.h"
#include <iostream>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "utils.h"
#include <cmath>
#include <limits>

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
#define PWR_MGMT_1		0x6B
#define WHO_AM_I		0x75

//#define IMU_DEBUG //Comment out to disable debug

static cImuBoard* pImuPtr;

cImuBoard::cImuBoard()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Class instantiated"<<std::endl;
	#endif
	imuFd = -1;
	if(setup() < 0)
	{
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Failed to initialise IMU board I2C"<<std::endl;
		#endif
	}
	zero();
	pImuPtr = this;	//Point the cImuBoard pointer here so the loop can use the
					//	internal variables
	setAccelRange(RANGE_2);
	setGyroRange(RANGE_250);
	wake();
}

int cImuBoard::setup()
{

	imuFd = wiringPiI2CSetup(IMU_ADDRESS);
    return imuFd;
}

int cImuBoard::wake()
{

	//Write 0 to the Sleep bit in PWR_MGMT_1

	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd,PWR_MGMT_1)<0)
	{
		std::cout<<"IMU | Failed to read sleep register (PWR_MGMT_1)"<<std::endl;
		return -1;
	}
	regval = bitLow(6,regval);
	if(wiringPiI2CWriteReg8(imuFd,PWR_MGMT_1,regval)<0)
    {
        std::cout<<"IMU | Failed to write to sleep register (PWR_MGMT_1)"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
}

int cImuBoard::accelXRaw()
{
	int low;
	int high;
	low 	= wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_L);
	high 	= wiringPiI2CReadReg8(imuFd, ACCEL_XOUT_H);
	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_XOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_XOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Accel X Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif
	return combineRegSigned(high, low);
}
int cImuBoard::accelYRaw()
{
	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_L);
	high = wiringPiI2CReadReg8(imuFd,ACCEL_YOUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_YOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_YOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Accel Y Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::accelZRaw()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU accelZRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_L);
	high = wiringPiI2CReadReg8(imuFd,ACCEL_ZOUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_ZOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (ACCEL_ZOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Accel Z Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::tempRaw()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU tempRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,TEMP_OUT_L);
	high = wiringPiI2CReadReg8(imuFd,TEMP_OUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (TEMP_OUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (TEMP_OUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Temp Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::gyroXRaw()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU gyroXRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_XOUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_XOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_XOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Gyro X Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::gyroYRaw()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU gyroYRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_YOUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_YOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_YOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Gyro Y Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}
int cImuBoard::gyroZRaw()
{
	#ifdef IMU_DEBUG
	std::cout<<"IMU gyroZRaw"<<std::endl;
	#endif

	int low;
	int high;
	low = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_L);
	high = wiringPiI2CReadReg8(imuFd,GYRO_ZOUT_H);

	if (low<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_ZOUT_L)" << std::endl;
		#endif
		return -1;
	}
	if (high<0)
	{
		#ifdef IMU_DEBUG
		std::cout << "IMU | Failed to read (GYRO_ZOUT_H)" << std::endl;
		#endif
		return -1;
	}
	#ifdef IMU_DEBUG
	std::cout<<"IMU | Gyro Z Raw Data (Low | High)\t "<<low<<" | "<<high<<std::endl;
	#endif

	return combineRegSigned(high,low);
}

float cImuBoard::accelX()
{
	float value = (float)accelXRaw() / accelMul;
	return value;
}

float cImuBoard::accelY()
{
	float value = (float)accelYRaw() / accelMul;
	return value;
}

float cImuBoard::accelZ()
{
	float value = (float)accelZRaw() / accelMul;
	return value;
}

float cImuBoard::temp()
{
	return tempRaw()/340.0 + 36.53;
}

float cImuBoard::gyroX()
{
	float value = (float)gyroXRaw() / gyroMul;
	return value;
}

float cImuBoard::gyroY()
{
	float value = (float)gyroYRaw() / gyroMul;
	return value;
}

float cImuBoard::gyroZ()
{
	float value = (float)gyroZRaw() / gyroMul;
	return value;
}

int cImuBoard::setAccelRange(eAccelRange range)
{
	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd,ACCEL_CONFIG)<0)
	{
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Failed to read (ACCEL_CONFIG)"<<std::endl;
		#endif
		return -1;
	}

	//Translates ranges into bit patterns
	switch (range)
	{
		case RANGE_2:
			#ifdef IMU_DEBUG
			std::cout<<"IMU | Setting Accelerometer range to 2g"<<std::endl;
			#endif
			regval = bitLow(3,regval);
			regval = bitLow(4,regval);
			accelMul = 16384;
			break;
		case RANGE_4:
			#ifdef IMU_DEBUG
			std::cout<<"IMU | Setting Accelerometer range to 4g"<<std::endl;
			#endif
			regval = bitHigh(3,regval);
			regval = bitLow(4,regval);
			accelMul = 8192;
			break;
		case RANGE_8:
			#ifdef IMU_DEBUG
			std::cout<<"IMU | Setting Accelerometer range to 8g"<<std::endl;
			#endif
			regval = bitLow(3,regval);
			regval = bitHigh(4,regval);
			accelMul = 4096;
			break;
		case RANGE_16:
			#ifdef IMU_DEBUG
			std::cout<<"IMU | Setting Accelerometer range to 16g"<<std::endl;
			#endif
			regval = bitHigh(3,regval);
			regval = bitHigh(4,regval);
			accelMul = 2048;
			break;
	}
	if(wiringPiI2CWriteReg8(imuFd,ACCEL_CONFIG,regval)<0)
    {
    	#ifdef IMU_DEMUG
        std::cout<<"IMU | Write failed (ACCEL_CONFIG)"<<std::endl;   //Write zero to start value lower byte
        #endif
        return -1;
    }
}

int cImuBoard::setGyroRange(eGyroRange range)
{
	int regval;
	if (regval = wiringPiI2CReadReg8(imuFd, GYRO_CONFIG)<0)
	{
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Failed to read (GYRO_CONFIG)"<<std::endl;
		#endif
		return -1;
	}

	//Translates ranges into bit patterns
	switch (range)
	{
	case RANGE_250:
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Gyro Range set to 250"<<std::endl;
		#endif
		regval = bitLow(3, regval);
		regval = bitLow(4, regval);
		gyroMul = 131;
		break;
	case RANGE_500:
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Gyro Range set to 500"<<std::endl;
		#endif
		regval = bitHigh(3, regval);
		regval = bitLow(4, regval);
		gyroMul = 65.5;
		break;
	case RANGE_1000:
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Gyro Range set to 1000"<<std::endl;
		#endif
		regval = bitLow(3, regval);
		regval = bitHigh(4, regval);
		gyroMul = 32.8;
		break;
	case RANGE_2000:
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Gyro Range set to 2000"<<std::endl;
		#endif
		regval = bitHigh(3, regval);
		regval = bitHigh(4, regval);
		gyroMul = 16.4;
		break;
	}
	if (wiringPiI2CWriteReg8(imuFd, GYRO_CONFIG, regval)<0)
	{
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Failed to write (GYRO_CONFIG)"<<std::endl;
		#endif
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
	#ifdef IMU_DEBUG
	std::cout<<"IMU | IMU loop start called"<<std::endl;
	#endif
	loopTime = micros();
	int x = piThreadCreate(imuLoop);
	if (x != 0)
	{
		#ifdef IMU_DEBUG
		std::cout<<"IMU | Failed to start the IMU loop thread"<<std::endl;
		#endif
	}
	return 0;
}

void *imuLoop(void* dummy)
{

	// The technique for the complementary filter design in this
	// loop is from Pieter-Jan Van de Maele, of Pieter-Jan.com.
	// His primer on this topic can be found at:
	//
	//	http://www.pieter-jan.com/node/11
	//
	// Details are different because of the different platform,
	// but the technique is identical.

	pImuPtr->zero();
	while(1)
	{
		float pitchlocal 	= 	pImuPtr->pitch;	//about x-axis
		float rolllocal 	= 	pImuPtr->roll;	//about y-axis
		float yawlocal		=	pImuPtr->yaw;	//about z-axis

		int lastTime = pImuPtr->loopTime;
		pImuPtr->loopTime= micros();
		float dt = (float)(pImuPtr->loopTime - lastTime)*0.000001;

		//Load in readings
		float accelx, accely, accelz, gyrox, gyroy, gyroz;
		accelx	= pImuPtr->accelX();
		accely	= pImuPtr->accelY();
		accelz	= pImuPtr->accelZ();
		gyrox	= pImuPtr->gyroX();
		gyroy = pImuPtr->gyroY();
		gyroz = pImuPtr->gyroZ();

		//Integrate dtheta/dt readings from gyro
		pitchlocal 	+= gyrox*dt;
		rolllocal 	+= gyroy*dt;
		yawlocal	+= gyroz*dt;

		//Make sure the force on the accelerometer is reasonably low, so the
		//	gravity vector dominates
		float forcemag = std::abs(accelx) + std::abs(accely) + std::abs(accelz);
		if ((forcemag > 0.5) && (forcemag < 2))
		{
			//Derive P/R from accelerometer values, and convert to degrees
			float pitchacc 	= atan2(accely,accelz) *180.0/M_PI;
			float rollacc	= atan2(accelx,accelz) *180.0/M_PI;

			//Filter to incline towards the accelerometer value over time
			pitchlocal 	= 0.98*pitchlocal + 0.02*pitchacc;
			rolllocal	= 0.98*rolllocal + 0.02*rollacc;
		}
		//Push back PYR values
		pImuPtr->pitch 	= pitchlocal;
		pImuPtr->roll 	= rolllocal;
		pImuPtr->yaw 	= yawlocal;

	}

}

