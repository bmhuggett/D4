#include <iostream>
#include <wiringPi.h>
#include "cMotorDriver.h"
#include "cRCreceiver.h"
#include "cProx.h"
#include "tracker.h"
#include "music.h"

#define ROTATION_DIVISOR     5
#define MOTOR_DRIVER_DIVISOR 10


int main()
{
    if(wiringPiSetup()<0)
    {
        std::cout << "WiringPi Setup Failed" << std::endl;
        return -1;
    }

    cProx prox0;
    int current_distance =50;
    /*
    while(1)
    {
        prox0.proxTrigger();
        delay(100);
        cout<<"distance :  "<<prox0.getDistance()<<endl<<endl;
        delay(500);
    }
    */

    cRCreceiver my_RC = cRCreceiver();
    std::pair<int, int> vel(0,0);
    int rot = 0;

    cPwmBoard my_PWM = cPwmBoard();

    cMotorDriver my_motors(&my_PWM);

    tracker track;
/*
    while(1)
    {

        vel = my_RC.getInputMovementSpeed();
        rot = my_RC.getInputRotationSpeed();
        vel.first = 0;
        vel.second = 500;
        rot = 0;

        prox0.proxTrigger();
        delay(50);//the delay is subjected to change
        current_distance = prox0.getDistance();
        if(current_distance> 15 && current_distance< 15000)
        {
            //drive forward
        }
        else
        {
            playSound("bb8-22.mp3");
        }

        my_motors.setMotorSpeed(MOTOR_A, (rot/ROTATION_DIVISOR + vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_B, (rot/ROTATION_DIVISOR - vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_C, (rot/ROTATION_DIVISOR + vel.second)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_D, (rot/ROTATION_DIVISOR - vel.second)/MOTOR_DRIVER_DIVISOR);

        while(my_RC.queryCVMode())
        {
            my_PWM.setPwm(PWM_15,70);//turing led on to idicate we are in CVmode

            switch(track.cvMode())//the radius dondury condition is yet to be deturmaned
            {
            case 'l':
                //turn left
                break;

            case 'r':
                //turn right
                break;
            case 'f':
                //go forwrd

                break;
            case 'b':
                //go backword

                break;
            default:
                break;
            }
        }
        my_PWM.setPwm(PWM_15,0);//turing led of CVmode indicator LED
    }

*/
    playSound("BB8.mp3");
    track.cvMode();
return 0;
}
