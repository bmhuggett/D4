/* RCreceiver.cpp
 * Author: Ben
 * Description:
 */

#include <wiringPi.h>
#include "cRCreceiver.h"

#define RC_PW_OFFSET_X 1500

unsigned int start_times[sizeof(RC_PINS_T)];
unsigned int PWs_in_us[sizeof(RC_PINS_T)];

static void startStopLRTimer(void);
static void startStopUDTimer(void);


cRCreceiver::cRCreceiver()
{
	wiringPiISR(RC_LEFTRIGHT_PIN, INT_EDGE_BOTH, &startStopLRTimer);
	wiringPiISR(RC_UPDOWN_PIN, 	  INT_EDGE_BOTH, &startStopUDTimer);
}

// Returns x,y velocity vector desired by user as values from -500ish to 500ish.
std::pair<int, int> cRCreceiver::getInputSpeed(void)
{
	int x = PWs_in_us[RC_LEFTRIGHT_PIN] - RC_PW_OFFSET;
	int y = PWs_in_us[RC_UPDOWN_PIN] - RC_PW_OFFSET;

	std::pair<int, int> velocity_vector(x, y);

	return velocity_vector;
}


// ISR for left/right channel.
static void startStopLRTimer(void)
{
	if(digitalRead(RC_LEFTRIGHT_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_LEFTRIGHT_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_LEFTRIGHT_PIN] = micros() - start_times[RC_LEFTRIGHT_PIN];
	}
}

// ISR for up/down channel.
static void startStopUDTimer(void)
{
	if(digitalRead(RC_UPDOWN_PIN) == HIGH)	//Rising edge
	{
		start_times[RC_UPDOWN_PIN] = micros();
	}
	else	//Falling edge
	{
		PWs_in_us[RC_UPDOWN_PIN] = micros() - start_times[RC_UPDOWN_PIN];
	}
}