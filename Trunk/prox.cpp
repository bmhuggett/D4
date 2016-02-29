/* prox.cpp
 * Author: Acura Tang
 * Description: intercaing with the proximity sensor
 */
 #include <prox.h>

void static proxStartTimer(void)
{
    proxStartTime = millis();
}

void static proxStopTimer(void)
{
    proxDistance = (millis()-proxStartTime)*0.034;
    proxReadyFlag = true;
}

prox::prox(void)
{
    pinMode(PROX_TRIGGER_PIN,OUTPUT);//seting the output pin to the proximity sensor
    if(wiringPiISR(ISR_START_PIN,INT_EDGE_RISING,&proxStartTimer)<0)
    {
        std::cout<<"Stra Time ISR Setup Failed"<<std::endl;
    }
    if(wiringPiISR(ISR_STOP_PIN,INT_EDGE_RISING,&proxStopTimer)<0)
    {
        std::cout<<"STop Time ISR Setup Failed"<<std::endl;
    }
}

void prox::proxTrigger(void)//send out pluse to Tring pin
{
    proxReadyFlag = false;
    digitalWrite(PROX_TRIGGER_PIN,1);
    delayMicroseconds(10);
    digitalWrite(PROX_TRIGGER_PIN,0);

}

int getDistance(void)// retun -1 if there is no nes data
{
    if(proxReadyFlag)
    {
        proxReadyFlag = false;
        return proxDistance;
    }
    return -1;
}
