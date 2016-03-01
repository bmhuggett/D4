

//REGISTERS
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

#define PWM_ALL_ON 	0xFA
#define PWM_ALL_OFF 0xFC

#define MODE1	0x00
#define MODE2	0x01

#define OPEN_DRAIN 	0
#define TOTEM_POLE 	1

#define PRESCALE_REG	0xFE
#define PRESCALE_MAX 	0xFF
#define PRESCALE_MIN 	0x03
#define PRESCALE_MAXF	1526	//Hz
#define PRESCALE_MINF	24		//Hz
//REGISTERS

class cPwmBoard
{
public:
	cPwmBoard();
	int setPwm(int reg, float duty);	//Set's the PWM of reg with a duty% duty cycle
	int setPwmInv(int reg, float duty);	//Opposite signal to setPwm (so 100-duty duty cycle)
	int setPwmAll(float duty);			//Similar in effect to setPwm on all channels
	int setDrive(int mode);				//Sets drive to either Open Drain or Totem Pole (see defines)
	int setFreq(float freq);			//Sets board frequency to freq (between 24 and 1526 Hz)

private:
	int setup();
	int pwmFd;
};

