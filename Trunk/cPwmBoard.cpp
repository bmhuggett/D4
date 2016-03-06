#include "cPwmBoard.h"
#include <iostream>
#include <wiringPiI2C.h>
#include "utils.h"

//#define PWM_DEBUG //Comment out to disable debug

cPwmBoard::cPwmBoard()
{
    pwmFd = -1;
    if (setup() <0)
    {
        #ifdef PWM_DEBUG
        std::cout << "PWM | Failed to initialise PWM board I2C" << std::endl;;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Failed to initialise PWM board I2C" << std::endl;
        #endif
    }
    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x01))          //Wake the device
    {
        #ifdef PWM_DEBUG
        std::cout<< "PWM | Could not wake device (MODE1 WRITE)"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Could not wake device (MODE1 WRITE)" << std::endl;
        #endif
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

    #ifdef PWM_DEBUG
    std::cout<<"PWM | Writing to "<<std::hex<<reg<<" with value "<<std::hex<<high<<"|"<<std::hex<<low<<std::dec<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "PWM | Writing to " << std::hex<<reg << " with value " << std::hex<<high << "|" << std::hex<<low << std::dec << std::endl;
    #endif

    if(pwmFd == -1)
    {
        #ifdef PWM_DEBUG
        std::cout <<  "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,0)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | PWM write failed ("<<std::hex<< reg <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex<<reg <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,0)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | PWM write failed ("<<std::hex<< reg+1 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex<<reg+1 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,low)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | PWM write failed ("<<std::hex<< reg+2 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex<<reg+2 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,high)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | PWM write failed ("<<std::hex<< reg+3 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex<<reg+3 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    return 0;
}

int cPwmBoard::setPwmInv(int reg, float duty)
{
    int val = (40.96 * duty)-1;  //0~4095 values in PWM
    int high = val>>8;    //High nibble(4bit)
    int low = val-(high<<8);      //Low byte

    #ifdef PWM_DEBUG
    std::cout<<"PWM | Writing to "<<std::hex<<reg<<" with value "<<std::hex<<high<<"|"<<std::hex<<low<<std::dec<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "PWM | Writing to " << std::hex<<reg << " with value " << std::hex<<high << "|" << std::hex<<low <<std::dec << std::endl;
    #endif

    if(pwmFd == -1)
    {
        #ifdef PWM_DEBUG
        std::cout <<  "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,reg,low)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Inverted PWM write failed ("<<std::hex<< reg <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Inverted PWM write failed (" << std::hex<<reg <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+1,high)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Inverted PWM write failed ("<<std::hex<< reg+1 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Inverted PWM write failed (" << std::hex<<reg+1 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+2,0xFF)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Inverted PWM write failed ("<<std::hex<< reg+2 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Inverted PWM write failed (" << std::hex<<reg+2 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,reg+3,0x0F)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Inverted PWM write failed ("<<std::hex<< reg+3 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Inverted PWM write failed (" << std::hex<<reg+3 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    return 0;
}

int cPwmBoard::kill(int reg)
{
    #ifdef PWM_DEBUG
	std::cout << "PWM | Killing " << std::hex << reg << std::dec << std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "PWM | Inverted PWM write failed (" << std::hex<<reg+3 <<std::dec << ")" << std::endl;
    #endif

	if (pwmFd == -1)
	{
        #ifdef PWM_DEBUG
		std::cout << "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Attempted to write to non-initalised PWM board" << std::endl;
        #endif

		return -1;
	}

	if (wiringPiI2CWriteReg8(pwmFd, reg, 0)<0)
	{
        #ifdef PWM_DEBUG
		std::cout << "PWM | PWM write failed (" << std::hex << reg << std::dec << ")" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex << reg << std::dec << ")" << std::endl;
        #endif

		return -1;
	}
	if (wiringPiI2CWriteReg8(pwmFd, reg + 1, 0)<0)
	{
        #ifdef PWM_DEBUG
		std::cout << "PWM | PWM write failed (" << std::hex << reg + 1 << std::dec << ")" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex << reg + 1 << std::dec << ")" << std::endl;
        #endif

		return -1;
	}
	if (wiringPiI2CWriteReg8(pwmFd, reg + 2, 0)<0)
	{
        #ifdef PWM_DEBUG
		std::cout << "PWM | PWM write failed (" << std::hex << reg + 2 << std::dec << ")" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex << reg + 2 << std::dec << ")" << std::endl;
        #endif

		return -1;
	}
	if (wiringPiI2CWriteReg8(pwmFd, reg + 3, 0x10)<0)
	{
        #ifdef PWM_DEBUG
		std::cout << "PWM | PWM write failed (" << std::hex << reg + 3 << std::dec << ")" << std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | PWM write failed (" << std::hex << reg + 3 << std::dec << ")" << std::endl;
        #endif

		return -1;
	}
	return 0;
}

int cPwmBoard::setPwmAll(float duty)
{
    int val = 4096 * duty;  //4096 values in PWM
    int high = val/256;    //High byte
    int low = val-high;      //Low byte

    #ifdef PWM_DEBUG
    std::cout<<"PWM | Writing to "<<std::hex<<PWM_ALL_ON<<" with value "<<std::hex<<high<<"|"<<std::hex<<low<<std::dec<<std::endl;
    #endif

    #ifdef LOGGING_FULL
    logfile << "PWM | Writing to " << std::hex<<PWM_ALL_ON << " with value " << std::hex<<high << "|" << std::hex<<low <<std::dec << std::endl;
    #endif

    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON,0)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting all PWM failed ("<<std::hex<< PWM_ALL_ON <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting all PWM failed (" << std::hex<<PWM_ALL_ON <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_ON+1,0)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting all PWM failed ("<<std::hex<< PWM_ALL_ON+1 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting all PWM failed (" << std::hex<<PWM_ALL_ON+1 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF,low)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting all PWM failed ("<<std::hex<< PWM_ALL_ON+2 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting all PWM failed (" << std::hex<<PWM_ALL_ON+2 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    if(wiringPiI2CWriteReg8(pwmFd,PWM_ALL_OFF+1,high)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting all PWM failed ("<<std::hex<< PWM_ALL_ON+3 <<std::dec<<")"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting all PWM failed (" << std::hex<<PWM_ALL_ON+3 <<std::dec << ")" << std::endl;
        #endif

        return -1;
    }
    return 0;
}

int cPwmBoard::setDrive(int mode)
{
    int regval =   wiringPiI2CReadReg8(pwmFd,MODE2);
    if (mode == OPEN_DRAIN)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting drive to Open Drain"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting drive to Open Drain" << std::endl;
        #endif

        regval  =   bitLow(2,regval);      //Bit 2 = 0
    }
    else if (mode == TOTEM_POLE)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting drive to Totem Pole"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting drive to Totem Pole" << std::endl;
        #endif

        regval =    bitHigh(2,regval);       //Bit 2 = 1
    }

    if(wiringPiI2CWriteReg8(pwmFd,MODE2,regval)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting drive mode failed (MODE2)"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting drive mode failed (MODE2)" << std::endl;
        #endif

        return -1;
    }
    return 0;

}

int cPwmBoard::setFreq(float freq)
{
    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x11))          //Sleep the device
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Sleeping device failed (MODE1)"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Sleeping device failed (MODE1)" << std::endl;
        #endif
    }
    
    int regvalue = (int)(25000000.0/(4096.0*freq))    -1;

    if(wiringPiI2CWriteReg8(pwmFd,PRESCALE_REG,regvalue)<0)
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Setting prescaler failed (PRESCALE_REG)"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Setting prescaler failed (PRESCALE_REG)" << std::endl;
        #endif

        return -1;
    }

    if(wiringPiI2CWriteReg8(pwmFd,MODE1,0x01))          //Wake the device
    {
        #ifdef PWM_DEBUG
        std::cout<<"PWM | Waking device failed (MODE1)"<<std::endl;
        #endif

        #ifdef LOGGING_FULL
        logfile << "PWM | Waking device failed (MODE1)" << std::endl;
        #endif
    }
    return 0;
}
