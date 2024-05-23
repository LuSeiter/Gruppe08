// Uncomment following #define to use the precompiled Timer library instead of
// the code in this file.
// #define USE_TIMER_LIBRARY

#ifndef USE_TIMER_LIBRARY


#include <Timer.h>


Timer::Timer() // @suppress("Class members should be properly initialized")
{

}

Timer::~Timer() // @suppress("Class members should be properly initialized")
{

}

void Timer::init(System* sys, uint32_t base, void (*ISR)(void), uint32_t freq)
{
    /*
     * Initialize timer A with an ISR in a given timer module.
     * sys:     Pointer to the current System instance. Needed to get CPU clock
     *          frequency.
     * base:    base constant of the timer module. (f.ex. TIMER0_BASE)
     * ISR:     ISR to be called at timeout.
     */

    // Store pointer to the given System object.
    this->sys = sys;

    // Save the timer base.
    this->base = base;

    // Number of the timer module used.
    // Note: TIMERx_BASE = TIMER0_BASE + x * 0x1000 (hw_memmap.h line 81-86)
    number = (base - TIMER0_BASE) / 0x1000;

    /*
     * Check if the base address is valid.
     * Even an incorrect base address can, because of round-off errors, give a
     * correct timerNumber. However in that case, the following baseCheck
     * would result in a different (valid) value.
     * Note: as timerNumber is an unsigned integer, (incorrect) values that
     *       would be < 0 result in values >> 5 (wrap-around). Therefore only a
     *       check against the upper limit of 5 is needed.
     */
    uint32_t baseCheck = TIMER0_BASE + number * 0x1000;
    if (number > 5 || base != baseCheck)
    {
        sys->error(TimerWrongConfig, &base);
    }

    // Enable and provide clock to the right timer module based on the timer 
    // Number and the array with the corresponding constants
    SysCtlPeripheralEnable(SYSCTL_MAPPING[number]);

    // Wait until peripheral is enabled "TivaWare(TM) Treiberbibliothek"
    // page 502, "TivaC Launchpad Workshop" page 78)
    sys->delayCycles(5);

    // Configure timer A to periodic mode.
    TimerConfigure(base, TIMER_CFG_PERIODIC);

    // Make sure the timer is not running yet.
    stop();

    // Apply optional frequency parameter.
    setFreq(freq);

    // Configure the timer interrupt and the interrupt handling.
    TimerIntEnable(base, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(base, TIMER_A, ISR);
}

void Timer::start()
{
    /*
     * Enable this timer.
     */

    TimerEnable(base, TIMER_A);
}

void Timer::stop()
{
    /*
     * Disable this timer and clear any pending interrupt flags.
     */

    TimerDisable(base, TIMER_A);
    clearInterruptFlag();
}

void Timer::clearInterruptFlag()
{
    /*
     * Clear the timer timeout interrupt flag. This should be the first thing
     * you call in your ISR.
     */

    TimerIntClear(base, TIMER_TIMA_TIMEOUT);
}

void Timer::setPeriodUS(uint32_t periodUS)
{
    /*
     * Set the timer to the given period.
     *
     * periodUS: period in us. A value of 0 stops the timer.
     */

    if (periodUS)
    {
        // Save the given value in the object.
        this->periodUS = periodUS;
        this->freq = 1000000 / periodUS;

        // Calculate the timer load value.
        loadValue = periodUS * (sys->getClockFreq() / 1000000) - 1;

        // Apply values.
        TimerLoadSet(base, TIMER_A, loadValue);
    }
    else
    {
        // Stop the timer.
        this->periodUS = 0;
        this->freq = 0;
        stop();
    }
}

void Timer::setFreq(uint32_t freq)
{
    /*
     * Set the timer to the given frequency.
     *
     * freq: frequency in Hz. A value of 0 stops the timer.
     */

    if (freq)
    {
        // Save the given value in the object.
        this->freq = freq;
        this->periodUS = 1000000 / freq;

        // Calculate the timer load value.
        loadValue = sys->getClockFreq() / freq - 1;

        // Apply values.
        TimerLoadSet(base, TIMER_A, loadValue);
    }
    else
    {
        // Stop the timer.
        this->periodUS = 0;
        this->freq = 0;
        stop();
    }
}

uint32_t Timer::getPeriodUS()
{
    /*
     * Return the current period of the timer in us.
     */

    return periodUS;
}

uint32_t Timer::getFreq()
{
    /*
     * Return the current frequency in Hz of the timer.
     */

    return freq;
}


#endif
