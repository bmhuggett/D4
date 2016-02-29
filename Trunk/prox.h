/* prox.h
 * Author: Acura Tang
 * Description: intercaing with the proximity sensor
 */
#include <iostream>
#include <wiringPi.h>

#define ISR_START_PIN 4
#define ISR_STOP_PIN 5
#define PROX_TRIGGER_PIN 6//pin to the input of proximity sensor and ISR_START_PIN



//golbal variables
int proxStartTime =0;
int proxDistance =500;//distance in cm
bool proxReadyFlag = false;

class prox
{
public:
     prox(void);
     void proxTrigger(void);
     int getDistance(void);
};
