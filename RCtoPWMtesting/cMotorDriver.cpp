#include "cMotorDriver.h"

cMotorDriver::cMotorDriver(cPwmBoard* pPwmBoardInstance)
{
	pPwmBoard = pPwmBoardInstance;
	pPwmBoard->setDrive(TOTEM_POLE);
	//pPwmBoard->setFreq( );	TODO
	pPwmBoard->setPwmAll(50);
}

setMotorSpeed(MOTOR_PINS_T motor, int speed)
{
	
}