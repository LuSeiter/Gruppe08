

/*
 * Author: Robin Rau
 *
 * Test program for the UART Communication with the remote Control. The characters are sent using UART (see System.cpp) every 1s and received every 1s.
 * For better user understanding, the characters sent represent the integer values 0 to 11.
 * To ensure that the bytes are in the right order, after each read the program checks whether the package is too short or too long. If either case
 * exists, the transmission wasn't correct and the status is set to false (-> On-Board LED is turned off).
 * If the transmission has the correct length, the status is true and the LED is turned on.
 * The stability of the transmission can therefore be seen via the on-board LED.
 * Set the UART Settings in the Config.h !
 */


#include "System.h"
#include "Timer.h"
#include "GPIO.h"
#include "UART.h"



Timer timer1;
UART communication;

bool status = true;
char receiveArray[12] = {};

void ISR()
{
    //TODO: Put your code here -> A4.2

}

void main(void)
{
    System sys;
    GPIO led1;

    sys.init(CFG_SYS_FREQ);
    sys.enableFPU();

    //TODO: Put your code here -> A4.2



    while(1)
    {
        //TODO: Put your code here -> A4.2

    }
}
