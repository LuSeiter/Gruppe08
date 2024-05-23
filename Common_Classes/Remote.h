#ifndef REMOTE_H_
#define REMOTE_H_


#include "System.h"
#include "Display.h"
#include "MPU6050.h"
#include "Config.h"
#include "GPIO.h"
#include "Steering.h"
#include "UART.h"
#include <cmath>

class Remote
{
public:
    Remote();
    virtual ~Remote();
    void init(System *sys);
    void updateDisplay();
    void update();
    void setTransmitValues();
private:
    System *remoteSys;
    GPIO sw1,sw2,sw3,sw4,esp,onboard;
    Steering wheel;
    MPU6050 sensor;
    UART communication;
    Display display;
    uint8_t menuNumber = 0;
    bool updateFlag = false;
    uint8_t joyCounter = 0;
    float joyMiddleX = 0, joyMiddleY = 0,joyOldValueX = 0, joyOldValueY = 0;
    int16_t pwm = 0;
    char transmit[CFG_BYTE_PER_MESSAGE_SENT+1] = {0,0,0,0}; //before 0,1,2,3 -> 0000 0000b, 0000 0001b, 0000 0010b, 0000 0011b ->We keep the one in second byte as we only do Bit manipulations
    char received[CFG_BYTE_PER_MESSAGE_REC] = {0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t cursorPos = 2;
    bool engineEnable = false;
    uint8_t uartCounter = 0;
    bool errorThrow = false;
};



#endif /* REMOTE_H_ */
