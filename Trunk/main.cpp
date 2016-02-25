#include <iostream>
#include <wiringPi.h>
#include <cPwmBoard.h>


#define ISR_START_PIN 4
#define ISR_STOP_PIN 5
#define PROX_OUTPUT_PIN 6//pin to the input of proximity sensor and ISR_START_PIN

using namespace std;

//fuction header
void proxStartTimer(void);
void proxStopTimer(void);


//golbal variables
int proxStartTime =0;
int distance =500;//distance in cm


int main()
{

    if(wiringPiSetup()<0)
    {
        cout<< "WiringPi Setup Failed"<<endl;
        return -1;
    }

    void (*fPtrStart)(void)= &proxStartTimer;
    void (*fPtrStop)(void) = &proxStopTimer;

    pinMode(PROX_OUTPUT_PIN,OUTPUT);//seting the output pin to the proximity sensor
    if(wiringPiISR(ISR_START_PIN,INT_EDGE_FALLING,fPtrStart)<0)
    {
        cout<<"Stra Time ISR Setup Failed"<<endl;
    }
    if(wiringPiISR(ISR_STOP_PIN,INT_EDGE_RISING,fPtrStop)<0)
    {
        cout<<"STop Time ISR Setup Failed"<<endl;
    }


    return 0;
}


void proxStartTimer(void)
{
    proxStartTime = millis();
}

void proxStopTimer(void)
{
    distance = (millis()-proxStartTime)*0.034;
}