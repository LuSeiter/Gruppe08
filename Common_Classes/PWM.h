#ifndef PWM_H_
#define PWM_H_

#include <stdbool.h>
#include <stdint.h>
#include "System.h"
#include "Config.h"
#include "GPIO.h"


class PWM
{
public:
    PWM();
    virtual ~PWM();
    void init(System *sys, GPIO *dir, uint32_t portBase, uint32_t pin, bool invert = false, uint32_t freq = 5000);
    void setFreq(uint32_t freq);
    void setDuty(float duty);

private:
    System *sys;
    GPIO *dir;
    uint32_t base, gen, clockFreq,
             out, outBit;
    const uint32_t PWM_PINS             = 0;
    const uint32_t SYSCTL_PORT          = 1;
    const uint32_t PIN_CONFIG           = 2;
    const uint32_t PWM_PINOUT_COUNT     = 16;
    const uint32_t PWM_PIN_MAPPING[16][3] = {
         {GPIO_PORTB_BASE | GPIO_PIN_6, SYSCTL_PERIPH_GPIOB, GPIO_PB6_M0PWM0},
         {GPIO_PORTB_BASE | GPIO_PIN_7, SYSCTL_PERIPH_GPIOB, GPIO_PB7_M0PWM1},
         {GPIO_PORTB_BASE | GPIO_PIN_4, SYSCTL_PERIPH_GPIOB, GPIO_PB4_M0PWM2},
         {GPIO_PORTB_BASE | GPIO_PIN_5, SYSCTL_PERIPH_GPIOB, GPIO_PB5_M0PWM3},
         {GPIO_PORTE_BASE | GPIO_PIN_4, SYSCTL_PERIPH_GPIOE, GPIO_PE4_M0PWM4},
         {GPIO_PORTE_BASE | GPIO_PIN_5, SYSCTL_PERIPH_GPIOE, GPIO_PE5_M0PWM5},
         {GPIO_PORTC_BASE | GPIO_PIN_4, SYSCTL_PERIPH_GPIOC, GPIO_PC4_M0PWM6},
         {GPIO_PORTC_BASE | GPIO_PIN_5, SYSCTL_PERIPH_GPIOC, GPIO_PC5_M0PWM7},
         {GPIO_PORTD_BASE | GPIO_PIN_0, SYSCTL_PERIPH_GPIOD, GPIO_PD0_M1PWM0},
         {GPIO_PORTD_BASE | GPIO_PIN_1, SYSCTL_PERIPH_GPIOD, GPIO_PD1_M1PWM1},
         {GPIO_PORTA_BASE | GPIO_PIN_6, SYSCTL_PERIPH_GPIOA, GPIO_PA6_M1PWM2},
         {GPIO_PORTA_BASE | GPIO_PIN_7, SYSCTL_PERIPH_GPIOA, GPIO_PA7_M1PWM3},
         {GPIO_PORTF_BASE | GPIO_PIN_0, SYSCTL_PERIPH_GPIOF, GPIO_PF0_M1PWM4},
         {GPIO_PORTF_BASE | GPIO_PIN_1, SYSCTL_PERIPH_GPIOF, GPIO_PF1_M1PWM5},
         {GPIO_PORTF_BASE | GPIO_PIN_2, SYSCTL_PERIPH_GPIOF, GPIO_PF2_M1PWM6},
         {GPIO_PORTF_BASE | GPIO_PIN_3, SYSCTL_PERIPH_GPIOF, GPIO_PF3_M1PWM7}
    };
};

#endif /* PWM_H_ */
