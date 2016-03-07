/* main.cpp
 * Author: Ben Huggett, George Elliott-Hunter, Acura Tang
 * Description: Main super loop
 */

#include <iostream>
#include <wiringPi.h>
#include "cMotorDriver.h"
#include "cRCreceiver.h"
#include "cProx.h"
#include "utils.h"
#include "tracker.h"
#include "cAudioVisual.h"


#define ROTATION_DIVISOR     2
#define MOTOR_DRIVER_DIVISOR 10


int main()
{
    #ifdef LOGGING_FULL
    if(!logfile.is_open())
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


    cProx my_prox;

    cRCreceiver my_RC;
    std::pair<int, int> vel(0,0);
    int rot = 0;

    cPwmBoard my_PWM;
    cMotorDriver my_motors(&my_PWM);

    tracker my_tracker;

    cAudioVisual my_AV(&my_PWM);


    while(1)
    {
        /* Get distances to surrounding objects. */
        my_prox.proxTrigger(PROX_LEFT);
        my_prox.proxTrigger(PROX_RIGHT);
        my_prox.proxTrigger(PROX_FRONT);
        my_prox.proxTrigger(PROX_BACK);
        delay(30);
        prox_current_distance[PROX_LEFT]  = my_prox.getDistance(PROX_LEFT);
        prox_current_distance[PROX_RIGHT] = my_prox.getDistance(PROX_RIGHT);
        prox_current_distance[PROX_FRONT] = my_prox.getDistance(PROX_FRONT);
        prox_current_distance[PROX_BACK]  = my_prox.getDistance(PROX_BACK);


        /* ################### ENTER COMPUTER VISION MODE ################### */
        if(my_RC.queryCVMode())
        {
            my_AV.setCVLED(true);       // Turn LED on to indicate we are in CVmode

            switch(my_tracker.cvMode())
            {
            case 'l':                   // Turn Left
                vel.first  = 0;
                vel.second = 0;
                rot        = -500;
                break;

            case 'r':                   // Turn Right
                vel.first  = 0;
                vel.second = 0;
                rot        = 500
                break;

            case 'f':                   // Move Forwards
                vel.first  = 0;
                vel.second = 500;
                rot        = 0;
                break;

            case 'b':                   // Move Backwards
                vel.first  = 0;
                vel.second = -500;
                rot        = 0;
                break;

            default:
                std::cout << "CV | CV Mode returned invalid command!" << std::endl;
                #ifdef LOGGING_FULL
                logfile << "CV | CV Mode returned invalid command!" << std::endl;
                #endif
                break;
            }
        }

        /* ################### ENTER REMOTE CONTROL MODE ################### */      
        else
        {
            my_AV.setCVLED(false);                // Turn off CV mode indicator LED

            vel = my_RC.getInputMovementSpeed();  // Get user input from RC
            rot = my_RC.getInputRotationSpeed();
        }


        /* Decide if there is room to move in the desired direction. */
        if( (vel.first > INPUT_DEADZONE_OFF) && (prox_current_distance[PROX_RIGHT] < PROX_STOP_THRESHOLD) )
        {
            vel.first = 0;                      // Don't move right if there's no room.
            playSound("bb8-22.mp3");

        }
        else if( (vel.first < -INPUT_DEADZONE_OFF) && (prox_current_distance[PROX_LEFT] < PROX_STOP_THRESHOLD) )
        {
            vel.first = 0;                      // Don't move left if there's no room.
            playSound("bb8-22.mp3");            
        }

        if( (vel.second > INPUT_DEADZONE_OFF) && (prox_current_distance[PROX_FRONT] < PROX_STOP_THRESHOLD) )
        {
            vel.second = 0;                     // Don't move forward if there's no room.
            playSound("bb8-22.mp3");            
        }
        else if( (vel.second < -INPUT_DEADZONE_OFF) && (prox_current_distance[PROX_BACK] < PROX_STOP_THRESHOLD) )
        {
            vel.second = 0;                     // Don't move backward if there's no room.
            playSound("bb8-22.mp3");            
        }


        /* Move BB8! */
        my_motors.setMotorSpeed(MOTOR_A, (rot/ROTATION_DIVISOR + vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_B, (rot/ROTATION_DIVISOR - vel.first)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_C, (rot/ROTATION_DIVISOR + vel.second)/MOTOR_DRIVER_DIVISOR);
        my_motors.setMotorSpeed(MOTOR_D, (rot/ROTATION_DIVISOR - vel.second)/MOTOR_DRIVER_DIVISOR);
    }

    return 0;
}
