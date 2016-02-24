#include <iostream>
#include <wiringPiI2C.h>

#define PWM_ADDRESS 0x40

#define PWM_0 	0x06
#define PWM_1 	0x0A
#define PWM_2 	0x0E
#define PWM_3 	0x12
#define PWM_4 	0x16
#define PWM_5 	0x1A
#define PWM_6 	0x1E
#define PWM_7 	0x22
#define PWM_8 	0x26
#define PWM_9 	0x2A
#define PWM_10 	0x2E
#define PWM_11 	0x32
#define PWM_12 	0x36
#define PWM_13 	0x3A
#define PWM_14 	0x3E
#define PWM_15 	0x42

class cPwmBoard
{
public:
	cPwmBoard();
	int setup();
	int setPWM(int reg, float duty);
private:
	int pwmFd;
};

