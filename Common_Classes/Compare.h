#ifndef COMPARE_H_
#define COMPARE_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "System.h"
#include "Config.h"


class Compare
{
public:
    Compare();
    virtual ~Compare();
    void init(System *sys, uint32_t port, uint32_t pin, void (*ISR)(void));
    void start();
    void stop();
    void clearInterruptFlag(uint32_t flag);
    void calcFreq();
    float getFreq();
    void ISR();
    float getRPM();
private:
    System *sys;
    uint32_t base,typeCap, capFlag, ctlFlag, counter, firstEdge, secondEdge, gpioBase, gpioPin;
    uint32_t tempHalf = 0;
    float freq;
    uint32_t config[12][2] = {{GPIO_PORTB_BASE | GPIO_PIN_6, GPIO_PB6_T0CCP0},
                                {GPIO_PORTB_BASE | GPIO_PIN_7, GPIO_PB7_T0CCP1},
                                {GPIO_PORTB_BASE | GPIO_PIN_4, GPIO_PB4_T1CCP0},
                                {GPIO_PORTB_BASE | GPIO_PIN_5, GPIO_PB5_T1CCP1},
                                {GPIO_PORTB_BASE | GPIO_PIN_0, GPIO_PB0_T2CCP0},
                                {GPIO_PORTB_BASE | GPIO_PIN_1, GPIO_PB1_T2CCP1},
                                {GPIO_PORTB_BASE | GPIO_PIN_2, GPIO_PB2_T3CCP0},
                                {GPIO_PORTB_BASE | GPIO_PIN_3, GPIO_PB3_T3CCP1},
                                {GPIO_PORTC_BASE | GPIO_PIN_0, GPIO_PC0_T4CCP0},
                                {GPIO_PORTC_BASE | GPIO_PIN_1, GPIO_PC1_T4CCP1},
                                {GPIO_PORTC_BASE | GPIO_PIN_2, GPIO_PC2_T5CCP0},
                                {GPIO_PORTC_BASE | GPIO_PIN_3, GPIO_PC3_T5CCP1}};
};


#endif /* COMPARE_H_ */
