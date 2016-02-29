#include "cPwmBoard.h"

typedef enum MOTOR_PINS_T
		{
		MOTOR_A = 4,
		MOTOR_B,
		MOTOR_C,
		MOTOR_D
		} MOTOR_PINS;

class cMotorDriver
{
public:
	cMotorDriver(cPwmBoard* pPwmBoardInstance);
	setMotorSpeed(MOTOR_PINS_T motor, int speed);
private:
	cPwmBoard* pPwmBoard;
};