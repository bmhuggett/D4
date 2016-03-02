#include <iostream>
#include <wiringPi.h>
#include <cPwmBoard.h>
#include <prox.h>
#include <tracker.h>

#define ROTATION_DIVISOR     5
#define MOTOR_DRIVER_DIVISOR 10

PI_THREAD(music)
{
       system("omxplayer -o local BB8.mp3");
}


int main()
{
    if(wiringPiSetup()<0)
    {
        std::cout << "WiringPi Setup Failed" << std::endl;
        return -1;
    }

    piThreadCreate(music);

    prox prox0;
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
        vel = my_RC.getInputMovementSpeed();
        rot = my_RC.getInputRotationSpeed();

        my_motors.setMotorSpeed(MOTOR_A, (rot/ROTATION_DIVISOR + vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_B, (rot/ROTATION_DIVISOR - vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_C, (rot/ROTATION_DIVISOR + vel.second)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_D, (rot/ROTATION_DIVISOR - vel.second)/MOTOR_DRIVER_DIVISOR);
    }

    //cvMode();

    return 0;
}
