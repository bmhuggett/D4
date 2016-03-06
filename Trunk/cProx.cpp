/* prox.cpp
 * Author: Acura Tang
 * Description: interfaing with the proximity sensor
 */

//#define PROX_DEBUG  //Comment out to disable debug

#include "cProx.h"
#include <iostream>
#include <wiringPi.h>
#include "utils.h"

//golbal variables
int proxStartTime = 0;  //the start time
int proxDistance = 500; //distance in cm
bool proxReadyFlag = false;

static void proxISR(void)
{
    if (digitalRead( PROX_ECHO_PIN))  //rising edge
    {
        proxStartTime = micros();

        #ifdef PROX_DEBUG
        std::cout<<"Prox | Start time:\t"<<proxStartTime<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "Prox | Start time:\t" << proxStartTime << std::endl;
        #endif
    }
    else
    {
        proxDistance = (micros()-proxStartTime)*0.034;  //falling edge
        proxReadyFlag = true;

        #ifdef PROX_DEBUG
        std::cout<<"Prox | Distance:\t"<<proxDistance<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "Prox | Distance:\t" << proxDistance << std::endl;
        #endif
    }
}


cProx::cProx(void)
{
    
    pinMode(PROX_TRIGGER_PIN,OUTPUT);                       //setting the output pin to the proximity sensor
    if(wiringPiISR(PROX_ECHO_PIN,INT_EDGE_BOTH,&proxISR)<0)
    {
        std::cout<<"Prox | ISR Setup Failed"<<std::endl;

        #ifdef LOGGING_FULL
        logfile << "Prox | ISR Setup Failed" << std::endl;
        #endif
    }
    #ifdef PROX_DEBUG
    std::cout<<"Prox | Class instantiated on pins "<<PROX_TRIGGER_PIN<<", "<<PROX_ECHO_PIN<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "Prox | Class instantiated on pins " << PROX_TRIGGER_PIN << ", " << PROX_ECHO_PIN << std::endl;
    #endif
}

void cProx::proxTrigger(void)                                //send out pulse to Trigger pin
{
    proxReadyFlag = false;
    digitalWrite(PROX_TRIGGER_PIN,1);
    delayMicroseconds(10);
    digitalWrite(PROX_TRIGGER_PIN,0);

    #ifdef PROX_DEBUG
    std::cout<<"Prox | Sent trigger out"<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "Prox | Sent trigger out" << std::endl;
    #endif
}

int cProx::getDistance(void)// returns -1 if there is no new data
{
    if(proxReadyFlag == true)
    {
        return proxDistance;
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
