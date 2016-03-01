/* prox.h
 * Author: Acura Tang
 * Description: intercaing with the proximity sensor
 */

#ifndef CPROX_H
#define CPROX_H

#define PROX_ECHO_PIN 4
#define PROX_TRIGGER_PIN 5//proximity sensor's trigger pin



class prox
{
public:
     prox(void);
     void proxTrigger(void);//sends 10uS pulse to the trigger input (add a 100mS delay beten proxTrigger and getDistance)
     int getDistance(void);//returns distance, if there is no new date retunes -1 (add a 100mS delay beten proxTrigger and getDistance)

};

#endif