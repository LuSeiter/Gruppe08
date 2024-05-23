#ifndef MAGNI_H_
#define MAGNI_H_

#include "System.h"
#include "PWM.h"
#include "GPIO.h"
#include "Config.h"
#include "ErrorCodes.h"
#include "Compare.h"
#include "ADC.h"
#include "Timer.h"
#include "Distance.h"
#include "UART.h"
#include <math.h>

#define COMBINED        0
#define PWMLEFT         1
#define PWMRIGHT        2
#define RPMLEFT         3
#define RPMRIGHT        4
#define CURRENTLEFT     5
#define CURRENTRIGHT    6
#define ERROR           7
#define SPEED           8
#define BATTERY         9
#define BATTERYCOMMA    10
#define SENSORDIR       11
#define DISTANCE        12


class Magni
{
public:
    Magni();
    virtual ~Magni();
    void init(System *sys, void (*rpmLeftISR)(void), void (*rpmRightISR)(void), void (*sensor1ISR)(void), void (*sensor2ISR)(void), void (*sensor3ISR)(void), void (*sensor4ISR)(void), void (*sensor5ISR)(void));
    void update();
    void sensorISR1();
    void background();
    void getTransmitWords();
    void rpmISR1();
    void rpmISR2();
    Compare rpmLeft, rpmRight;
    Distance sensor0, sensor1, sensor2, sensor3, sensor4;
private:
    uint8_t bool2Byte(bool *values);
    System *magniSys;
    UART communication;
    PWM motorLeft, motorRight;
    GPIO enable, brakeLeft, brakeRight, faultLeft, faultRight, faultCLR, dirLeft, dirRight, blueLED, redLED;
    ADC motorLeftCurrent, motorRightCurrent, battery;
    char receiveWords[3] = {0x00, 0x00, 0x00};
    bool standby = false, updateFlag = false, measureStarted = false, enableStatus = false, status = false, sensor = true;
    char transmitWords[13] = "ABCDEFGHIJ";
    float dutyLeft = 0.0f, dutyRight = 0.0f, minimal_distance = 255.0f, speed = 1.0f;
    uint8_t joyStickVert = 0, joyStickHori = 0, sensor_dir = 0, batteryCounter = 0, uartCounter = 0, timeOut = 0,distanceMid = 0, distanceCounter = 0;
    uint8_t lastDistances[5] = {255,255,255,255,255};
};



#endif /* MAGNI_H_ */
