#include <iostream>
#include <wiringPi.h>
#include "cMotorDriver.h"
#include "cRCreceiver.h"
#include "cProx.h"

#define ROTATION_DIVISOR     5
#define MOTOR_DRIVER_DIVISOR 10


int main()
{
    if(wiringPiSetup()<0)
    {
        std::cout << "WiringPi Setup Failed" << std::endl;
        return -1;
    }

    cProx my_prox;
    /*
    while(1)
    {
        prox0.proxTrigger();
        delay(100);
        cout<<"distance :  "<<prox0.getDistance()<<endl<<endl;
        delay(5000);
    }
    */

    cRCreceiver my_RC;
    std::pair<int, int> vel(0,0);
    int rot = 0;

    cPwmBoard my_PWM;
    cMotorDriver my_motors(&my_PWM);

    while(1)
    {
        //if(queryCVMode())   cvMode();     //TODO: Work out how to integrate this with the control loop properly.

        vel = my_RC.getInputMovementSpeed();
        rot = my_RC.getInputRotationSpeed();

        my_motors.setMotorSpeed(MOTOR_A, (rot/ROTATION_DIVISOR + vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_B, (rot/ROTATION_DIVISOR - vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_C, (rot/ROTATION_DIVISOR + vel.second)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_D, (rot/ROTATION_DIVISOR - vel.second)/MOTOR_DRIVER_DIVISOR);
    }

    return 0;
}
