/*	cImuBoard.cpp
*	Author: George
*	Description: Allows calls to run routines that play audio
*		and run LEDs. Exists in a separate thread to avoid
*		blocking while allowing long routines
*/

#include "cPwmBoard.h"
#include "music.h"

//Routine names
enum eAVRoutines {CV_ON,CV_FOUND,CV_LOST,PROX_NEAR,TURN_ON};

class cAudioVisual
{
private:
	cAudioVisual();
	cPwmBoard* pwmInstance;	//Points to the PWM board
	
	
public:
	cAudioVisual(cPwmBoard* pwmPtr);	//Only initialise with a PWM class
	void setRoutine(eAVRoutines routine);	//Sets the next routine to run
	void setCVLED(bool state);	//Control the CV LED
};

static eAVRoutines nextRoutine;	//Statics that control next routine
static bool routinePending;	//and if it needs to run
static void* avLoop(void* dummy);	//Loop func