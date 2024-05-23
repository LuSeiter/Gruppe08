// Uncomment following #define to use the precompiled ADC library instead of
// the code in this file.
//#define USE_UART_LIBRARY

#ifndef USE_UART_LIBRARY

#include "UART.h"

UART::UART() // @suppress("Class members should be properly initialized")
{

}

UART::~UART() // @suppress("Class members should be properly initialized")
{

}
void UART::init(System *sys, uint32_t portBase, uint32_t tx, uint32_t rx)
{
    /*
     * System *sys: pointer to current system instance
     * uint32_t portBase: GPIO port address of desired UART module
     * uint32_t tx: GPIO pin used by the UART module to send
     * uint32_t rx: GPIO pin used by the UART module to receive
     */
    /*
     * The goal of this method is to enable and configure the UART and GPIO module used for the communication.
     * First, figure out which UART module should be configured based on the GPIO Port/Pin Combination given in the
     * argument. After that, enable the UART module and GPIO module, configure the GPIO pins as UART pins, and set the
     * UART configuration.
     * Desired Configuration:
     * Word Length = 8
     * Stop Bit = 1
     * Parity Bit = None
     * Lastly, enable the FIFOs.
     */

    this->sys = sys;

    //TODO: Put your code here -> A4.1
}

void UART::send(const char *transmit, uint8_t length)
{
    /*
     * Sends the message that is given in the argument
     *
     * *transmit: Pointer to the char array which stores the message
     * length: Length of the char array
     */

    //TODO: Put your code here -> A4.1
}

bool UART::checkAvailability(){
    if(UARTCharsAvail(this->base))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UART::receive(char *receive, uint8_t length)
{
    /*
     * The goal of the receive method is reading the receive FIFO, decoding the incoming message by removing start/stop
     * and parity bit(s) and searching the decoded message for the synchronization word (view Config.h).
     * First, all incoming messages are being trashed until the synchronization word is found.
     * After that, the message is written in the receive array provided in the argument.
     * If the message is too short the method stops and returns false.
     * If the message is too long the method clears the rest of the FIFO until it's empty and returns false.
     * If the message is as long as expected the method returns true.
     */

    //TODO: Put your code here -> A4.1
}
#endif
