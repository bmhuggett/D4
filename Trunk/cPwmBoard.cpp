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
    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x01))          //Wake the device
    {
        std::cout<< "Write failed to PWM_MODE1"<<std::endl;
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
    int high = val>>8;    //High nibble(4bit)
    int low = val-(high<<8);      //Low byte
    std::cout<<"Writing to "<<std::hex<<reg<<" with value "<<std::hex<<high<<"|"<<std::hex<<low<<std::endl;
    if(pwmFd == -1)
    {
        std::cout <<  "Attempted to write to non-initalised PWM board";
        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    std::cout<<"Exiting setPwm"<<std::endl;
    return 0;
}

int cPwmBoard::setPwmInv(int reg, float duty)
{
    int val = (40.96 * duty)-1;  //0~4095 values in PWM
    int high = val>>8;    //High nibble(4bit)
    int low = val-(high<<8);      //Low byte
    std::cout<<"Writing to "<<std::hex<<reg<<" with value "<<std::hex<<high<<"|"<<std::hex<<low<<std::endl;
    if(pwmFd == -1)
    {
        std::cout <<  "Attempted to write to non-initalised PWM board";
        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,0xFF)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,0x0F)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    std::cout<<"Exiting setPwm"<<std::endl;
    return 0;
}

int cPwmBoard::setPwmAll(float duty)
{
    int val = 4096 * duty;  //4096 values in PWM
    int high = val/256;    //High byte
    int low = val-high;      //Low byte

    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON+1,0)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;  
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF,low)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;
        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF+1,high)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;  
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
        std::cout<<"Write Operation Failed"<<std::endl;  
        return -1;
    }
    return 0;

}

int cPwmBoard::setPreScaler(int val)
{
    if(wiringPiI2CWriteReg8(pwmFd,PRESCALE_REG,val)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;  
        return -1;
    }
    return 0;
}

int cPwmBoard::setFreq(float freq)
{
    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x11))          //Sleep the device
    {
        std::cout<< "Write failed to PWM_MODE1"<<std::endl;
    }
    
    int regvalue = (int)(25000000.0/(4096.0*freq))    -1;

    if(wiringPiI2CWriteReg8(pwmFd,PRESCALE_REG,regvalue)<0)
    {
        std::cout<<"Write Operation Failed"<<std::endl;  
        return -1;
    }
    return 0;

    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x01))          //Wakeleep the device
    {
        std::cout<< "Write failed to PWM_MODE1"<<std::endl;
    }
}
