#include <iostream>
#include <wiringPi.h>
#include <cPwmBoard.h>
#include <prox.h>
#include <tracker.h>

using namespace std;


PI_THREAD(music)
{
       system("omxplayer -o local BB8.mp3");
}


int main()
{
    if(wiringPiSetup()<0)
    {
        cout<< "WiringPi Setup Failed"<<endl;
        return -1;
    }

    piThreadCreate(music);
    prox prox0;
    /*
    while(1)
    {
        prox0.proxTrigger();
        delay(100);
        cout<<"distance :  "<<prox0.getDistance()<<endl<<endl;
        delay(5000);
    }
    */
    cvMode();

    cout<<"test"<<endl;
    return 0;
}
