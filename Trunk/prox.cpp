/* prox.cpp
 * Author: Acura Tang
 * Description: intercaing with the proximity sensor
 */#include <prox.h>
#include <iostream>
#include <wiringPi.h>

//golbal variables
int proxStartTime = 0;//the srat time
int proxDistance = 500;//distance in cm
bool proxReadyFlag = false;

static void proxISR(void)
{
    if (digitalRead( ISR_PIN))//rising edge
    {
        proxStartTime = micros();
        std::cout<<"strat time :"<<proxStartTime<<std::endl;
    }
    else
    {
        proxDistance = (micros()-proxStartTime)*0.034;//falling edge
        proxReadyFlag = true;
        std::cout<<"set distance to :"<<proxDistance<<std::endl;

    }
}


prox::prox(void)
{
    pinMode(PROX_TRIGGER_PIN,OUTPUT);//seting the output pin to the proximity sensor
    if(wiringPiISR(ISR_PIN,INT_EDGE_BOTH,&proxISR)<0)
    {
        std::cout<<"Stra Time ISR Setup Failed"<<std::endl;
    }
}

void prox::proxTrigger(void)//send out pluse to Tring pin
{
    proxReadyFlag = false;
    digitalWrite(PROX_TRIGGER_PIN,1);
    delayMicroseconds(10);
    digitalWrite(PROX_TRIGGER_PIN,0);
    std::cout<<"sand trigger"<<std::endl;

}

int prox::getDistance(void)// retun -1 if there is no nes data
{
    if(proxReadyFlag == true)
    {
        return proxDistance;
    }
    else
    {
        return -1;
    }
}
