// Uncomment following #define to use the precompiled ADC library instead of
// the code in this file.
//#define USE_DISTANCE_LIBRARY

#ifndef USE_DISTANCE_LIBRARY

#include "Distance.h"
#include "ErrorCodes.h"

Distance::Distance() // @suppress("Class members should be properly initialized")
{

}

Distance::~Distance() // @suppress("Class members should be properly initialized")
{

}

void Distance::init(System *sys, uint32_t echoPort, uint32_t echoPin, uint32_t triggerPort, uint32_t triggerPin, void (*ISR)(void))
{
    /*
     * Saves the configurations of the echo Port and the System Object
     */

    this->distanceSys = sys;
    this->echoPort = echoPort;
    this->echoPin = echoPin;

    //TODO: Put your code here -> A5.1
}
void Distance::trigger()
{
    /*
     * Triggers the measurement. The measurement is started when a pulse with a length
     * of minimum 10 us occurs at the trigger pin.
     */

    //TODO: Put your code here -> A3.1

}
void Distance::edgeDetect()
{
    /*
     * The class method which is called during interrupts.
     * When either edge is detected, the timer value is been stored.
     * If two values are stored, the calcDistance Method is called to
     * calculate the duration of the pulse and therefore the distance
     * to the nearest object.
     * Disables the timer and resets the measuring variable.
     */
    
    //TODO: Put your code here -> A5.1
}

float Distance::getDistance()
{
   //TODO: Put your code here -> A5.1
}


#endif

