#include "Magni.h"

#define USEDISTANCE

Magni::Magni() // @suppress("Class members should be properly initialized")
{

}
Magni::~Magni() // @suppress("Class members should be properly initialized")
{

}

void Magni::init(System *sys, void (*rpmLeftISR)(void), void (*rpmRightISR)(void), void (*sensor0ISR)(void), void (*sensor1ISR)(void), void (*sensor2ISR)(void), void (*sensor3ISR)(void), void (*sensor4ISR)(void))
{
    /*
     * Initializes all the objects used in the Magni Class.
     *
     * Needs to be called before any components can be used.
     *
     * System *sys: Pointer to the current instance of the system class.
     * void (*rpmLeftISR) (void): Pointer to the ISR which measures the RPM of the left wheel.
     * void (*rpmRightISR) (void): Pointer to the ISR which measures the RPM of the right wheel.
     * void (*sensor1ISR) (void): Pointer to the ISR which reads the distance measured by sensor 1
     * void (*sensor2ISR) (void): Pointer to the ISR which reads the distance measured by sensor 2
     * void (*sensor3ISR) (void): Pointer to the ISR which reads the distance measured by sensor 3
     * void (*sensor4ISR) (void): Pointer to the ISR which reads the distance measured by sensor 4
     * void (*sensor5ISR) (void): Pointer to the ISR which reads the distance measured by sensor 5
     */

    // Creates private reference to the given System Object.
    this->magniSys = sys;

    // Initializes all needed object according to the given parameters and the parameters in Config.h
    this->communication.init(magniSys, CFG_UART_PORT, CFG_UART_TX, CFG_UART_RX);
    dirLeft.init(magniSys, CFG_MTR1_DIR_PORT, CFG_MTR1_DIR_PIN, CFG_MTR1_DIR_DIR, CFG_MTR1_DIR_PULLUP);
    dirRight.init(magniSys, CFG_MTR2_DIR_PORT, CFG_MTR2_DIR_PIN, CFG_MTR2_DIR_DIR, CFG_MTR2_DIR_PULLUP);

    motorLeft.init(magniSys, &dirLeft, CFG_MTR_PWM_PORT, CFG_MTR_PWM_PIN1, CFG_MTR_PWM_INV, CFG_MTR_PWM_FREQ);
    motorRight.init(magniSys, &dirRight, CFG_MTR_PWM_PORT, CFG_MTR_PWM_PIN2, CFG_MTR_PWM_INV, CFG_MTR_PWM_FREQ);

    motorLeftCurrent.init(magniSys, CFG_MTR1_CURRENT_BASE, CFG_MTR1_CURRENT_SAMPLESEQ, CFG_MTR1_CURRENT_ANALOGIN);
    motorRightCurrent.init(magniSys, CFG_MTR2_CURRENT_BASE, CFG_MTR2_CURRENT_SAMPLESEQ, CFG_MTR2_CURRENT_ANALOGIN);

    enable.init(magniSys, CFG_MTR_EN_PORT, CFG_MTR_EN_PIN, CFG_MTR_EN_DIR, CFG_MTR_EN_PULLUP);
    brakeLeft.init(magniSys, CFG_MTR1_BRAKE_PORT, CFG_MTR1_BRAKE_PIN, CFG_MTR1_BRAKE_DIR, CFG_MTR1_BRAKE_PULLUP);
    brakeRight.init(magniSys, CFG_MTR2_BRAKE_PORT, CFG_MTR2_BRAKE_PIN, CFG_MTR2_BRAKE_DIR, CFG_MTR2_BRAKE_PULLUP);

    battery.init(magniSys, CFG_BATT_BASE, CFG_BATT_SAMPLESEQ, CFG_BATT_ANALOGIN);

    rpmLeft.init(magniSys, CFG_RPM1_PORT, CFG_RPM1_PIN, rpmLeftISR);
    rpmRight.init(magniSys, CFG_RPM2_PORT, CFG_RPM2_PIN, rpmRightISR);

    faultLeft.init(magniSys, CFG_FLT1_PORT, CFG_FLT1_PIN, CFG_FLT1_DIR, CFG_FLT1_PULLUP);
    faultRight.init(magniSys, CFG_FLT2_PORT, CFG_FLT2_PIN, CFG_FLT2_DIR, CFG_FLT2_PULLUP);
    faultCLR.init(magniSys, CFG_FLT_CLR_PORT, CFG_FLT_CLR_PIN, CFG_FLT_CLR_DIR, CFG_FLT_CLR_PULLUP);

    sensor0.init(magniSys, CFG_ECHO0_PORT, CFG_ECHO0_PIN, CFG_TRIG0_PORT, CFG_TRIG0_PIN, sensor0ISR);
    sensor1.init(magniSys, CFG_ECHO1_PORT, CFG_ECHO1_PIN, CFG_TRIG1_PORT, CFG_TRIG1_PIN, sensor1ISR);
    sensor2.init(magniSys, CFG_ECHO2_PORT, CFG_ECHO2_PIN, CFG_TRIG2_PORT, CFG_TRIG2_PIN, sensor2ISR);
    sensor3.init(magniSys, CFG_ECHO3_PORT, CFG_ECHO3_PIN, CFG_TRIG3_PORT, CFG_TRIG3_PIN, sensor3ISR);
    sensor4.init(magniSys, CFG_ECHO4_PORT, CFG_ECHO4_PIN, CFG_TRIG4_PORT, CFG_TRIG4_PIN, sensor4ISR);

    // Initializes two on-board LEDs for debugging purposes
    blueLED.init(magniSys, GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT, false);
    redLED.init(magniSys, GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT, false);

    // Sets the brake Signals and the half bridge enable such that the Magni can drive
    brakeLeft.write(!CFG_MTR_BRAKE_TRUE);
    brakeRight.write(!CFG_MTR_BRAKE_TRUE);
    enable.write(true);

    // Enables the floating point unit to speed up the float calculations
    magniSys->enableFPU();
}
void Magni::update()
{
    /*
     * Read the fault signal, the sensor values and set the output values based on the sensors and the received
     * communication from the remote
     */
    // If the remote send the fault clear signal clear the fault and enter drive mode (standby = false)

    // Removed because BS (receiveWords[1] should be left wheel speed...)
    //    if(receiveWords[1] & (1 << CFG_COM_FAULTCLR))
    //    {
    //        this->standby = false;
    //        faultCLR.write(true);
    //        brakeLeft.write(!CFG_MTR_BRAKE_TRUE);
    //        brakeRight.write(!CFG_MTR_BRAKE_TRUE);
    //    }

    // If the Magni is in standby set the PWM values to 0 and the brake signals
    if(this->standby)
    {
        motorLeft.setDuty(0.0f);
        motorRight.setDuty(0.0f);
        brakeLeft.write(CFG_MTR_BRAKE_TRUE);
        brakeRight.write(CFG_MTR_BRAKE_TRUE);
    }
    else
    {
        /*
         * If the Magni is in drive mode, start the measurement of the RPM and the distances of each ultrasound sensor.
         * If there is an object in close distance to the Magni react according to the direction in which the object is
         * located.
         * Otherwise, read the PWM values received from the remote and set these as output.
         */
        rpmLeft.start();
        rpmRight.start();

        /*
         * sensor1 object is Sensor J5 -> Direction Rightmost 0
         * sensor2 object is Sensor J1 -> Direction Right 1
         * sensor3 object is Sensor J6 -> Direction Middle 2
         * sensor4 object is Sensor J4 -> Direction Left 3
         * sensor5 object is Sensor J2 -> Direction Leftmost 4
         */
#ifdef USEDISTANCE
        distanceCounter++;
        if(distanceCounter == 10)
        {
            if(sensor)
            {
                sensor1.trigger();
                sensor2.trigger();
                sensor3.trigger();
            }
            else
            {
                sensor0.trigger();
                sensor4.trigger();
            }
            sensor = !sensor;
            distanceCounter = 0;
        }

        // Gets the minimal distance from the sensors
        this->minimal_distance = fminf(sensor0.getDistance(), fminf(sensor1.getDistance(),
                                                                    fminf(sensor2.getDistance(),
                                                                          fminf(sensor3.getDistance(),
                                                                                sensor4.getDistance()))));
        // Sets the direction in which the nearest object is

        if(sensor1.getDistance() == this->minimal_distance)
        {
            this->sensor_dir = 0;
        }
        else if(sensor2.getDistance() == this->minimal_distance)
        {
            this->sensor_dir = 1;
        }
        else if(sensor3.getDistance() == this->minimal_distance)
        {
            this->sensor_dir = 2;
        }
        else if(sensor4.getDistance() == this->minimal_distance)
        {
            this->sensor_dir = 3;
        }
        else
        {
            this->sensor_dir = 4;
        }
        blueLED.write(this->minimal_distance < 20);
#endif

        if(this->status)
        {
#ifdef USEDISTANCE
            if((this->minimal_distance < 40) && (this->minimal_distance > 15))
            {

                if(distanceMid > 3)
                {
                    this->dutyLeft = 0.0f;
                    this->dutyRight = 0.0f;
                    blueLED.write(true);
                    lastDistances[0] = lastDistances[1];
                    lastDistances[1] = lastDistances[2];
                    lastDistances[2] = lastDistances[3];
                    lastDistances[3] = lastDistances[4];
                    lastDistances[4] = this->minimal_distance;
                }
                else
                {
                    lastDistances[distanceMid] = this->minimal_distance;
                    distanceMid++;
                }

            }
            else
            {

                if(this->minimal_distance < 40)
                {
                    switch(this->sensor_dir)
                    {
                    case 0:
                        this->dutyLeft = 0.0f;
                        this->dutyRight = -0.15f;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        this->dutyLeft = -0.15f;
                        this->dutyRight = -0.15f;
                        break;
                    case 4:
                        this->dutyLeft = -0.15f;
                        this->dutyRight = 0.0f;
                        break;
                    }
                    blueLED.write(true);
                }
                else
                {
                    bool dirPinLeft = receiveWords[0] & (1 << 5); // Suddenly we use bitshifts hardcoded again... TODO: All bitshifts as config
                    bool dirPinRight = receiveWords[0] & (1 << 6);
                    this->dutyLeft = receiveWords[1] / 100.0f;
                    this->dutyRight = receiveWords[2] / 100.0f;
                    if(!dirPinLeft)     this->dutyLeft = -this->dutyLeft;
                    if(!dirPinRight)    this->dutyRight = -this->dutyRight;
                    blueLED.write(false);
                }
#endif
            }
        }
        magniSys->setDebugVal("DutyLeft", this->dutyLeft * 100);
        magniSys->setDebugVal("DutyRight", this->dutyRight * 100);

        /*
         * Sets the new duty values for each motor (since they are installed opposite of each other, the logical
         * forward is negative for one and positive for the other).
         */
        motorLeft.setDuty(this->dutyLeft);
        motorRight.setDuty(this->dutyRight);

        // Enables the motors if the bit is sent by the remote control

        if(this->status)
        {
            enable.write(receiveWords[0] & (1 << CFG_COM_ENABLE));
        }
        else
        {
            this->timeOut++;
            if(this->timeOut == 50)
            {
                enable.write(false);
                this->timeOut = 0;
            }
        }


    }
    this->updateFlag = true;
}
uint8_t Magni::bool2Byte(bool *values)
{
    /*
     * Takes a boolean array of 8 values and turns it into a uint8_t value
     * First element is lowest bit and last element is highest bit
     */
    uint8_t charEnc = 0;
    for(int i=0;i<8;i++)
    {
        charEnc += values[i] << i;
    }
    return charEnc;
}
void Magni::getTransmitWords()
{
    /*
     * Gets the values to transmit via the UART pins
     * A total of 10 8bit words is sent
     * 0            BrakeLeft,BrakeRight,Enable,FaulClr,FaultLeft,FaultRight (from High to Low each as one bit)
     * 1            PWM Left Duty
     * 2            PWM Right Duty
     * 3            RPM Left
     * 4            RPM Right
     * 5            Motor Left Current
     * 6            Motor Right Current
     * 7            Error Code (0: No Error 1:Battery Low 2:No Connection 3: PWM out of range 4:timerload out of range)
     * 8            Speed
     * 9            BatteryVoltage
     * 10           Direction in which object was detected (0: straight 1: slight left 2: left 3:slight right 4: right)
     * 11           Distance to nearest object
     */
    transmitWords[COMBINED] = bool2Byte((bool[]){faultRight.read(), faultLeft.read(), faultCLR.read(),
        this->enableStatus, brakeRight.read(), brakeLeft.read(), 0, 0});

    transmitWords[PWMLEFT] = (int8_t) (dutyLeft * 100);
    transmitWords[PWMRIGHT] = (int8_t) (dutyRight * 100);

    /*
     * RPM of each motor. The frequency equals the hall sensor frequency. The input signal is a pwm signal with 50% duty cycle. The frequency of
     * that signal equals the motor frequency in Hz. getRPM() calculates the RPM in 1/min.
     */

    this->transmitWords[RPMLEFT] = rpmLeft.getFreq();
    this->transmitWords[RPMRIGHT] = rpmRight.getFreq();

    /*
     * Op Amp has an offset of 1.8V. The input voltage is measured via a shunt resistor with 1mOhms. The Op Amp has
     * a gain of 57, so the possible values with a 5V supply voltage is -31mV to 56mV equal to -31A to 56A.
     * The current is measured by an Attiny and galvanically isolated. Before measuring, the signal is divided by a voltage
     * divider with R1 = 10k and R2 = 18k which leads to the following equation:
     * CA = (U_measure * ((R1+R2)/R2) - Offset) / (57 * 1e-3)
     * Possible Values: -31 to 56
     */

    float leftcur = motorLeftCurrent.readVolt();
    float rightcur = motorRightCurrent.readVolt();
    this->transmitWords[CURRENTLEFT] = (int8_t) ((motorLeftCurrent.readVolt() * (28.0f/18) - 1.8f) * 1000 / 57);
    this->transmitWords[CURRENTRIGHT] = (int8_t) ((motorRightCurrent.readVolt() * (28.0f/18) - 1.8f) * 1000 / 57);

    /*
     * The Battery is divided by a voltage divider with R1 = 10k and R2 = 18k and a voltage divider before the Attiny with R3 = 15k and R4 = 3k.
     * Resulting in the formula for the Battery Voltage:
     * U_Batt = U_measure * (R1+R2) * (R3+R4) / (R2*R4)
     * Since R2 = R3+R4 = 18k the formula can be simplified to
     * U_Batt = U_measure * (R1+R2) / R4
     * Possible Values: 0V to 30V
     */


    this->transmitWords[SPEED] = this->speed * 100; // Speed
    float batteryVoltage = ((battery.readVolt() * CFG_BATT_FACTOR));
    this->transmitWords[BATTERY] = (uint8_t) batteryVoltage;
    this->transmitWords[BATTERYCOMMA] = (uint8_t) ((batteryVoltage - (uint8_t) batteryVoltage) * 100);
    this->transmitWords[SENSORDIR] = this->sensor_dir;
    if(this->minimal_distance > 255)
    {
        this->transmitWords[DISTANCE] = 255;
    }
    else
    {
        this->transmitWords[DISTANCE] = (uint8_t) (this->minimal_distance);
    }
}
void Magni::background()
{
    /*
     * Takes care of the background tasks. If the update method was called before executing the background method
     * the received communication is read and saved to use in the next call of the update method. Sends the diagnostic
     * data back to the remote and monitors the battery voltage.
     */
    if(updateFlag)
    {

        getTransmitWords();

        //TODO: Put your code here -> A7.1

        communication.send(this->transmitWords,14);
        if(communication.checkAvailability()){
            this->status = communication.receive(this->receiveWords,3);
        }

        if(status){
            redLED.write(this->status);

        }
        else {
            redLED.write(this->status);
        }
        this->updateFlag = false;
    }
}

