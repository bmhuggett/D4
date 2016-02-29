#include <iostream>
#include <wiringPi.h>
#include <cPwmBoard.h>
#include <prox.h>
#include <tracker.h>

using namespace std;





int main()
{
    if(wiringPiSetup()<0)
    {
        cout<< "WiringPi Setup Failed"<<endl;
        return -1;
    }
    cvMode();

    return 0;
}
