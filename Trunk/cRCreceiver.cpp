/* RCreceiver.cpp
 * Author: Ben
 * Description: Gets the inputs from the RC receiver module's PWM signal.
 */

#include <wiringPi.h>
#include "cRCreceiver.h"

#define RC_PW_OFFSET 1500
//#define RC_DEBUG //Comment out to disable debug


unsigned int start_times[RC_maximum_channels];
unsigned int PWs_in_us[RC_maximum_channels];

static void startStopR_LRTimer(void);
static void startStopR_UDTimer(void);
static void startStopL_LRTimer(void);
static void startStopL_UDTimer(void);


cRCreceiver::cRCreceiver()
{
	wiringPiISR(RC_RHS_LEFTRIGHT_PIN, INT_EDGE_BOTH, &startStopR_LRTimer);
	wiringPiISR(RC_RHS_UPDOWN_PIN, 	  INT_EDGE_BOTH, &startStopR_UDTimer);
	wiringPiISR(RC_LHS_LEFTRIGHT_PIN, INT_EDGE_BOTH, &startStopL_LRTimer);
	wiringPiISR(RC_LHS_UPDOWN_PIN, 	  INT_EDGE_BOTH, &startStopL_UDTimer);

	for(int i = 0; i < RC_maximum_channels; i++)	PWs_in_us[i] = RC_PW_OFFSET;

	#ifdef RC_DEBUG
    std::cout<<"RC | Class instantiated on pins "<<RC_RHS_LEFTRIGHT_PIN<<", "<<RC_RHS_UPDOWN_PIN<<", "RC_LHS_LEFTRIGHT_PIN<<", "RC_LHS_UPDOWN_PIN<<std::endl;
    #endif
}

// Returns x,y velocity vector desired by user as values from -500ish to 500ish.
std::pair<int, int> cRCreceiver::getInputMovementSpeed(void)
{
	int x = PWs_in_us[RC_RHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int y = PWs_in_us[RC_RHS_UPDOWN_PIN] - RC_PW_OFFSET;

	std::pair<int, int> velocity_vector(x, y);

	return velocity_vector;
}

// Returns rotational change desired by user as values from -500ish to 500ish.
int cRCreceiver::getInputRotationSpeed(void)
{
	int x = PWs_in_us[RC_LHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;

	return x;
}


// ISR for right-hand left/right channel.
static void startStopR_LRTimer(void)
{
	if(digitalRead(RC_RHS_LEFTRIGHT_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_RHS_LEFTRIGHT_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_RHS_LEFTRIGHT_PIN] = micros() - start_times[RC_RHS_LEFTRIGHT_PIN];
	}
}

// ISR for right-hand up/down channel.
static void startStopR_UDTimer(void)
{
	if(digitalRead(RC_RHS_UPDOWN_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_RHS_UPDOWN_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_RHS_UPDOWN_PIN] = micros() - start_times[RC_RHS_UPDOWN_PIN];
	}
}

// ISR for left-hand left/right channel.
static void startStopL_LRTimer(void)
{
	if(digitalRead(RC_LHS_LEFTRIGHT_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_LHS_LEFTRIGHT_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_LHS_LEFTRIGHT_PIN] = micros() - start_times[RC_LHS_LEFTRIGHT_PIN];
	}
}

// ISR for left-hand up/down channel.
static void startStopL_UDTimer(void)
{
	if(digitalRead(RC_LHS_UPDOWN_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_LHS_UPDOWN_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_LHS_UPDOWN_PIN] = micros() - start_times[RC_LHS_UPDOWN_PIN];
	}
}