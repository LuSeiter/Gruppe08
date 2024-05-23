#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "System.h"
#include "GPIO.h"
#include <cstring>

class Display
{
public:
    Display();
    virtual ~Display();
    void init(System *sys);
    void sendByte(uint8_t data, bool isdata);
    void enable();
    void clearDisplay();
    void resetCursor();
    void cursorPosition(uint8_t coloum, uint8_t row);
    void printText(const char text);
    void printText(const char *text);
    void printNumber(int16_t number);
    void printFloat(float number);
    void clearSegment(uint8_t length);
private:
    System *displaySys;
    GPIO enablePin,registerSelect,r_w,data4,data5,data6,data7;
};



#endif /* DISPLAY_H_ */
