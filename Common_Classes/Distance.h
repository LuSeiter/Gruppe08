#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "System.h"
#include "GPIO.h"

class Distance
{
public:
    Distance();
    virtual ~Distance();
    void init(System *sys, uint32_t echoPort, uint32_t echoPin, uint32_t triggerPort, uint32_t triggerPin, void (*ISR)(void));
    void trigger();
    void edgeDetect();
    float getDistance();
private:
    System *distanceSys;
    GPIO triggerObject;
    uint32_t echoPort, echoPin, timerBase, timerType, timerIntType;
    uint32_t firstEdge,secondEdge;
    float distance = 256;
    bool edge = false;
    uint32_t config[12][4] = {{GPIO_PORTC_BASE, GPIO_PIN_4, WTIMER0_BASE, GPIO_PC4_WT0CCP0},{GPIO_PORTC_BASE, GPIO_PIN_5, WTIMER0_BASE, GPIO_PC5_WT0CCP1},
                              {GPIO_PORTC_BASE, GPIO_PIN_6, WTIMER1_BASE, GPIO_PC6_WT1CCP0},{GPIO_PORTC_BASE, GPIO_PIN_7, WTIMER1_BASE, GPIO_PC7_WT1CCP1},
                              {GPIO_PORTD_BASE, GPIO_PIN_0, WTIMER2_BASE, GPIO_PD0_WT2CCP0},{GPIO_PORTD_BASE, GPIO_PIN_1, WTIMER2_BASE, GPIO_PD1_WT2CCP1},
                              {GPIO_PORTD_BASE, GPIO_PIN_2, WTIMER3_BASE, GPIO_PD2_WT3CCP0},{GPIO_PORTD_BASE, GPIO_PIN_3, WTIMER3_BASE, GPIO_PD3_WT3CCP1},
                              {GPIO_PORTD_BASE, GPIO_PIN_4, WTIMER4_BASE, GPIO_PD4_WT4CCP0},{GPIO_PORTD_BASE, GPIO_PIN_5, WTIMER4_BASE, GPIO_PD5_WT4CCP1},
                              {GPIO_PORTD_BASE, GPIO_PIN_6, WTIMER5_BASE, GPIO_PD6_WT5CCP0},{GPIO_PORTD_BASE, GPIO_PIN_7, WTIMER5_BASE, GPIO_PD7_WT5CCP1}};
};




#endif /* DISTANCE_H_ */
