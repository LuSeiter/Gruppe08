#ifndef ADC_H_
#define ADC_H_


/*
 * stdbool.h:               Boolean definitions for the C99 standard
 * stdint.h:                Variable definitions for the C99 standard
 * inc/hw_types.h:          Macros for hardware access, both direct and via the
 *                          bit-band region.
 * inc/hw_memmap.h:         Macros defining the memory map of the Tiva C Series
 *                          device. This includes defines such as peripheral
 *                          base address locations such as GPIO_PORTF_BASE.
 * inc/hw_ints.h:           Macros defining the interrupt assignments
 * driverlib/sysctl.h:      Defines and macros for the System Control API of
 *                          DriverLib. This includes API functions such as
 *                          SysCtlClockSet.
 * driverlib/gpio.h:        Defines and macros for GPIO API of DriverLib. This
 *                          includes API functions such as GPIOPinConfigure.
 * driverlib/adc.h          Definitions for using the ADC driver
 * System.h:                Access to current CPU clock and other functions.
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "System.h"
#include "Config.h"


// Defines for readability using adcInputPinMapping
#define AIN_PERIPH  0
#define AIN_PORT    1
#define AIN_PIN     2


class ADC
{
public:
    ADC();
    virtual ~ADC();
    void init(System *sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput);
    void setHWAveraging(uint32_t averaging);
    uint32_t read();
    float readVolt();
private:
    uint32_t base, sampleSeq, input, reading[1];
    const uint32_t INPUT_PIN_MAPPING[12][3] =
    {{SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_3},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_1},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_0},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_3},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_2},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_1},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_0},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_5},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_4},
     {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE, GPIO_PIN_4},
     {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE, GPIO_PIN_5}};
};


#endif /* ADC_H_ */
