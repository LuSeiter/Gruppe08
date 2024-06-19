// Uncomment following #define to use the precompiled PWM library instead of
// the code in this file.
//#define USE_PWM_LIBRARY

#ifndef USE_PWM_LIBRARY


#include <PWM.h>


PWM::PWM() // @suppress("Class members should be properly initialized")
{

}

PWM::~PWM() // @suppress("Class members should be properly initialized")
{

}


void PWM::init(System *sys, GPIO *dir, uint32_t portBase, uint32_t pin,
               bool invert, uint32_t freq)
{
    /*
     * Initialize a PWM module with the right PWM generator to drive an
     * H-Bridge connected to pin1 and pin2. It will run at 5kHz by default.
     * Outputs won't be active until the user sets a duty cycle.
     *
     * sys:        Pointer to the current System instance. Needed to get CPU
     *             clock frequency.
     * dir:        Zeiger auf den Richtungspin des Motors.
     * portBase:   Base address of the port with the output pins
     *             (e.g. GPIO_PORTA_BASE).
     * pin1:       The two output pins this object will use (e.g. GPIO_PIN_6).
     *             The lower pin will be used for forwards, the higher for
     *             backwards.
     * freq:       optional parameter to set the PWM frequency. Default: 5kHz
     */

    // Store pointer to the given System object, the pointer to the direction pins and the PWMInvert parameter
    this->sys = sys;
    this->dir = dir;

    // PWM::setDuty(float duty) uses floats
    sys->enableFPU();

   
   //TODO: Put your code here -> A3.1

}

void PWM::setFreq(uint32_t freq)
{
    /*
     * Set the frequency at which the PWM output will be driven.
     *
     * freq: frequency in Hz. A too low frequency will deactivate the outputs.
     */

    //TODO: Put your code here -> A3.1



    // Enable the PWM0 peripheral
       //
       SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
       // Wait for the PWM0 module to be ready.
       //
       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
       {



       }
}

void PWM::setDuty(float duty)
{
    /*
     * Set the duty cycle of the PWM output. The sign of the value determines
     * which one of the pins puts the pwm signal out. Negative values activate
     * the reverse pin, positive values the forward pin. A value of 0.0f (or
     * very close to 0.0f) deactivates the output.
     *
     * duty: duty cycle for the motor between -1.0f and 1.0f.
     */


    //TODO: Put your code here -> A3.1




    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //

    PWMGenConfigure(PWM_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //period for 5k HZ, 1/5000= 0,0002, mit dreisatzt to 800000 clock ticks
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 800000);

    //
    // Set the pulse width of PWM0 for a 25% duty cycle.
    //
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 2000000);
    //
    // Set the pulse width of PWM1 for a 75% duty cycle.
    //
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 600000);
    //
    // Start the timers in generator 0.
    //
    PWMGenEnable(PWM_BASE, PWM_GEN_0);
    //
    // Enable the outputs.
    //
    PWMOutputState(PWM_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);



}

#endif
