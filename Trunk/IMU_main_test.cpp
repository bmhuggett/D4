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
	delay(100);
	while(1)
	{
		//std::cout<<"WHO_AM_I gives "<<wiringPiI2CReadReg8(IMUBOARD.imuFd,0x75)<<std::endl;
		//std::cout<<"gyroXRaw gives "<<IMUBOARD.gyroXRaw()<<std::endl;
		//std::cout<<"gyroX gives "<<IMUBOARD.gyroX()<<std::endl;
		std::cout<<"temp gives "<<IMUBOARD.temp()<<std::endl;
		delay(200);
		continue;
		float pos = 0;
		int i;
		for(i=0;i<10;i++)
		{
			pos+=IMUBOARD.accelXRaw();
			delay(10);
		}
		pos = pos/10;
		std::cout<<pos<<std::endl;
		delay(400);
		
	}
	return 0;
}