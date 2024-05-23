/*
 *      main.cpp
 *
 *      Author: Robin Rau
 *      E-Mail: robin.rau@student.kit.edu
 *
 * Run the Magni Silver
 */


/*
 * Config.h: Header file containing all configurable parameters of the segway,
 *           as for example its pinout.
 *           Note: all constants are prefixed by CFG_.
 * System.h: Header file for the System class (needed for example for clock
 *           settings).
 * Magni.h:  Header file for the Magni class which contains all the code for
 *           controlling a Magni Silver.
 * Timer.h:  Header file for the Timer class (used for periodic tasks).
 */

#include "Config.h"
#include "System.h"
#include "Magni.h"
#include "Timer.h"

// These objects are used inside the global ISR mainTimerISR, thus must be
// global, too.

System magniSys;
Timer mainTimer, debugTimer;
Magni magni;

void Sensor4ISR(void)
{
    /*
     * This ISR is called periodically through the edge detect to determine the distance measured
     * by sensor 5.
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     */
    magni.sensor4.edgeDetect();
}

void Sensor3ISR(void)
{
    /*
     * This ISR is called periodically through the edge detect to determine the distance measured
     * by sensor 4.
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     */
    magni.sensor3.edgeDetect();
}

void Sensor2ISR(void)
{
    /*
     * This ISR is called periodically through the edge detect to determine the distance measured
     * by sensor 3.
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     */
    magni.sensor2.edgeDetect();
}

void Sensor1ISR(void)
{
    /*
     * This ISR is called periodically through the edge detect to determine the distance measured
     * by sensor 2.
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     */
    magni.sensor1.edgeDetect();
}

void Sensor0ISR(void)
{
    /*
     * This ISR is called periodically through the edge detect to determine the distance measured
     * by sensor 1.
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     */
    magni.sensor0.edgeDetect();
}

void ISRLeft(void)
{
    /*
     * This ISR is called via the edge Detect to determine the RPM for the left wheel.
     *
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     *
     */
    magni.rpmLeft.ISR();
}
void ISRRight(void)
{
    /*
     * This ISR is called via the edge Detect to determine the RPM for the right wheel.
     *
     * Note: It is not possible to set a class method as ISR. Therefore, this "helper function" is needed.
     *
     */
    magni.rpmRight.ISR();
}
void mainISR(void)
{
    /*
     * This ISR is periodically called by the mainTimer. It causes the
     * magni class to update all inputs and set the corresponding new outputs.
     *
     * Note: it is not possible to set a class method as ISR. Therefore this
     *       "helper-function" is needed.
     */
    magni.update();
    mainTimer.clearInterruptFlag();
}

void debugISR(void)
{
    /*
      * This ISR is periodically called by the debugTsimer. It causes the system
      * class to transmit the latest debug values.
      *
      * Note: it is not possible to set a class method as ISR. Therefore this
      *       "helper-function" is needed.
      */
    // Transmits debug values
    magniSys.sendDebugVals();
    debugTimer.clearInterruptFlag();
}

void main(void)
{
    // Initializes the object according to the values in Config.h
    magniSys.init(CFG_SYS_FREQ);
    mainTimer.init(&magniSys, CFG_MAIN_TIMER_BASE, &mainISR, CFG_UPDATE_FREQ);
    //debugTimer.init(&magniSys, CFG_US_TIMER_BASE, &debugISR, CFG_US_TIMER_FREQ); //OLD Version, switched to below for further testing
    debugTimer.init(&magniSys, TIMER2_BASE, &debugISR, 10);


    //Initializes and starts the Magni
    magni.init(&magniSys, &ISRLeft, &ISRRight,&Sensor0ISR,&Sensor1ISR,&Sensor2ISR,&Sensor3ISR,&Sensor4ISR);
    mainTimer.start();
//    debugTimer.start();
    while(1)
    {
        // Some monitoring tasks can run in the background and don't have to be inside the ISR
        magni.background();
    }
}
