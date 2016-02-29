#include "cPwmBoard.h"
#include <iostream>
#include <wiringPi.h>


int main()
{
	cPwmBoard PWMBOARD = cPwmBoard();
	while(1)
	{
		int duty;
		std::cout<<"Give me a duty cycle: \t";
		std::cin>>duty;
		std::cout<<std::endl<<std::endl;
		std::cout<<"Setting to "<<std::dec<<duty<<std::endl;
		PWMBOARD.setPwm(PWM_0,duty);
		PWMBOARD.setPwmInv(PWM_1,duty);
	}
	return 0;
}

