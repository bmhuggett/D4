/* cMotorDriver.h
 * Author: Ben
 * Description: Wrapper class for cPwmBoard, to make motor control easy.
 */

#ifndef CMOTORDRIVER_H
#define CMOTORDRIVER_H

#include "cPwmBoard.h"

#define INPUT_DEADZONE_OFF 10
#define INPUT_DEADZONE_LOW 30

typedef enum MOTORS_T
		{
		MOTOR_A,
		MOTOR_B,
		MOTOR_C,
		MOTOR_D
		} MOTORS;

class cMotorDriver
{
public:
	cMotorDriver(cPwmBoard* pPwmBoardInstance);
	void setMotorSpeed(MOTORS_T motor, int speed);
private:
	cPwmBoard* pPwmBoard;
};

#endif
