/* prox.h
 * Author: Ben Huggett & Acura Tang
 * Description: Interfacing with the proximity sensors
 */

#ifndef CPROX_H
#define CPROX_H

#define PROX_LEFT_ECHO_PIN 4
#define PROX_LEFT_TRIGGER_PIN 5

#define PROX_RIGHT_ECHO_PIN 6
#define PROX_RIGHT_TRIGGER_PIN 7

#define PROX_FRONT_ECHO_PIN 8
#define PROX_FRONT_TRIGGER_PIN 9

#define PROX_BACK_ECHO_PIN 10
#define PROX_BACK_TRIGGER_PIN 11


#define PROX_STOP_THRESHOLD 20


typedef enum PROX_SENSORS_T
				{
					PROX_LEFT = 0,
					PROX_RIGHT,
					PROX_FRONT,
					PROX_BACK,
					PROX_maximum_sensors = 4
				} PROX_SENSORS;


class cProx
{
public:
	cProx();
	int  getDistance(PROX_SENSORS_T sensor);
	void proxTrigger(PROX_SENSORS_T sensor);
};

int prox_current_distance[PROX_maximum_sensors];

#endif
