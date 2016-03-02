/* RCtest.cpp
 * Author: Ben
 * Description: Test to output the pulse width on channel 1 of the RC receiver (Left/right on right stick).
 */

#include <iostream>
#include <wiringPi.h>

#define LR_PIN 0

void startstopLRtimer(void);

unsigned int RC_start_times[2];
unsigned int RC_PWs_in_us[2];


int main(void)
{
	wiringPiSetup();

	wiringPiISR(LR_PIN, INT_EDGE_BOTH, &startstopLRtimer);

	while(1)
	{
		std::cout << "LR pulse width is " << RC_PWs_in_us[LR_PIN] << "us." << std::endl;
		delay(100);
	};
}

void startstopLRtimer(void)
{
	if(digitalRead(LR_PIN) == HIGH)
	{
		RC_start_times[LR_PIN] = micros();
		//cout << "Rising edge" << endl;
	}
	else
	{
		RC_PWs_in_us[LR_PIN] = micros() - RC_start_times[LR_PIN];
		//cout << "Falling edge" << endl;
	}

	return;
}

