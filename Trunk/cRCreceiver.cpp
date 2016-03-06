/* RCreceiver.cpp
 * Author: Ben
 * Description: Gets the inputs from the RC receiver module's PWM signal.
 */

#include <wiringPi.h>
#include "cRCreceiver.h"
#include "utils.h"
#include <iostream>

#define RC_PW_OFFSET 1500
#define CV_MODE_THRESHOLD 300	// Between 0 and 500

#define RC_DEBUG //Comment out to disable debug


unsigned int start_times[3][RC_maximum_channels];
unsigned int PWs_in_us[3][RC_maximum_channels];
int looper[RC_maximum_channels];

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

	for(int i = 0; i < RC_maximum_channels; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			PWs_in_us[j][i] = RC_PW_OFFSET;
		}
		looper[i] = 0;
	}	

	#ifdef RC_DEBUG
    std::cout << "RC | Class instantiated on pins " << RC_RHS_LEFTRIGHT_PIN << ", " << RC_RHS_UPDOWN_PIN << ", " << RC_LHS_LEFTRIGHT_PIN << ", " << RC_LHS_UPDOWN_PIN << std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "RC | Class instantiated on pins " << RC_RHS_LEFTRIGHT_PIN << ", " << RC_RHS_UPDOWN_PIN << ", " << RC_LHS_LEFTRIGHT_PIN << ", " << RC_LHS_UPDOWN_PIN << std::endl;
    #endif
}

// Returns x,y velocity vector desired by user as values from -500ish to 500ish.
std::pair<int, int> cRCreceiver::getInputMovementSpeed(void)
{
	int xa = PWs_in_us[0][RC_RHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int xb = PWs_in_us[1][RC_RHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int xc = PWs_in_us[2][RC_RHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;

	int ya = PWs_in_us[0][RC_RHS_UPDOWN_PIN] - RC_PW_OFFSET;
	int yb = PWs_in_us[1][RC_RHS_UPDOWN_PIN] - RC_PW_OFFSET;
	int yc = PWs_in_us[2][RC_RHS_UPDOWN_PIN] - RC_PW_OFFSET;

	std::pair<int, int> velocity_vector( (xa+xb+xc)/3 , (ya+yb+yc)/3 );
	
	#ifdef RC_DEBUG
	std::cout << "RC | Got velocity (x,y): " << velocity_vector.first << "," << velocity_vector.second << std::endl;
	#endif

    #ifdef LOGGING_FULL
    logfile << "RC | Got velocity (x,y): " << velocity_vector.first << "," << velocity_vector.second << std::endl;
    #endif

	return velocity_vector;
}

// Returns rotational change desired by user as values from -500ish to 500ish.
int cRCreceiver::getInputRotationSpeed(void)
{
	int a = PWs_in_us[0][RC_LHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int b = PWs_in_us[1][RC_LHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int c = PWs_in_us[2][RC_LHS_LEFTRIGHT_PIN] - RC_PW_OFFSET;

	#ifdef RC_DEBUG
	std::cout << "RC | Got rotation: " << (a+b+c)/3 << std::endl;
	#endif

    #ifdef LOGGING_FULL
    logfile << "RC | Got rotation: " << (a+b+c)/3 << std::endl;
    #endif

	return (a+b+c)/3;
}

// Returns whether or not the left stick is all the way forward.
bool cRCreceiver::queryCVMode(void)
{
	int a = PWs_in_us[0][RC_LHS_UPDOWN_PIN] - RC_PW_OFFSET;
	int b = PWs_in_us[1][RC_LHS_UPDOWN_PIN] - RC_PW_OFFSET;
	int c = PWs_in_us[2][RC_LHS_UPDOWN_PIN] - RC_PW_OFFSET;

	if((a+b+c)/3 > CV_MODE_THRESHOLD)	return true;
	else								return false;
}


// ISR for any channel, which is handed the necessary pin in the specific ISRs below.
static void startStopGenericTimer(RC_PINS_T pin)
{
	if(digitalRead(pin) == HIGH)	//Rising edge
	{
		start_times[ looper[pin] ][pin] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[ looper[pin] ][pin] = micros() - start_times[ looper[pin] ][pin];
		looper[pin] = (looper[pin] + 1)%3;
	}
}

// ISR for right-hand left/right channel.
static void startStopR_LRTimer(void)
{
	startStopGenericTimer(RC_RHS_LEFTRIGHT_PIN);
}

// ISR for right-hand up/down channel.
static void startStopR_UDTimer(void)
{
	startStopGenericTimer(RC_RHS_UPDOWN_PIN);
}

// ISR for left-hand left/right channel.
static void startStopL_LRTimer(void)
{
	startStopGenericTimer(RC_LHS_LEFTRIGHT_PIN);
}

// ISR for left-hand up/down channel.
static void startStopL_UDTimer(void)
{
	startStopGenericTimer(RC_LHS_UPDOWN_PIN);
}
