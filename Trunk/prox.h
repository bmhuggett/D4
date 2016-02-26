#include <iostream>
#include <wiringPi.h>

#define ISR_START_PIN 4
#define ISR_STOP_PIN 5
#define PROX_OUTPUT_PIN 6//pin to the input of proximity sensor and ISR_START_PIN

void proxStartTimer(void);
void proxStopTimer(void);
void proxSetup(void);

//golbal variables
int proxStartTime =0;
int distance =500;//distance in cm