#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifndef PART_TM4C123GH6PM
    #define PART_TM4C123GH6PM
#endif

#include <stdbool.h>
#include <stdint.h>
#include <algorithm>
#include <string.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "uartstdio.h"
#include "ErrorCodes.h"
#include "driverlib/pwm.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "Config.h"


class System
{
public:
    System();
    virtual ~System();
    void init(uint32_t clk);
    void error(ErrorCodes ErrorCode = UnknownError,
               void *faultOrigin0 = 0,
               void *faultOrigin1 = 0,
               void *faultOrigin2 = 0);
    void enableFPU();
    void setPWMClockDiv(uint32_t div);
    uint32_t getClockFreq();
    uint32_t getPWMClockDiv();
    void delayCycles(uint32_t cycles);
    void delayUS(uint32_t us);
    void setDebugging(bool debug);
    void setDebugVal(const char* name, int32_t value);
    void sendDebugVals();
private:
    bool debugEnabled = true;
    bool debugNewLabel = false;
    const static uint32_t maxDebugVals = 8;
    const char debugUnused[1] = "";
    int32_t debugVals[maxDebugVals];
    const char* debugNames[maxDebugVals] = {
        debugUnused,
        debugUnused,
        debugUnused,
        debugUnused,
        debugUnused,
        debugUnused,
        debugUnused,
        debugUnused
    };
    bool tooManyDebugVals = false;

    uint32_t clockFrequency = 0;
    uint32_t pwmClockDiv = 0;

    // All PWM Clock dividors
    const uint32_t PWM_CLOCK_DIV_COUNT = 8;
    const uint32_t PWM_CLOCK_DIV_MAPPING[8] = {
        SYSCTL_PWMDIV_1,
        SYSCTL_PWMDIV_2,
        SYSCTL_PWMDIV_4,
        SYSCTL_PWMDIV_8,
        SYSCTL_PWMDIV_16,
        SYSCTL_PWMDIV_32,
        SYSCTL_PWMDIV_64,
    };

    // All peripherals of the uC
    const uint_fast8_t PERIPH_COUNT = 49;
    const uint32_t ALL_PERIPHS[49] = {
        SYSCTL_PERIPH_WDOG0,
        SYSCTL_PERIPH_WDOG1,
        SYSCTL_PERIPH_TIMER0,
        SYSCTL_PERIPH_TIMER1,
        SYSCTL_PERIPH_TIMER2,
        SYSCTL_PERIPH_TIMER3,
        SYSCTL_PERIPH_TIMER4,
        SYSCTL_PERIPH_TIMER5,
        SYSCTL_PERIPH_GPIOA,
        SYSCTL_PERIPH_GPIOB,
        SYSCTL_PERIPH_GPIOC,
        SYSCTL_PERIPH_GPIOD,
        SYSCTL_PERIPH_GPIOE,
        SYSCTL_PERIPH_GPIOF,
        SYSCTL_PERIPH_UDMA,
        SYSCTL_PERIPH_HIBERNATE,
        SYSCTL_PERIPH_UART0,
        SYSCTL_PERIPH_UART1,
        SYSCTL_PERIPH_UART2,
        SYSCTL_PERIPH_UART3,
        SYSCTL_PERIPH_UART4,
        SYSCTL_PERIPH_UART5,
        SYSCTL_PERIPH_UART6,
        SYSCTL_PERIPH_UART7,
        SYSCTL_PERIPH_SSI0,
        SYSCTL_PERIPH_SSI1,
        SYSCTL_PERIPH_SSI2,
        SYSCTL_PERIPH_SSI3,
        SYSCTL_PERIPH_I2C0,
        SYSCTL_PERIPH_I2C1,
        SYSCTL_PERIPH_I2C2,
        SYSCTL_PERIPH_I2C3,
        SYSCTL_PERIPH_USB0,
        SYSCTL_PERIPH_CAN0,
        SYSCTL_PERIPH_CAN1,
        SYSCTL_PERIPH_ADC0,
        SYSCTL_PERIPH_ADC1,
        SYSCTL_PERIPH_COMP0,
        SYSCTL_PERIPH_PWM0,
        SYSCTL_PERIPH_PWM1,
        SYSCTL_PERIPH_QEI0,
        SYSCTL_PERIPH_QEI1,
        SYSCTL_PERIPH_EEPROM0,
        SYSCTL_PERIPH_WTIMER0,
        SYSCTL_PERIPH_WTIMER1,
        SYSCTL_PERIPH_WTIMER2,
        SYSCTL_PERIPH_WTIMER3,
        SYSCTL_PERIPH_WTIMER4,
        SYSCTL_PERIPH_WTIMER5
    };
};


#endif /* SYSTEM_H_ */
