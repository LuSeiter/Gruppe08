#include "MPU6050.h"


MPU6050::MPU6050() // @suppress("Class members should be properly initialized")
{

}

MPU6050::~MPU6050() // @suppress("Class members should be properly initialized")
{

}

void MPU6050::init(System *sys, uint32_t I2CBase, bool addressBit,
                   char wheelAxis, char horAxis)
{
    /*
     * Initialize the I2C bus for communicating with an MPU6050 sensor and
     * initialize this sensor.
     *
     * sys:        Pointer to the current System instance. Needed for error
     *             handling.
     * I2CBase:    base address of the I2C module the sensor is connected to
     * addressBit: value of the address pin (AD0) on the sensor determining
     *             the I2C slave address of it.
     * wheelAxis:  Sensor axis which is parallel to the axis of the wheels.
     * horAxis:    Sensor axis which - apart from the wheelAxis - is horizontal
     *             (in calibration position).
     */

    // Create private reference to the given System object.
    mpuSys = sys;

    // Save I2C Module base address
    mpuI2CBase = I2CBase;

    // Determine which I2C module is used (hw_memmap.h line 69).
    mpuI2CModuleNum = (mpuI2CBase - I2C0_BASE) / 0x1000;

    // Determine I2C Address (MPU-6050 datasheet page 15)
    mpuAddress = 0b1101000 + addressBit;

    // We use floats
    mpuSys->enableFPU();

    // Enable the GPIO port with the I2C pins.
    SysCtlPeripheralEnable(mpuConstants[mpuI2CModuleNum][mpuGPIOPeriph]);
    SysCtlPeripheralEnable(mpuConstants[mpuI2CModuleNum][mpuI2CPeriph]);

    // Wait until peripheral is enabled ("TivaWare(TM) Treiberbibliothek"
    // page 502, "TivaC Launchpad Workshop" page 78)
    mpuSys->delayCycles(5);

    // Set pin types to I2C SCL/SDA pins.
    GPIOPinTypeI2CSCL(mpuConstants[mpuI2CModuleNum][mpuGPIOBase],
                      mpuConstants[mpuI2CModuleNum][mpuSCLPin]);
    GPIOPinTypeI2C(mpuConstants[mpuI2CModuleNum][mpuGPIOBase],
                   mpuConstants[mpuI2CModuleNum][mpuSDAPin]);

    // Configure these two pins.
    GPIOPinConfigure(mpuConstants[mpuI2CModuleNum][mpuSCLPinCfg]);
    GPIOPinConfigure(mpuConstants[mpuI2CModuleNum][mpuSDAPinCfg]);

    // Initialize Master and Slave to communicate at 400kb/s
    // (set fast-mode (400kb/s) to true).
    I2CMasterInitExpClk(mpuI2CBase, mpuSys->getClockFreq(), true);

    /*
     * Check whether communicating with the MPU6050 works.
     *
     * Note: The MPU-6050s WHO_AM_I register stores the I2C address without
     *       address bit. However there's a variant of the MPU-6050, the
     *       MPU-6052C whose WHO_AM_I value seems to be 0b01110010 (although
     *       both variants have the same I2C address).
     *       Unfortunately no Register Map could be found for this variant.
     */
    uint8_t whoAmI = getRegister(MPU6050_WHO_AM_I);
    if (whoAmI + addressBit != mpuAddress && whoAmI != 0b1110010)
    {
        mpuSys->error(MPUCommunicationError);
    }

    /*
     * Initialize MPU6050.
     * For more information see MPU6050 Register Map and Descriptions
     */
    // Set the clock source to PLL with X axis gyroscope reference.
    setRegister(MPU6050_PWR_MGMT_1, 0x01);

    // Configure the MPU6050 to disable external Frame Synchronization, to let
    // the accelerometer run unfiltered at 1kHz and the gyroscope at 8kHz.
    setRegister(MPU6050_CONFIG, 0x00);

    // Set the gyroscope sample rate divider to 1.
    setRegister(MPU6050_SMPLRT_DIV, 0x00);

    // Configure the gyro to the appropriate full scale range and do not
    // perform a self-test.
    uint8_t range;
    switch (mpuGyroRange)
    {
    case 2000:
        range = 0b11000;
        break;
    case 1000:
        range = 0b10000;
        break;
    case 500:
        range = 0b01000;
        break;
    case 250:
        range = 0b00000;
        break;
    default:
        mpuSys->error(MPUWrongGyroRange);
    }
    setRegister(MPU6050_GYRO_CONFIG, range);

    // Configure the accelerometer to the appropriate full scale range and do
    // not perform self-test.
    switch (mpuAccelRange)
    {
    case 16:
        range = 0b11000;
        break;
    case 8:
        range = 0b10000;
        break;
    case 4:
        range = 0b01000;
        break;
    case 2:
        range = 0b00000;
        break;
    default:
        mpuSys->error(MPUWrongAccelRange);
    }
    setRegister(MPU6050_ACCEL_CONFIG, range);

    // Configure sensor orientation
    setWheelAxis(wheelAxis);
    setHorAxis(horAxis);
}

void MPU6050::setWheelAxis(char axis)
{
    /*
     * Tell the MPU6050 object which one of the sensor's axes is parallel
     * to the axis of the wheels. This is the axis of which the gyrometer
     * measures the angle rate.
     * Note: this method also resets the horizontal and vertical axis
     *       (as changing the wheel axis necessitates a change to one of
     *       these). Remember to call MPU6050::setHorAxis afterwards.
     *
     * axis: the character 'x' ('X'), 'y' ('Y') or 'z' ('Z')
     */

    // Convert to lower case (see ASCII table)
    mpuAxis = (axis < 'a') ? (axis + 0x20) : axis;

    // set the gyrometer register we want to read to the right axis.
    switch (mpuAxis)
    {
    case 'x':
        mpuAngleRateRegister = MPU6050_GYRO_XOUT_H;
        mpuAccelHorRegister = MPU6050_ACCEL_YOUT_H;
        mpuAccelVerRegister = MPU6050_ACCEL_ZOUT_H;
        break;
    case 'y':
        mpuAngleRateRegister = MPU6050_GYRO_YOUT_H;
        mpuAccelHorRegister = MPU6050_ACCEL_XOUT_H;
        mpuAccelVerRegister = MPU6050_ACCEL_ZOUT_H;
        break;
    case 'z':
        mpuAngleRateRegister = MPU6050_GYRO_ZOUT_H;
        mpuAccelHorRegister = MPU6050_ACCEL_XOUT_H;
        mpuAccelVerRegister = MPU6050_ACCEL_YOUT_H;
        break;
    default:
        mpuSys->error(MPUWrongWheelAxis, &mpuAxis);
    }
}

void MPU6050::setHorAxis(char hor)
{
    /*
     * Set horizontal axis (in calibration position) of the sensor which must
     * not be the wheel axis.
     *
     * hor: x, y, or z (lower or upper case)
     */

    // Convert to lower case
    hor = (hor < 'a') ? (hor + 0x20) : hor;

    //
    if (hor == mpuAxis)
    {
        mpuSys->error(MPUHorEqualsWheelAxis, &hor);
    }

    switch (hor)
    {
        case 'x':
        mpuAccelHorRegister = MPU6050_ACCEL_XOUT_H;
        if (mpuAxis == 'z')
        {
            mpuAccelVerRegister = MPU6050_ACCEL_YOUT_H;
        }
        else
        {
            mpuAccelVerRegister = MPU6050_ACCEL_ZOUT_H;
        }
        break;
        case 'y':
        mpuAccelHorRegister = MPU6050_ACCEL_YOUT_H;
        if (mpuAxis == 'x')
        {
            mpuAccelVerRegister = MPU6050_ACCEL_ZOUT_H;
        }
        else
        {
            mpuAccelVerRegister = MPU6050_ACCEL_XOUT_H;
        }
        break;
        case 'z':
        mpuAccelHorRegister = MPU6050_ACCEL_ZOUT_H;
        if (mpuAxis == 'y')
        {
            mpuAccelVerRegister = MPU6050_ACCEL_XOUT_H;
        }
        else
        {
            mpuAccelVerRegister = MPU6050_ACCEL_YOUT_H;
        }
        break;
        default:
        mpuSys->error(MPUWrongHorAxis, &hor);
    }
}

void MPU6050::angleRateInvertSign(bool invertSign)
{
    /*
     * Depending on the orientation of the MPU-6050 sensor you have to invert
     * the sign.
     *
     * invertSign: whether to invert the sign or not.
     */

    if (invertSign)
    {
        mpuAngleRateSign = -1.0f;
    }
    else
    {
        mpuAngleRateSign = 1.0f;
    }
}

void MPU6050::accelHorInvertSign(bool invertSign)
{
    /*
     * Depending on the orientation of the MPU-6050 sensor you have to
     * invert the sign.
     * invertSign: whether to invert the sign or not.
     */

    if (invertSign)
    {
        mpuAccelHorSign = -1.0f;
    }
    else
    {
        mpuAccelHorSign = 1.0f;
    }
}

void MPU6050::accelVerInvertSign(bool invertSign)
{
    /*
     * Depending on the orientation of the MPU-6050 sensor you have to
     * invert the sign.
     *
     * invertSign: whether to invert the sign or not.
     */

    if (invertSign)
    {
        mpuAccelVerSign = -1.0f;
    }
    else
    {
        mpuAccelVerSign = 1.0f;
    }
}

float MPU6050::getAngleRate()
{
    /*
     * Return the angle rate in �/s from the corresponding gyro.
     */

    int16_t rawAngleRate = (getRegister(mpuAngleRateRegister) << 8)
                           | getRegister(mpuAngleRateRegister + 1);
    return (mpuAngleRateSign * rawAngleRate * mpuGyroRange) / (1 << 15);
}

float MPU6050::getAccelHor()
{
    /*
     * Return the horizontal acceleration in g from the corresponding
     * accelerometer.
     */

    int16_t rawAccelHor = (getRegister(mpuAccelHorRegister) << 8)
                          | getRegister(mpuAccelHorRegister + 1);
    return (mpuAccelHorSign * rawAccelHor * mpuAccelRange) / (1 << 15);
}

float MPU6050::getAccelVer()
{
    /*
     * Return the vertical acceleration in g from the corresponding
     * accelerometer.
     */

    int16_t rawAccelVer = (getRegister(mpuAccelVerRegister) << 8)
                          | getRegister(mpuAccelHorRegister + 1);
    return (mpuAccelVerSign * rawAccelVer * mpuAccelRange) / (1 << 15);
}

void MPU6050::setRegister(uint8_t reg, uint8_t val)
{
    /*
     * Set a register reg from the MPU to the value val.
     *
     * reg: Address of the register to be overwritten
     * val: Value to be written to the register
     */

    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(mpuI2CBase, mpuAddress, false);

    // Tell the MPU6050 which register is modified.
    I2CMasterDataPut(mpuI2CBase, reg);

    // Initiate send of data from the MCU
    I2CMasterControl(mpuI2CBase, I2C_MASTER_CMD_BURST_SEND_START);

    // Wait until MCU is done transferring.
    while (I2CMasterBusy(mpuI2CBase));

    // Put value to be written in the data register
    I2CMasterDataPut(mpuI2CBase, val);

    // End transmission
    I2CMasterControl(mpuI2CBase, I2C_MASTER_CMD_BURST_SEND_FINISH);

    // Wait until MCU is done transferring.
    while (I2CMasterBusy(mpuI2CBase));
}

uint32_t MPU6050::getRegister(uint8_t reg)
{
    /*
     * Read a register from the MPU.
     * reg: Address of the register to be read
     * Return value: value of this register
     *
     * This code is based on the example from https://www.digikey.com/eewiki/
     * display/microcontroller/I2C+Communication+with+the+TI+Tiva+TM4C123GXL
     */

    //specify that we are writing (a register address) to the slave device
    I2CMasterSlaveAddrSet(mpuI2CBase, mpuAddress, false);

    //specify register to be read
    I2CMasterDataPut(mpuI2CBase, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(mpuI2CBase, I2C_MASTER_CMD_SINGLE_SEND);

    //wait for MCU to finish transaction
    while (I2CMasterBusy(mpuI2CBase));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(mpuI2CBase, mpuAddress, true);

    //send control byte and read from the register we specified
    I2CMasterControl(mpuI2CBase, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while (I2CMasterBusy(mpuI2CBase));

    //return data pulled from the specified register
    return I2CMasterDataGet(mpuI2CBase);
}
