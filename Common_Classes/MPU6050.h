/*
 * MPU6050.h
 *
 *    Author: Max Zuidberg
 *     Email: m.zuidberg@icloud.com
 */

#ifndef MPU6050_H_
#define MPU6050_H_


/*
 * stdbool.h:               Boolean definitions for the C99 standard
 * stdint.h:                Variable definitions for the C99 standard
 * inc/hw_memmap.h:         Macros defining the memory map of the Tiva C Series
 *                          device. This includes defines such as peripheral
 *                          base address locations such as GPIO_PORTF_BASE.
 * inc/hw_pinmap.h:         Mapping of peripherals to pins for all parts.
 * driverlib/sysctl.h:      Defines and macros for the System Control API of
 *                          DriverLib. This includes API functions such as
 *                          SysCtlClockSet.
 * driverlib/i2c.h          Defines and macros for I2C API of DriverLib. This
 *                          includes API functions such as I2CMasterControl.
 * driverlib/gpio.h:        Defines and macros for GPIO API of DriverLib. This
 *                          includes API functions such as GPIOPinConfigure.
 * System.h:                Access to current CPU clock and other functions.
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "System.h"


// Register addresses of the MPU6050
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b

// Addresses of the registers with the MSB part (xxx_H) of the sensor values
// The LSB part is always stored in xxx_H + 1
#define MPU6050_ACCEL_XOUT_H 0x3b
#define MPU6050_ACCEL_YOUT_H 0x3d
#define MPU6050_ACCEL_ZOUT_H 0x3f
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_ZOUT_H  0x47


class MPU6050
{
public:
    MPU6050();
    virtual ~MPU6050();
    void init(System *sys, uint32_t I2CBase, bool addressBit,
              char wheelAxis = 'x', char horAxis = 'y');
    void setWheelAxis(char axis);
    void setHorAxis(char hor);
    void angleRateInvertSign(bool invertSign);
    void accelHorInvertSign(bool invertSign);
    void accelVerInvertSign(bool invertSign);
    float getAngleRate();
    float getAccelHor();
    float getAccelVer();
private:
    uint32_t getRegister(uint8_t reg);
    void setRegister(uint8_t reg, uint8_t val);
    System *mpuSys;
    uint32_t mpuI2CBase, mpuAddress;
    uint8_t mpuI2CModuleNum;
    float mpuAngleRateSign = 1.0f;
    float mpuAccelHorSign = 1.0f;
    float mpuAccelVerSign = 1.0f;
    uint8_t mpuAngleRateRegister, mpuAccelHorRegister, mpuAccelVerRegister;
    char mpuAxis;
    const uint16_t mpuGyroRange = 250; // [°/s]
    const uint8_t mpuAccelRange = 2;        // [g]
    const uint8_t mpuI2CPeriph = 0, mpuGPIOPeriph = 1, mpuGPIOBase = 2,
                  mpuSCLPin = 3, mpuSDAPin = 4, mpuSCLPinCfg = 5,
                  mpuSDAPinCfg = 6;
    const uint32_t mpuConstants[4][7] =
                 {{SYSCTL_PERIPH_I2C0, SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE,
                   GPIO_PIN_2, GPIO_PIN_3, GPIO_PB2_I2C0SCL, GPIO_PB3_I2C0SDA},
                  {SYSCTL_PERIPH_I2C1, SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE,
                   GPIO_PIN_6, GPIO_PIN_7, GPIO_PA6_I2C1SCL, GPIO_PA7_I2C1SDA},
                  {SYSCTL_PERIPH_I2C2, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE,
                   GPIO_PIN_4, GPIO_PIN_5, GPIO_PE4_I2C2SCL, GPIO_PE5_I2C2SDA},
                  {SYSCTL_PERIPH_I2C3, SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE,
                   GPIO_PIN_0, GPIO_PIN_1, GPIO_PD0_I2C3SCL, GPIO_PD1_I2C3SDA}};
};

#endif /* MPU6050_H_ */
