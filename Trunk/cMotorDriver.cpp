/* cMotorDriver.cpp
 * Author: Ben
 * Description: Writes to the PWM board to control motor speeds/directions.
 */

#include "cMotorDriver.h"

//#define MOTOR_DEBUG

cMotorDriver::cMotorDriver(cPwmBoard* pPwmBoardInstance)
{
	#ifdef MOTOR_DEBUG
	std::cout<<"MOTOR | Class Instantiated"<<std::endl;
	#endif
	pPwmBoard = pPwmBoardInstance;
	pPwmBoard->setDrive(TOTEM_POLE);
	//pPwmBoard->setFreq( );		TODO!!
	pPwmBoard->setPwmAll(50);
}

// Choose a motor to set to a speed between -50ish and 50ish (??)
void cMotorDriver::setMotorSpeed(MOTOR_PINS_T motor, int speed)
{
	duty = speed + 50;
	if(duty < 2) 		duty = 2;
	else if(duty > 98) 	duty = 98;

	switch(motor)
	{
	case MOTOR_A: 
		pPwmBoard->setPwm(PWM_0,duty);
		pPwmBoard->setPwmInv(PWM_1,duty);
	case MOTOR_B:
		pPwmBoard->setPwm(PWM_2,duty);
		pPwmBoard->setPwmInv(PWM_3,duty);
	case MOTOR_C:
		pPwmBoard->setPwm(PWM_4,duty);
		pPwmBoard->setPwmInv(PWM_5,duty);
	case MOTOR_D:
		pPwmBoard->setPwm(PWM_6,duty);
		pPwmBoard->setPwmInv(PWM_7,duty);
	default:
		std::cerr << "Invalid motor being set!" << endl;
}