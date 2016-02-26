#include "cImuBoard.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

int main()
{
	cImuBoard IMUBOARD = cImuBoard();
	//IMUBOARD.beginLoop();
	IMUBOARD.wake();
	std::cout<<"Exited wake"<<std::endl;
	IMUBOARD.beginLoop();
	delay(100);
	while(1)
	{
		std::cout<<"Pitch is "<<IMUBOARD.pitch<<std::endl;
		//std::cout<<"WHO_AM_I gives "<<wiringPiI2CReadReg8(IMUBOARD.imuFd,0x75)<<std::endl;
		//std::cout<<"gyroXRaw gives "<<IMUBOARD.gyroXRaw()<<std::endl;
		//std::cout<<"gyroX gives "<<IMUBOARD.gyroX()<<std::endl;
		//std::cout<<"temp gives "<<IMUBOARD.temp()<<std::endl;
		//std::cout<<"gyroX gives "<<IMUBOARD.gyroX()<<std::endl;
		delay(200);

	}
	return 0;
}