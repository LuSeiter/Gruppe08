#ifndef ERRORCODES_H_
#define ERRORCODES_H_

enum ErrorCodes
{
    /*
     * Note: Custom error codes can be added without problems but DO NOT
     *       CHANGE ANY OF THE EXISTING CODES to maintain compatibility!
     */

    // Codes                // Error parameters
    UnknownError,           // None
    BatteryLow,             // None
    SysWrongFrequency,      // uint32_t clk
    SysWrongPWMDiv,         // uint32_t div
    SysWrongUART,
    GPIOWrongConfig,        // uint32_t portBase, uint32_t pin, uint32_t dir
    GPIOWrongCurrent,       // uint32_t current
    ADCWrongConfig,         // None
    TimerWrongConfig,       // uint32_t base
    PWMWrongPins,           // uint32_t portBase, uint32_t pin1, uint32_t pin2
    PWMDutyOutOfRange,      // float duty
    MPUCommunicationError,
    MPUHorEqualsWheelAxis,
    MPUWrongAccelRange,
    MPUWrongGyroRange,
    MPUWrongHorAxis,
    MPUWrongWheelAxis,
    FaultRead,
    UltrasoundFalseConfig,
    CompareWrongConfig,
    CompareWrongGPIOPort

    // Add custom codes here
};

#endif /* ERRORCODES_H_ */
