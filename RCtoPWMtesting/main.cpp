#include "cRCreceiver.h"
#include "cPwmBoard.h"
#include <iostream>
#include <wiringPi.h>

int main(void)
{
	wiringPiSetup();

	cRCreceiver my_RC;
	std::pair<int, int> vel(0,0);
	int rot = 0;

	cPWMBoard my_PWM;

	while(1)
	{
		delay(500);

		vel = my_RC.getInputMovementSpeed();
		rot = my_RC.getInputRotationSpeed();

		
	}
}