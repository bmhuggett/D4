/* RCreceiver.h
 * Author: Ben
 * Description:
 */

 #include <tuple>

typedef const enum RC_PINS_T 
					{
					RC_LEFTRIGHT_PIN = 0, 
					RC_UPDOWN_PIN	 = 1
					} RC_PINS;

class cRCreceiver
{
public:
	cRCreceiver();
	std::pair<int, int> getInputSpeed(void);
};