#include <System.h>


System::System() // @suppress("Class members should be properly initialized")
{

}

System::~System()
{

}

void System::init(uint32_t clk)
{
    /*
     * Initialize the uC by setting and enabling the CPU clock, the PWM Unit
     * Clock Divisor, unlocking all available GPIO pins and allowing the
     * processor to respond to interrupts.
     * CPU clock frequency and PWM Unit Clock Divisor can be read with the
     * corresponding methods.
     * Note: Call this method before doing anything else in your program!
     *
     * clk: the desired clock frequency of the CPU in Hz. It can be 40MHz,
     *      50MHz or 80MHz.
     */

    // Configure clock ("TivaC Launchpad Workshop" page 75)
    if (clk == 40000000)
    {
        SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                       | SYSCTL_OSC_MAIN);
    }
    else if (clk == 50000000)
    {
        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                       | SYSCTL_OSC_MAIN);
    }
    else if (clk == 80000000)
    {
        SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                       | SYSCTL_OSC_MAIN);
    }
    else
    {
        error(SysWrongFrequency, &clk);
    }

    // Store the CPU clock
    clockFrequency = clk;

    /*
     * Set the clock divisor which applies to all PWM modules ("TivaWare(TM)
     * Treiberbibliothek" page 509 and "TivaC Mikrocontroller Datenblatt"
     * page 1234). Fixed to 16, it allows PWM frequencies of 38Hz-25kHz
     * (with a CPU clock of 40MHz and a minimum resolution of 1%).
     */
    setPWMClockDiv(16);

    /*
     * Use the USB UART to send debugging data.
     */

     // Enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Wait until peripheral is enabled "TivaWare(TM) Treiberbibliothek"
    // page 502, "TivaC Launchpad Workshop" page 78)
    delayCycles(5);

    // Configure UART and use it with UARTStdio
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, getClockFreq());

    /*
     *  Unlock the two Pins PF0 and PD7. Because those can be used for an NMI
     *  they cannot be used for any other purpose without unlocking them first.
     *  As no use of NMIs is intended, they are unlocked here and can be used
     *  afterwards by any other class.
     */

    // Peripherals must be enabled before accessing their registers
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Wait until peripheral is enabled ("TivaWare(TM) Treiberbibliothek"
    // page 502)
    delayCycles(5);

    // Unlock both pins ("TivaC Launchpad Workshop" page 70, "TivaC
    // Mikrocontroller Datenblatt" page 656)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR)  |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

    // Enable interrupts
    IntMasterEnable();
}

void System::error(ErrorCodes errorCode, void *faultOrigin0,
                   void *faultOrigin1, void *faultOrigin2)
{
    /*
     * In case of an error other classes call this method and provide optional
     * debugging informations. It disables interrupts, stops all the
     * peripherals of the uC and enters an infinite loop.
     *
     * errorCode:   optional error parameter giving informations about the
     *              origin of the fault. Default is UnknownError
     * faultOrigin: optional pointers to the variable/object/function that
     *              caused the error.
     */

    // Disable Interrupts
    IntMasterDisable();

    // Stop all peripherals
    for (uint_fast8_t i = 0; i < PERIPH_COUNT; i++)
    {
        SysCtlPeripheralReset(ALL_PERIPHS[i]);
        SysCtlPeripheralDisable(ALL_PERIPHS[i]);
    }

    while (42);
}

uint32_t System::getClockFreq()
{
    /*
     * Returns the clock frequency of the CPU.
     */

    /*
     * Note: you could also use SysCtlClockGet(); but if you have a look at it,
     *       you'll see it's rather complicated. Therefore storing the
     *       frequency in a variable should be faster.
     */
    return clockFrequency;
}

void System::enableFPU()
{
    /*
     * Enable the Floating Point Unit. Any calculations involving "float" will
     * be executed on the FPU which is much faster than the CPU.
     */

    // "TivaC Launchpad Workshop" page 189 and 193-194
    FPULazyStackingEnable();
    FPUEnable();
}

void System::setPWMClockDiv(uint32_t div)
{
    /*
     * Set the clock divisor which applies to all PWM modules ("TivaWare(TM)
     * Treiberbibliothek" page 509 and "TivaC Mikrocontroller Datenblatt"
     * page 1234).
     *
     * div: Desired divisor. Must be a power of 2  from 1 to 64 (including).
     */

    // Set this temporarily to 0 to see if a valid divisor could be found.
    pwmClockDiv = 0;

    /*
     * Note: Since the available divisors are consecutive powers of 2, the
     * constant for 2^x (1 << x) is at position x in the array. This allows
     * for the following easy code to find the right constant.
     * This also verifies that the desired divisor is indeed a power of 2.
     */
    for (uint32_t i = 0; i < PWM_CLOCK_DIV_COUNT; i++)
    {
        if (div == (1 << i))
        {
            SysCtlPWMClockSet(PWM_CLOCK_DIV_MAPPING[i]);
            pwmClockDiv = div;
        }
    }

    // No new value has been assigned to pwmClockDiv, meaning that the
    // desired divisor was not legal.
    if (!pwmClockDiv)
    {
        error(SysWrongPWMDiv, &div);
    }
}

uint32_t System::getPWMClockDiv()
{
    /*
     * Return the value of the PWM Unit Clock Divisor. It is a power of 2
     * ranging from 1 to 64.
     */

    return pwmClockDiv;
}

void System::delayCycles(uint32_t cycles)
{
    /*
     * Halt the CPU for at least the given amount of cycles.
     * Note: Because this function uses SysCtlDelay (see "TivaWare(TM)
     *       Treiberbibliothek" page 489) only multiples of 3 clock cycles are
     *       possible. Any other value will be rounded up to the next multiple.
     *
     * cycles: Minimum number of clock cycles to halt the CPU
     */

    // To round up in an integer division of p by q, you have to increase
    // p by q-1
    cycles = (cycles + 2) / 3;
    SysCtlDelay(cycles);
}

void System::delayUS(uint32_t us)
{
    /*
     * Halt the CPU for at least the given amount of microseconds.
     * Note: Because this function uses SysCtlDelay (see "TivaWare(TM)
     *       Treiberbibliothek" page 489) only multiples of 3 clock cycles are
     *       possible. Any other value will be rounded up to the next multiple.
     *
     * us: Minimum delay in microseconds
     */

    // Convert to clock cycles
    us *= getClockFreq() / 1000000;

    delayCycles(us);
}

void System::setDebugging(bool debug)
{
    /*
     * Enable or disable the transmission of debugging data via UART.
     * By default debugging is enabled.
     */
    debugEnabled = debug;
}

void System::setDebugVal(const char* name, int32_t value)
{
    /*
     * Set up to <systemMaxDebugVals> values to monitor via USB UART (Serial
     * Monitor or Serial Plotter).
     * Example:
     *   You have two functions that use debugging. Function foo watches 1
     *   value, function bar watches 2 values. Of course you do not want to
     *   overwrite the values from foo inside bar and vice-versa.
     *   Therefore you write inside foo:
     *      fooSys->setDebugVal("My Foo Value", fooValue);
     *   And inside bar:
     *      barSys->setDebugVal("My first Bar Value",  barValue1);
     *      barSys->setDebugVal("My second Bar Value", barValue2);
     * Notes:
     *   * If you use the Arduino Serial Plotter your Arduino IDE needs to be
     *     v1.8.10 or newer.
     *   * Values are transmitted at 115200 baud.
     *   * Transmission format (details under System::sendDebugVals()):
     *     Label_1\tLabel_2\tLabel_3\n
     *     Value_1\tValue_2\tValue_3\n
     *     Value_1\tValue_2\tValue_3\n
     *     Value_1\tValue_2\tValue_3\n
     *     ...
     *     Label_1\tLabel_2\tLabel_3\tLabel_4\n
     *     Value_1\tValue_2\tValue_3\tValue_4\n
     *     Value_1\tValue_2\tValue_3\tValue_4\n
     *     ...
     *
     * name:  String with name that shall be shown in Serial Plotter legend.
     *        Must not contain ","  "\t"  " " or "\n".
     * value: Integer value to transmit.
     */

    if (debugEnabled)
    {
        /*
         * Check if this variable is already being watched and if not, check
         * if there's space for a new variable. In either case the
         * corresponding value is updated.
         */
        for(uint_fast8_t i = 0; i < maxDebugVals; i++)
        {
            if (debugNames[i] == name)
            {
                debugVals[i]  = value;
                return;
            }
            else if (debugNames[i] == debugUnused)
            {
                debugVals[i]  = value;
                debugNames[i] = name;
                debugNewLabel = true;
                return;
            }
        }

        /*
         * If we did not reach a return statement in the for loop we have a
         * new variable but no space for it and want to set the according flag.
         * This must be done only once otherwise System::sendDebugValues will
         * send labels only (because systemDebugNewLabel would always be true).
         */
        if (!tooManyDebugVals)
        {
            tooManyDebugVals = true;
            debugNewLabel = true;
        }
    }
}

void System::sendDebugVals()
{
    /*
     * If debugging is enabled, send data to PC via UART. Unused values are
     * skipped.
     * This method should be called periodically (f.ex. 10Hz Timer interrupt)
     */

    if (debugEnabled)
    {
        if (debugNewLabel)
        {
            /*
             * Labels are only transmitted if there's a new one. The result
             * in the Arduino serial plotter will be the same but it requires
             * far less time to transmit the values compared to sending the
             * labels at each value update.
             * See https://github.com/arduino/Arduino/blob/master/build/shared/
             * ArduinoSerialPlotterProtocol.md
             */
            for (uint_fast8_t i = 0; i < maxDebugVals; i++)
            {
                if (debugNames[i] != debugUnused)
                {
                    UARTprintf("%s\t", debugNames[i]);
                }
            }
            if (tooManyDebugVals)
            {
                UARTprintf("%s", "Too_many_debug_values");
            }
            UARTprintf("\n");
            debugNewLabel = false;
        }
        
        /*
         * Normal operation. Send values only.
         */
        for (uint_fast8_t i = 0; i < maxDebugVals; i++)
        {
            if (debugNames[i] != debugUnused)
            {
                UARTprintf("%04d\t", debugVals[i]);
            }
        }
        UARTprintf("\n");
        
    }
}
