#include "cImuBoard.h"
#include <wiringPi.h>
#include <iostream>

int main()
{
	cImuBoard IMUBOARD = cImuBoard();
	IMUBOARD.beginLoop();
	while(1)
	{
		delay(1000);
		std::cout<<"I just checked pitch! It's "<<IMUBOARD.pitch<<std::endl;
	}
	return 0;
}