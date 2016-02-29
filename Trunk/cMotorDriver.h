/* cMotorDriver.h
 * Author: Ben
 * Description: Wrapper class for cPwmBoard, to make motor control easy.
 */

#include "cPwmBoard.h"

typedef enum MOTOR_PINS_T
		{
		MOTOR_A = 4,
		MOTOR_B = 5,
		MOTOR_C = 6,
		MOTOR_D = 7
		} MOTOR_PINS;

class cMotorDriver
{
public:
	cMotorDriver(cPwmBoard* pPwmBoardInstance);
	setMotorSpeed(MOTOR_PINS_T motor, int speed);
private:
	cPwmBoard* pPwmBoard;
};