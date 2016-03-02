/* cMotorDriver.cpp
 * Author: Ben
 * Description: Writes to the PWM board to control motor speeds/directions.
 */

#include "cMotorDriver.h"
#include <iostream>

//#define MOTOR_DEBUG

cMotorDriver::cMotorDriver(cPwmBoard* pPwmBoardInstance)
{
	#ifdef MOTOR_DEBUG
	std::cout<<"MOTOR | Class Instantiated"<<std::endl;
	#endif
	pPwmBoard = pPwmBoardInstance;
	pPwmBoard->setFreq(70);
	pPwmBoard->setDrive(TOTEM_POLE);
	pPwmBoard->setPwmAll(50);
}

// Choose a motor to set to a speed between -50ish and 50ish (??)
void cMotorDriver::setMotorSpeed(MOTORS_T motor, int speed)
{
	int duty;

	// Scale input to give more meaningful output.
	if      (speed <-5)	duty = (speed+50) *7/9;
	else if (speed > 5)	duty = (speed-5) *7/9 + 65;
	else 				duty = 50;

	// Avoid maximing out duty cycle.
	if     (duty < 2) 	duty = 2;
	else if(duty >98) 	duty = 98;
		
	switch(motor)
	{
	case MOTOR_A: 
		pPwmBoard->setPwm(PWM_0,duty);
		pPwmBoard->setPwmInv(PWM_1,duty);
		if(duty == 50)		
		{
			pPwmBoard->kill(PWM_0);
			pPwmBoard->kill(PWM_1);
		}		
		break;
	case MOTOR_B:
		pPwmBoard->setPwm(PWM_2,duty);
		pPwmBoard->setPwmInv(PWM_3,duty);
		if(duty == 50)		
		{
			pPwmBoard->kill(PWM_2);
			pPwmBoard->kill(PWM_3);
		}		
		break;		
	case MOTOR_C:
		pPwmBoard->setPwm(PWM_4,duty);
		pPwmBoard->setPwmInv(PWM_5,duty);
		if(duty == 50)		
		{
			pPwmBoard->kill(PWM_4);
			pPwmBoard->kill(PWM_5);
		}		
		break;
	case MOTOR_D:
		pPwmBoard->setPwm(PWM_6,duty);
		pPwmBoard->setPwmInv(PWM_7,duty);
		if(duty == 50)		
		{
			pPwmBoard->kill(PWM_6);
			pPwmBoard->kill(PWM_7);
		}		
		break;
	default:
		std::cerr << "Invalid motor being set!" << std::endl;
	}
}
