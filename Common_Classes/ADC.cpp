// Uncomment following #define to use the precompiled ADC library instead of
// the code in this file.
//#define USE_ADC_LIBRARY

#ifndef USE_ADC_LIBRARY


#include <ADC.h>


ADC::ADC() // @suppress("Class members should be properly initialized")
{

}

ADC::~ADC() // @suppress("Class members should be properly initialized")
{

}

void ADC::init(System *sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput)
{
    /*
     * Initialize a sequence on the given ADC module to read the given
     * analog pin. The user must take care for not choosing a sequencer
     * already in use.
     *
     * sys:         Pointer to the current System instance. Needed for error
     *              handling.
     * base:        Selects the ADC module to use (ADC0_BASE or ADC1_BASE).
     * sampleSeq:   Number of the sample sequence in the module (0-3).
     * analogInput: Input that shall be read (ADC_CTL_CHx with 0 <= x <= 11)
     *              (See "TivaC Mikrocontroller Datenblatt" page 801-802 for
     *               detailed pin mapping information).
     */

    // Store base address of the ADC module, number of the sample sequencer to
    // use, the input pin, and the number of the ADC module (0 or 1).
    this->base = base;
    this->sampleSeq = sampleSeq;
    this->input = analogInput;

    // Check which ADC module is selected and enable and provide clock to
    // it.
    if (base == ADC0_BASE)
    {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    }
    else if(base == ADC1_BASE)
    {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    }
    else
    {
        sys->error(ADCWrongConfig, &base);
    }

    if((input > 0xB) || (sampleSeq > 3))
    {
        sys->error(ADCWrongConfig, &input);
    }

    /*
     * Enable the corresponding GPIO port and configure the pin as ADC Input.
     * driverlib/adc.h line 85 to 96: ADC_CTL_CHx = x
     * "TivaC Mikrocontroller Datenblatt" page 801-802
     */
    SysCtlPeripheralEnable(INPUT_PIN_MAPPING[input][AIN_PERIPH]);

    // Wait until peripheral is enabled "TivaWare(TM) Treiberbibliothek"
    // page 502, "TivaC Launchpad Workshop" page 78)
    sys->delayCycles(5);

    // Configure the pin as analog input.
    GPIOPinTypeADC(INPUT_PIN_MAPPING[input][AIN_PORT],
                   INPUT_PIN_MAPPING[input][AIN_PIN]);

    /*
     * Configure the given sample sequence. It shall be triggered by software.
     * As priorities are not important for this class, but must be different
     * for each sequence, we simply set the sequence x to priority x.
     */
    ADCSequenceConfigure(base, sampleSeq, ADC_TRIGGER_PROCESSOR,
                         sampleSeq);

    /*
     * For simplicity we configure any sequence to sample only the given pin.
     * Then we set the interrupt flag, to mark the end of the conversion,
     * and stop sampling.
     * NOTE: we do not enable any interrupts. Therefore the interrupt flag
     * won't trigger any interrupts. However we can use it, to check if the AD
     * conversion is completed.
     */
    ADCSequenceStepConfigure(base, sampleSeq, 0,
                             input | ADC_CTL_IE | ADC_CTL_END);

    // Enable the configured sample sequence.
    ADCSequenceEnable(base, sampleSeq);

    // ADC::readVolt uses floats
    sys->enableFPU();
}

void ADC::setHWAveraging(uint32_t averaging)
{
    /*
     * Enable Hardware averaging (a.k.a. oversampling).
     * Note: this affects the whole ADC module (and therefore possibly other
     * ADC objects).
     *
     * averaging: Number of measurements which are averaged. Can be 2, 4, 8,
     * 16, 32 or 64. 0 disables averaging.
     */

    ADCHardwareOversampleConfigure(base, averaging);
}

uint32_t ADC::read()
{
    /*
     * Read the analog pin and return a value ranging from 0 to 4095
     * corresponding to 0V to 3.3V.
     */

    /*
     * Note: Because the same pin could be read from a loop and an interrupt
     *       at the same time we must disable interrupts while reading.
     *       The problem otherwise:
     *        - main loop triggers conversion
     *        - interrupt occurs
     *        - ISR triggers conversion which already runs
     *        - ISR reads value and resets (!) ADC flag
     *        - main loop waits for ADC flag to be set, but this never occurs
     *          because the reading finished during the ISR and the flag is
     *          already cleared.
     *       It could also happen when reading the same pin in different ISRs.
     *       The solution is to stop the CPU from responding to interrupts. It
     *       will handle them _after_ the reading is completed and therefore
     *       readings can only happen one by one.
     */
    IntMasterDisable();

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(base, sampleSeq);

    // Trigger the ADC conversion.
    ADCProcessorTrigger(base, sampleSeq);

    // Wait for conversion to be completed.
    while(!ADCIntStatus(base, sampleSeq, false));

    // Clear the ADC interrupt flag.
    ADCIntClear(base, sampleSeq);

    // Read ADC Value.
    ADCSequenceDataGet(base, sampleSeq, reading);

    // Now that the reading is done, we can allow the processor again to
    // respond to interrupts
    IntMasterEnable();

    // Return the read value
    return reading[0];
}

float ADC::readVolt()
{
    /*
     * Read the analog pin and return the measured voltage in volt.
     */

    return read() * 3.3f / 4095.0f;
}


#endif
