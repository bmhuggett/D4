# include <prox.h>

void proxSetup(void)
{
    pinMode(PROX_OUTPUT_PIN,OUTPUT);//seting the output pin to the proximity sensor
    if(wiringPiISR(ISR_START_PIN,INT_EDGE_FALLING,&proxStartTimer)<0)
    {
        std::cout<<"Stra Time ISR Setup Failed"<<std::endl;
    }
    if(wiringPiISR(ISR_STOP_PIN,INT_EDGE_RISING,&proxStopTimer)<0)
    {
        std::cout<<"STop Time ISR Setup Failed"<<std::endl;
    }
}

void proxStartTimer(void)
{
    proxStartTime = millis();
}

void proxStopTimer(void)
{
    distance = (millis()-proxStartTime)*0.034;
}