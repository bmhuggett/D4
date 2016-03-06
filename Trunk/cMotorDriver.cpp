/* cMotorDriver.cpp
 * Author: Ben
 * Description: Writes to the PWM board to control motor speeds/directions.
 */

#include "cMotorDriver.h"
#include "utils.h"
#include <iostream>

//#define MOTOR_DEBUG

#define PWM_FREQ 70
#define DUTY_CYCLE_DIVISOR 5

int current_duties[4];

cMotorDriver::cMotorDriver(cPwmBoard* pPwmBoardInstance)
{
	#ifdef MOTOR_DEBUG
	std::cout << "MOTOR | Class Instantiated" << std::endl;
	#endif

	#ifdef LOGGING_FULL
	logfile << "MOTOR | Class Instantiated" << std::endl;
	#endif

	for(int i=0;i<4;i++) current_duties[i]=50;

	pPwmBoard = pPwmBoardInstance;
	pPwmBoard->setFreq(PWM_FREQ);
	pPwmBoard->setDrive(TOTEM_POLE);
	pPwmBoard->setPwmAll(50);
}

// Choose a motor to set to a speed between -50ish and 50ish
void cMotorDriver::setMotorSpeed(MOTORS_T motor, int speed)
{
	int current_duty = current_duties[motor];
	int input_duty;

	// Set requested duty cycle according to input speed.
	if      (speed <-INPUT_DEADZONE_LOW)	input_duty=0;
	else if (speed <-INPUT_DEADZONE_OFF)	input_duty = 15;
	else if (speed > INPUT_DEADZONE_LOW)	input_duty =100;
	else if (speed > INPUT_DEADZONE_OFF)	input_duty = 85;
	else 									input_duty = 50;
	
	// Scale current duty cycle towards the requested duty cycle.
	if (input_duty!=50)		current_duty = (int)( ((float)input_duty - (float)current_duty)/(float)DUTY_CYCLE_DIVISOR + (float)current_duty  );
	else					current_duty = 50;

	// Save current duty cycle for later.
	current_duties[motor] = current_duty;

	#ifdef MOTOR_DEBUG
	std::cout << "MOTOR | Requested Duty is " << current_duty << std::endl;
	#endif

	#ifdef LOGGING_FULL
	logfile << "MOTOR | Requested Duty is " << current_duty << std::endl;
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
		std::cout << "MOTOR | Invalid motor being set!" << std::endl;
		#ifdef LOGGING_FULL
		logfile << "MOTOR | Invalid motor being set!" << std::endl;
		#endif
	}
}
