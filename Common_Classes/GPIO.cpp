#include "GPIO.h"


GPIO::GPIO() // @suppress("Class members should be properly initialized")
{

}

GPIO::~GPIO() // @suppress("Class members should be properly initialized")
{

}

void GPIO::init(System* sys, uint32_t portBase, uint32_t pin, uint32_t dir, bool pullup)
{
    /*
     * Initializes a GPIO-pin object by enabling the corresponding peripheral
     * (the port which contains the pin), and configuring the pin as input or
     * output.
     *
     * sys:      Pointer to the current System instance. Needed for error
     *           handling.
     * portBase: base address of the GPIO port. Use the constants provided by
     *           the TivaWare API (inc/hw_memmap.h line 53-56, 73-76).
     * pin:      pin number in the GPIO port. Use the constants provided by
     *           the TivaWare API (driverlib/gpio.h line 60-67).
     * dir:      indicates whether it's an input or an output pin. Use the
     *           constants provided by the TivaWare API (driverlib/gpio.h
     *           line 75-77).
     * pullUp:   Optional bool parameter to enable or disable the internal weak
     *           pull-up. Default is to let it disabled.
     */

    // Save system, port, pin and direction
    this->sys = sys;
    this->portBase = portBase;
    this->pin = pin;
    this->dir = dir;

    // The default current and pin type (also see gpio.c line 1552-1614)
    current = GPIO_STRENGTH_2MA;
    pinType = GPIO_PIN_TYPE_STD;

    /*
     * Enable the peripheral (here: a GPIO port). This can be redone for other
     * pins in the same port, as only one bit is set in a register. Therefore
     * no need to check whether the bit is already set (simpler and faster
     * without unnecessary check).
     *
     * Note: In this class sometimes a mapping of values is needed. For example
     *       in this case the base address of the port must be mapped to the
     *       correct system control constant. This can be done in different 
     *       ways. For educational purposes, this class uses three different
     *       methods: switch case, if else, array. More details should be given
     *       in the student's manual.
     */
    switch (portBase)
    {
    case GPIO_PORTA_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        break;
    case GPIO_PORTB_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        break;
    case GPIO_PORTC_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        break;
    case GPIO_PORTD_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        break;
    case GPIO_PORTE_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        break;
    case GPIO_PORTF_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        break;
    default:
        sys->error(GPIOWrongConfig, &portBase, &pin, &dir);
    }

    // Wait until peripheral is enabled "TivaWare(TM) Treiberbibliothek"
    // page 502, "TivaC Launchpad Workshop" page 78)
    sys->delayCycles(5);

    // Configure pin as input or output.
    GPIODirModeSet(portBase, pin, dir);

    // Apply pull-up parameter. This also calls GPIOPadConfigSet needed by
    // GPIODirModeSet (see "TivaWare(TM) Treiberbibliothek" page 253).
    setPullup(pullup);
}

bool GPIO::read()
{
    /*
     * Reads the pin associated with this GPIO object and returns that value.
     */

    /*
     * GPIOPinRead reads the whole port, masked by the pins we want to read.
     * This value is non-zero (which means true) if the pin is set, and 0
     * (which means false) if not.
     * Notes:
     *  - The & 0xff is necessary because the upper 24 bits are _not_ masked
     *    by GPIOPinRead. This has to do with how GPIO masking is done in
     *    hardware.
     *  - This trick only works if the reading is (implicitly) converted to
     *    a bool. if (read == true) does not work because "true" is defined
     *    as 1 and (f.ex.) 0b00010000 != 1. If you want to use an if, you
     *    need to omit the == true or type cast the reading to a bool.
     *    However all of this is unnecessarily complex.
     * Example: GPIO port A state: 0b01001110
     *          - GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) returns
     *            0b00001000 != 0 which means "true"
     *          - GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) returns 0b00000000
     *            which means "false"
     */
    return (GPIOPinRead(portBase, pin) & 0xff);
}

void GPIO::write(bool state)
{
    /*
     * Writes a bit to the pin associated with this object.
     * state: the desired value for the pin
     */

    /*
     * GPIOPinWrite writes the indicated bits of a given value to the GPIO port
     * (see "TivaWare(TM) Treiberbibliothek" page 280), thus manipulates only
     * the indicated pins corresponding to that value.
     * Here we only want to set one pin to a given state. Therefore we give
     * only that pin as mask, and our value contains only the bit for that pin.
     */
    GPIOPinWrite(portBase, pin, (state * pin));
}

uint32_t GPIO::getCurrent()
{
    /*
     * Returns the maximum current (in mA) this GPIO pin can deliver.
     */

    /*
     * The parameter stored in gpioCurrent (see "TivaWare(TM)
     * Treiberbibliothek" page 260) needs to be converted to the
     * corresponding mA value.
     */
    if (current == GPIO_STRENGTH_2MA)
    {
        return 2;
    }
    else if (current == GPIO_STRENGTH_4MA)
    {
        return 4;
    }
    else if (current == GPIO_STRENGTH_6MA)
    {
        return 6;
    }
    else if (current == GPIO_STRENGTH_8MA)
    {
        return 8;
    }
    else if (current == GPIO_STRENGTH_10MA)
    {
        return 10;
    }
    else if (current == GPIO_STRENGTH_12MA)
    {
        return 12;
    }
    else
    {
        return 0;
    }
}

void GPIO::setCurrent(uint32_t current)
{
    /*
     * Sets the maximum current of the pin associated with this object.
     * current: desired maximum current in mA for this pin.
     */

    // Check if current is valid (even value between 2 and 12)
    if ((current % 2 != 0) || (current < 2) || (current > 12))
    {
        sys->error(GPIOWrongCurrent, &current);
    }

    /*
     * As the current is given in mA it still needs to be converted to the
     * right parameter (see "TivaWare(TM) Treiberbibliothek" page 260).
     * Because currents are given in steps of 2mA from 2mA to 12mA, this can
     * easily be done with an array, using half the current as index.
     */
    current = HALF_CURRENT_TO_PARAM[current / 2];
    refreshConfig();
}

void GPIO::setPullup(bool enabled)
{
    /*
     * Enable or disable the internal weak pull-up resistor for this pin.
     *
     * enabled: 0: deactivate pull-up
     *          1: activate pull-up
     */

    if (enabled)
    {
        pinType = GPIO_PIN_TYPE_STD_WPU;
    }
    else
    {
        pinType = GPIO_PIN_TYPE_STD;
    }
    refreshConfig();
}

void GPIO::setPulldown(bool enabled)
{
    /*
     * Enable or disable the internal weak pull-down resistor for this pin.
     *
     * enabled: 0: deactivate pull-up
     *          1: activate pull-up
     */

    if (enabled)
    {
        pinType = GPIO_PIN_TYPE_STD_WPD;
    }
    else
    {
        pinType = GPIO_PIN_TYPE_STD;
    }
    refreshConfig();
}

void GPIO::refreshConfig()
{
    /*
     * Calls GPIOPadConfig with the current object variables. Usually called
     * after changing these to apply the new values.
     */

    GPIOPadConfigSet(portBase, pin, current, pinType);
}
