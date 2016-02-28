#include "cPwmBoard.h"
#include <iostream>
#include <wiringPiI2C.h>

cPwmBoard::cPwmBoard()
{
    pwmFd = -1;
    if (setup() <0)
    {
        std::cout << "Failed to initialise PWM board";
    }
}

int cPwmBoard::setup()
{
    pwmFd = wiringPiI2CSetup(PWM_ADDRESS);
    return pwmFd;
}


int cPwmBoard::setPwm(int reg, float duty)
    {
    int val = (40.96 * duty)-1;  //0~4095 values in PWM
    int high = val/256;    //High nibble(4bit)
    int low = val-high;      //Low byte
    if(pwmFd == -1)
    {
        std::cout <<  "Attempted to write to non-initalised PWM board";
        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value upper byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write low byte to start value lower byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write high byte to start value upper byte
        return -1;
    }

    return 0;
}

int cPwmBoard::setPwmAll(float duty)
{
    int val = 4096 * duty;  //4096 values in PWM
    int high = val/256;    //High byte
    int low = val-high;      //Low byte

    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON+1,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF+1,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    return 0;
}

int cPwmBoard::setDrive(int mode)
{
    int regval =   wiringPiI2CReadReg8(pwmFd,MODE2);
    if (mode == OPEN_DRAIN)
    {
        regval  =   regval & ~(1<<2);      //Bit 2 = 0
    }
    else if (mode == TOTEM_POLE)
    {
        regval =    regval | (1<<2);       //Bit 2 = 1
    }

    if(wiringPiI2CWriteReg8(pwmFd,MODE2,regval)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    return 0;

}

int cPwmBoard::setPreScaler(int val)
{
    if(wiringPiI2CWriteReg8(pwmFd,PRESCALE_REG,val)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    return 0;
}

int cPwmBoard::setFreq(int freq)
{
    float perc = (  (float)(freq-PRESCALE_MINF)  /  (float)(PRESCALE_MAXF-PRESCALE_MINF)    );   //Get percentage along the freq range
    int out = (int) (PRESCALE_MIN + perc*(PRESCALE_MAX-PRESCALE_MIN));                          //Translate into same fraction along the byte range

    if(wiringPiI2CWriteReg8(pwmFd,PRESCALE_REG,out)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;   //Write zero to start value lower byte
        return -1;
    }
    return 0;
}
