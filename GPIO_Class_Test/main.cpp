/*
 * main.cpp
 *
 *    Author: Max Zuidberg
 *     Email: m.zuidberg@icloud.com
 *
 *  Test program for the GPIO class. The on-board switch SW1 is used to change
 *  the state of the red on-board LED.
 */

/*
 * GPIO.h:   Header file for the GPIO class
 * System.h: Header file for the System class (needed for example for
 *           clock settings)
 */
#include "GPIO.h"
#include "System.h"


int main(void)
{
    // Construct all objects
    System system;
    GPIO redLed, sw1;

    // Start system and run at 40 MHz.
    system.init(40000000);

    // Red LED on pin PF1
    redLed.init(&system, GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);

    // Switch SW1 on pin PF4
    sw1.init(&system, GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);

    // Enable pull-up on SW1
    sw1.setPullup(true);

    while (1)
    {
        /*
         * Template for debouncing contacts:
         * if (button pressed)
         * {
         *     delay(50ms);
         *     while (button pressed);
         *     do something;
         *     delay(50ms);
         * }
         */

        // Wait until button1 is pressed and released again.
        if (!sw1.read())
        {
            system.delayUS(50000);
            while (!sw1.read());

            // Toggle redLed
            redLed.write(!redLed.read());

            system.delayUS(50000);
        }
    }
}
