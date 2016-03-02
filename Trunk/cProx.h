/* prox.h
 * Author: Acura Tang
 * Description: interfaing with the proximity sensor
 */

#ifndef CPROX_H
#define CPROX_H

#define PROX_ECHO_PIN 4
#define PROX_TRIGGER_PIN 5//proximity sensor's trigger pin


class cProx
{
public:
     cProx(void);				//Sets up ISRs and 
     void proxTrigger(void);	//sends 10uS pulse to the trigger input (add a 100mS delay between proxTrigger and getDistance)
     int getDistance(void);		//returns distance, if there is no new data returns -1 (add a 100mS delay between proxTrigger and getDistance)

};

//void proxISR(void) also declared here as static

#endif