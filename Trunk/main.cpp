#include <iostream>
#include <wiringPi.h>
#include "cMotorDriver.h"
#include "cRCreceiver.h"
#include "cProx.h"
#include "utils.h"

#define ROTATION_DIVISOR     2
#define MOTOR_DRIVER_DIVISOR 10

/*PI_THREAD(music)
{
       system("omxplayer -o local BB8.mp3");
}*/


int main()
{
    #ifdef LOGGING_FULL
    if(!ouputfile.is_open())
    {
        std::cout << "Log file failed to open" << std::endl;
        return -1;
    }
    #endif

    if(wiringPiSetup()<0)
    {
        std::cout << "WiringPi Setup Failed" << std::endl;

        #ifdef LOGGING_FULL
        logfile << "WiringPi Setup Failed" << std::endl;
        #endif

        return -1;
    }

    //piThreadCreate(music);

    cProx prox0;
    /*
    while(1)
    {
        prox0.proxTrigger();
        delay(100);
        cout<<"distance :  "<<prox0.getDistance()<<endl<<endl;
        delay(5000);
    }
    */

    cRCreceiver my_RC = cRCreceiver();
    std::pair<int, int> vel(0,0);
    int rot = 0;

    cPwmBoard my_PWM = cPwmBoard();
    cMotorDriver my_motors(&my_PWM);

    while(1)
    {
	delay(30);
        //if(queryCVMode())   cvMode();     //TODO: Work out how to integrate this with the control loop properly.

        vel = my_RC.getInputMovementSpeed();
        rot = my_RC.getInputRotationSpeed();
	//vel.first = 0;
	//vel.second = 500;
	//rot = 0;
        my_motors.setMotorSpeed(MOTOR_A, (rot/ROTATION_DIVISOR + vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_B, (rot/ROTATION_DIVISOR - vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_C, (rot/ROTATION_DIVISOR + vel.second)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_D, (rot/ROTATION_DIVISOR - vel.second)/MOTOR_DRIVER_DIVISOR);
    }

    return 0;
}
