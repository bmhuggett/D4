/* cMotorDriver.cpp
 * Author: Ben
 * Description: Writes to the PWM board to control motor speeds/directions.
 */

#include "cMotorDriver.h"
#include <iostream>

#define MOTOR_DEBUG

#define PWM_FREQ 70
#define INPUT_DEADZONE 10
#define DUTY_CYCLE_DIVISOR 2

int current_duties[4];

cMotorDriver::cMotorDriver(cPwmBoard* pPwmBoardInstance)
{
	#ifdef MOTOR_DEBUG
	std::cout<<"MOTOR | Class Instantiated"<<std::endl;
	#endif

	for(int i=0;i<4;i++) current_duties[i]=50;

	pPwmBoard = pPwmBoardInstance;
	pPwmBoard->setFreq(PWM_FREQ);
	pPwmBoard->setDrive(TOTEM_POLE);
	pPwmBoard->setPwmAll(50);
}

// Choose a motor to set to a speed between -50ish and 50ish (??)
void cMotorDriver::setMotorSpeed(MOTORS_T motor, int speed)
{
	int current_duty = current_duties[motor];
	int input_duty;

	// Scale input to give more meaningful output.
	if      (speed <-INPUT_DEADZONE)	input_duty=0;//duty = (speed+50) *7/9;
	else if (speed > INPUT_DEADZONE)	input_duty =100;// = (speed-5) *7/9 + 65;
	else 								input_duty = 50;

	// Avoid maximing out duty cycle.
	//if     (input_duty < 2) 	input_duty = 2;
	//else if(input_duty >98) 	input_duty = 98;
	
	if (input_duty!=50)
	{	
		current_duty = (int)( ((float)input_duty - (float)current_duty)/(float)DUTY_CYCLE_DIVISOR + (float)current_duty  );
	}
	else
	{
		current_duty=50;
	}
	current_duties[motor]=current_duty;
	#ifdef MOTOR_DEBUG
	std::cout<<"MOTOR | Requested Duty is "<<current_duty<<std::endl;
	#endif
	switch(motor)
	{
	case MOTOR_A: 
		if(current_duty !=50)
		{
			pPwmBoard->setPwm(PWM_0, current_duty);
			pPwmBoard->setPwmInv(PWM_1, current_duty);
		}
		else
		{
			pPwmBoard->kill(PWM_0);
			pPwmBoard->kill(PWM_1);
		}
		break;
	case MOTOR_B:
		if(current_duty!=50)
		{
			pPwmBoard->setPwm(PWM_2, current_duty);
			pPwmBoard->setPwmInv(PWM_3, current_duty);
		}
		else
		{
			pPwmBoard->kill(PWM_2);
			pPwmBoard->kill(PWM_3);
		}
		break;
	case MOTOR_C:
		if(current_duty!=50)
		{
			pPwmBoard->setPwm(PWM_4, current_duty);
			pPwmBoard->setPwmInv(PWM_5, current_duty);
		}
		else		
		{
			pPwmBoard->kill(PWM_4);
			pPwmBoard->kill(PWM_5);
		}		
		break;
	case MOTOR_D:
		if(current_duty!=50)
		{
			pPwmBoard->setPwm(PWM_6, current_duty);
			pPwmBoard->setPwmInv(PWM_7, current_duty);
		}
		else		
		{
			pPwmBoard->kill(PWM_6);
			pPwmBoard->kill(PWM_7);
		}		
		break;
	default:
		std::cerr << "Invalid motor being set!" << std::endl;
	}
}
