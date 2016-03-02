#include "cRCreceiver.h"
#include "cMotorDriver.h"
#include <iostream>
#include <wiringPi.h>

int main(void)
{
	wiringPiSetup();

	cRCreceiver my_RC;
	std::pair<int, int> vel(0,0);

	cPwmBoard my_PWM;

	cMotorDriver my_motors(&my_PWM);

	while(1)
	{
		vel = my_RC.getInputMovementSpeed();
		my_motors.setMotorSpeed(MOTOR_A, vel.first/10);
	}
}