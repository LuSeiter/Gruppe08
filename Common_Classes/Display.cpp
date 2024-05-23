#include "Display.h"

Display::Display() // @suppress("Class members should be properly initialized")
{

}

Display::~Display() // @suppress("Class members should be properly initialized")
{

}
void Display::init(System *sys)
{
    /*
     * Initializes the GPIO Pins to control the display and configures the display.
     * System *sys: Pointer to the current system instance
     */
    // Creates private reference to the current system instance
    this->displaySys = sys;

    // Configures all needed GPIO Pins to control the display
    r_w.init(sys, GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_DIR_MODE_OUT, false);
    enablePin.init(sys, GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_DIR_MODE_OUT, false);
    registerSelect.init(sys, GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT, false);

    data4.init(sys, GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT, false);
    data5.init(sys, GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT, false);
    data6.init(sys, GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT, false);
    data7.init(sys, GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT, false);

    // Configures the display with the desired configuration
    sendByte(0b00000010, false);    // 4-bit data length
    sendByte(0b00101000, false);    // 4-bit data length, 4-line display, 5x7 font
    sendByte(0b00001111, false);    // display on, cursor on, blinking cursor
    clearDisplay();                 // clears display
    sendByte(0b00000110, false);    // cursor auto increment

}
void Display::sendByte(uint8_t data, bool isData)
{
    /*
     * Writes the data provided in the argument. The writing process differs whether it is data or configuration
     *
     * uint8_t data: 8 bit value to be written to the display
     * bool isData: Defines if data is data or configuration
     */
    data4.write(data & 0b00010000);
    data5.write(data & 0b00100000);
    data6.write(data & 0b01000000);
    data7.write(data & 0b10000000);

    registerSelect.write(isData);
    enable();

    data4.write(data & 0b00000001);
    data5.write(data & 0b00000010);
    data6.write(data & 0b00000100);
    data7.write(data & 0b00001000);
    registerSelect.write(isData);
    enable();

}
void Display::enable()
{
    /*
     * enables the data input with a ms pulse
     */
    enablePin.write(true);
    this->displaySys->delayUS(1000);
    enablePin.write(false);
    this->displaySys->delayUS(1000);
}

void Display::clearDisplay()
{
    /*
     * deletes all characters display on the display
     */

    sendByte(1, false);
}
void Display::resetCursor()
{
    // Resets the cursor to its origin
    sendByte(2, false);
}
void Display::cursorPosition(uint8_t column, uint8_t row)
{
    /*
     * Sets the cursor to the desired position provided in the argument
     * uint8_t column: Specifies the desired column
     * uint8_t row: Specifies the desired row
     *
     * 1 -> + 0
     * 2 -> + 1000000b
     * 3 -> + 10100b
     * 4 -> + 1010100b
     *
     */
    if(row == 1)
    {
        sendByte(0b10000000 + column - 1, false);   // DD RAM address 0x00 - 0x13
    }
    else if(row == 2)
    {
        sendByte(0b11000000 + column - 1, false);   // DD RAM address 0x40 - 0x53
    }
    else if(row == 3)
    {
        sendByte(0b10010100 + column - 1, false);   // DD RAM address 0x14 - 0x27
    }
    else if(row == 4)
    {
        sendByte(0b11010100 + column - 1, false);   // DD RAM address 0x54 - 0x67
    }
}

void Display::printText(char text)
{
    /*
     * Send the desired char to the display
     */
    sendByte(text, true);
}

void Display::printText(const char *text)
{
    /*
     * Sends a whole char array to the display at once
     * const char *text: Pointer to the char array
     */

    for(int i=0;i<strlen(text);i++)
    {
        sendByte(text[i], true);
    }
}

void Display::printNumber(int16_t number)
{
    /*
     * Send a 16 bit integer number to the display
     */
    if(number == 0)
    {
        sendByte('0', true);
        return;
    }
    if(number < 0)
    {
        sendByte('-',true);
        number = -number;
    }
    int8_t firstDigit = 0;
    uint16_t tens = 10000;
    for(int i = 4;i>=0;i--)
    {
        int8_t currentDigit = number / tens;
        number -= currentDigit * tens;
        firstDigit += currentDigit;
        if(firstDigit > 0)
        {
            sendByte('0' + currentDigit, true);
        }
        tens = tens / 10;
    }
}

void Display::printFloat(float number)
{
    /*
     * Prints a float to the display
     */
    uint8_t integerPart = (uint8_t) number;
    uint8_t commaPart = (number - integerPart) * 100;
    printNumber(integerPart);
    printText(".");
    printNumber(commaPart);
}

void Display::clearSegment(uint8_t length)
{
    /*
     * Clears a specific segment
     */
    for(int i=0;i<length;i++)
    {
        printText(" ");
    }
}
