

/**
 * main.c
 */

#include "System.h"
#include "Timer.h"
#include "Remote.h"

System remoteSystem;
Timer displayUpdateTimer,mainTimer, debugTimer;
Remote remote;

void ISRDisplay()
{
    remote.updateDisplay();
    displayUpdateTimer.clearInterruptFlag();
}

void ISRMain()
{
    remote.update();
    mainTimer.clearInterruptFlag();
}

void debugISR()
{
    remoteSystem.sendDebugVals();
    debugTimer.clearInterruptFlag();
}

int main(void)
{
    // Create and initialize system object
    remoteSystem.init(40000000);

    // Initialization of two timers. One for display update and one for value updates and communication
    displayUpdateTimer.init(&remoteSystem, TIMER0_BASE, &ISRDisplay, 2);
    mainTimer.init(&remoteSystem, TIMER1_BASE, &ISRMain, 100);
    debugTimer.init(&remoteSystem, TIMER2_BASE, &debugISR, 10);
    // Initialize the remote Class with all peripherals
    remote.init(&remoteSystem);

    // Start the timers
    displayUpdateTimer.start();
    mainTimer.start();
//    debugTimer.start();

    while(1)
    {

    }
}
