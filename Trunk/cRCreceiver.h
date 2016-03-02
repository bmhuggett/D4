/* RCreceiver.h
 * Author: Ben
 * Description: Class cRCreceiver is used for getting the inputs from the user from the RC receiver module.
 */

#ifndef CRCRECEIVER_H
#define CRCRECEIVER_H



typedef const enum RC_PINS_T 
					{
					RC_RHS_LEFTRIGHT_PIN = 0, 
					RC_RHS_UPDOWN_PIN,
					RC_LHS_UPDOWN_PIN,
					RC_LHS_LEFTRIGHT_PIN,
					RC_maximum_channels = 4
					} RC_PINS;

class cRCreceiver
{
public:
	cRCreceiver();
	std::pair<int, int> getInputMovementSpeed(void);
	int 				getInputRotationSpeed(void);};

#endif