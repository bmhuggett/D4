#include "cRCreceiver.h"
#include <iostream>
#include <wiringPi.h>

int main(void)
{
	wiringPiSetup();
	cRCreceiver my_RC;
	std::pair<int, int> vel(0,0);
	int rot = 0;

	while(1)
	{
		delay(500);

		vel = my_RC.getInputMovementSpeed();
		rot = my_RC.getInputRotationSpeed();

		std::cout << "X, Y velocity vector is (" << vel.first/10 << ", " << vel.second/10 << ").             Rotational velocity is " << rot/10 << ". " << std::endl;
	}
}