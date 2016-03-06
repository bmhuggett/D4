/*	cImuBoard.cpp
*	Author: George
*	Description: Implements the class described in cAudioVisual.h
*/

#include "cAudioVisual.h"
#include <wiringPi.h>
#include <iostream>
#define AV_DEBUG


cAudioVisual::cAudioVisual(cPwmBoard* pwmPtr)
{
	pwmInstance = pwmPtr;
	routinePending = 0;
	int x = piThreadCreate(avLoop);
	if(x)
	{
		#ifdef AV_DEBUG
		std::cout<<"AV | Thread failed to start"<<std::endl;
		#endif
	}
	
}

void cAudioVisual::setRoutine(eAVRoutines routine)
{
	routinePending = 1;
	nextRoutine = routine;
}


void cAudioVisual::setCVLED(bool state)
{
	if(state)
	{
		pwmInstance->setPwm(PWM_15,0);	//Sink current to LEDs
	}
	else
	{
		pwmInstance->setPwm(PWM_15,100);
	}

	
}

void* avLoop(void* dummy)
{
	while(1)
	{
		if(routinePending)	//If we have a routine requested
		{
			routinePending = 0;	//Indicate servicing the routine
			switch (nextRoutine)
			{
				default:
					#ifdef IMU_DEBUG
					std::cout<<"AV | Dropped out of AV routine switch"<<std::endl;
					#endif
					break;
			}
		}
	}
}
