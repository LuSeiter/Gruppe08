/**
 * main.c
 */
#include "System.h"
#include "Distance.h"
#include "Timer.h"

System sys;
Distance sensor0;
GPIO led;
Timer measure;

void ISR0()
{
    sensor0.edgeDetect();
}

void ControlISR()
{
    sensor0.trigger();
    measure.clearInterruptFlag();
}

void main()
{
    sys.init(CFG_SYS_FREQ);

    //TODO: Put your code here -> A5.2

    while(1)
    {
        //TODO: Put your code here -> A5.2
    }
}
