#include "cPwmBoard.h"

cPwmBoard::cPwmBoard()
{
    pwmFd = -1;
    if (setup() == -1)
    {
        std::cout << "Failed to initialise PWM board";
    }
}

int cPwmBoard::setup()
{
    return wiringPiI2CSetup(PWM_ADDRESS);
}


int cPwmBoard::setPWM(int reg, float duty)
{

    int val = 4096 * duty;  //4096 values in PWM
    int high = val/256;    //High byte
    int low = val-high;      //Low byte
    if(pwmFd == -1)
    {
        std::cout <<  "Attempted to write to non-initalised PWM board";
        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value upper byte
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write low byte to start value lower byte
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write high byte to start value upper byte
    }

    return 0;
}