/* cProx.cpp
 * Author: Ben Huggett & Acura Tang
 * Description: Interfacing with the proximity sensors
 */

//#define PROX_DEBUG  //Comment out to disable debug

#include "cProx.h"
#include <iostream>
#include <wiringPi.h>
#include "utils.h"


static bool proxReadyFlag;
static unsigned int start_times[3][PROX_maximum_sensors];
static unsigned int PWs_in_us[3][PROX_maximum_sensors];
static int looper[PROX_maximum_sensors];

static void startStopLeftTimer(void);
static void startStopRightTimer(void);
static void startStopFrontTimer(void);
static void startStopBackTimer(void);


cProx::cProx()
{
    pinMode(PROX_LEFT_TRIGGER_PIN, OUTPUT);
    pinMode(PROX_RIGHT_TRIGGER_PIN, OUTPUT);
    pinMode(PROX_FRONT_TRIGGER_PIN, OUTPUT);
    pinMode(PROX_BACK_TRIGGER_PIN, OUTPUT);

    wiringPiISR(PROX_LEFT_ECHO_PIN, INT_EDGE_BOTH, &startStopLeftTimer);
    wiringPiISR(PROX_RIGHT_ECHO_PIN, INT_EDGE_BOTH, &startStopRightTimer);
    wiringPiISR(PROX_FRONT_ECHO_PIN, INT_EDGE_BOTH, &startStopFrontTimer);
    wiringPiISR(PROX_BACK_ECHO_PIN, INT_EDGE_BOTH, &startStopBackTimer);

    for(int i = 0; i < PROX_maximum_sensors; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            PWs_in_us[j][i] = 1500;
        }
        looper[i] = 0;
    }
    proxReadyFlag = true;

    #ifdef PROX_DEBUG
    std::cout << "Prox | Proximity sensors initialised." << std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "Prox | Proximity sensors initialised." << std::endl;
    #endif
}

// Returns distance in direction of 'sensor', -1 if not ready yet.
int cProx::getDistance(PROX_SENSORS_T sensor)
{
    if(proxReadyFlag == true)
    {
        int a = PWs_in_us[0][sensor];
        int b = PWs_in_us[1][sensor];
        int c = PWs_in_us[2][sensor];

        int x = (a+b+c)/3;
        
        #ifdef PROX_DEBUG
        std::cout << "Prox | Got distance: " << x << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "Prox | Got distance: " << x << std::endl;
        #endif

        return x;
    }
    else
    {
        #ifdef PROX_DEBUG
        std::cout<<"Prox | getDistance called too early"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "Prox | getDistance called too early" << std::endl;
        #endif

        return -1;
    }
}

// Sends out pulse to trigger pin
void cProx::proxTrigger(PROX_SENSORS_T sensor)
{
    proxReadyFlag = false;

    switch(sensor)
    {
        case(PROX_LEFT):
                    digitalWrite(PROX_LEFT_TRIGGER_PIN,1);
                    delayMicroseconds(10);
                    digitalWrite(PROX_LEFT_TRIGGER_PIN,0);
                    break;

        case(PROX_RIGHT):
                    digitalWrite(PROX_RIGHT_TRIGGER_PIN,1);
                    delayMicroseconds(10);
                    digitalWrite(PROX_RIGHT_TRIGGER_PIN,0);
                    break;
                    
        case(PROX_FRONT):
                    digitalWrite(PROX_FRONT_TRIGGER_PIN,1);
                    delayMicroseconds(10);
                    digitalWrite(PROX_FRONT_TRIGGER_PIN,0);
                    break;
                    
        case(PROX_BACK):
                    digitalWrite(PROX_BACK_TRIGGER_PIN,1);
                    delayMicroseconds(10);
                    digitalWrite(PROX_BACK_TRIGGER_PIN,0);
                    break;

        default:
                    std::cout<<"Prox | Invalid sensor triggered!"<<std::endl;
                    #ifdef LOGGING_FULL
                    logfile << "Prox | Invalid sensor triggered!" << std::endl;
                    #endif

    }


    #ifdef PROX_DEBUG
    std::cout<<"Prox | Sent trigger out"<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "Prox | Sent trigger out" << std::endl;
    #endif
}


// ISR for any sensor, which is handed the necessary sensor in the specific ISRs below.
static void startStopGenericTimer(PROX_SENSORS_T sensor)
{
	int pin;

    switch(sensor)
    {
        case(PROX_LEFT):
        			pin = PROX_LEFT_ECHO_PIN;
                    break;

        case(PROX_RIGHT):
        			pin = PROX_RIGHT_ECHO_PIN;
                    break;
                    
        case(PROX_FRONT):
        			pin = PROX_FRONT_ECHO_PIN;
                    break;
                    
        case(PROX_BACK):
        			pin = PROX_BACK_ECHO_PIN;
                    break;

        default:
                    std::cout<<"Prox | Invalid sensor ISR called!"<<std::endl;
                    #ifdef LOGGING_FULL
                    logfile << "Prox | Invalid sensor ISR called!" << std::endl;
                    #endif

    }

    if(digitalRead(pin) == HIGH)    //Rising edge
    {
        start_times[ looper[sensor] ][sensor] = micros();
    }
    else    //Falling edge
    {
        PWs_in_us[ looper[sensor] ][sensor] = micros() - start_times[ looper[sensor] ][sensor];
        looper[sensor] = (looper[sensor] + 1)%3;
    }
}

// ISR for left sensor.
static void startStopLeftTimer(void)
{
    startStopGenericTimer(PROX_LEFT);
}

// ISR for right sensor.
static void startStopRightTimer(void)
{
    startStopGenericTimer(PROX_RIGHT);
}

// ISR for front sensor.
static void startStopFrontTimer(void)
{
    startStopGenericTimer(PROX_FRONT);
}

// ISR for back sensor.
static void startStopBackTimer(void)
{
    startStopGenericTimer(PROX_BACK);
}
