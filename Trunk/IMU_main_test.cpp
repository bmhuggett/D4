/*	IMU_main_test.cpp
*	Author: George
*	Description: Creates an cImuBoard instance and then prints out
*	Pitch/Yaw/Roll to console at 5Hz
*/

#include "cImuBoard.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <iomanip>

int main()
{
	cImuBoard IMUBOARD = cImuBoard();
	//IMUBOARD.beginLoop();
	IMUBOARD.beginLoop();
	delay(100);
	while(1)
	{
		std::cout<<"P|Y|R\t"<<std::setw(15)<<IMUBOARD.pitch<<"\t"
		<<std::setw(15)<<IMUBOARD.yaw<<"\t"<<std::setw(15)<<IMUBOARD.roll<<"\t"<<std::endl;
		delay(200);

	}
	return 0;
}