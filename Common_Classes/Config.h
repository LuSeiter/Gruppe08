#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"


#define CFG_SYS_FREQ                    40000000            // CPU clock
#define SYNCWORD                        0b11000011          // Syncword used in UART communication
#define PI                              3.14159265358979f
#define MINPULSE                        10                  // Minimum pulse used in triggering the ultrasound sensor

// Defines constants used in projects run on the magni itself (PWM_Class_Test, Magni_Class_Test, Distance_Class_Test)
#ifdef MAGNI

#define VoltToAmp                       57e-3f           // 57 * 1mOhm
#define UMFANG                          64.5f

// System
#define CFG_TEMP                        25
#define SOUNDVELOCITY                   (331.5f + (0.6f * CFG_TEMP))


#define CFG_BATT_TIMEOUT                5                   // time in seconds until battery error occurs
#define CFG_BATT_FACTOR                 (28.0f/3)           // Battery Conversion Constant
#define CFG_BATT_MIN                    21.0f               // Minimum Battery Voltage after conversion

#define CFG_US_TIMER_BASE               TIMER1_BASE         // Timer used to send debug data with a fixed frequency to the computer.
#define CFG_US_TIMER_FREQ               50                  // Frequency at which the computer receives new debug data.


// Main timer
#define CFG_MAIN_TIMER_BASE             TIMER0_BASE         // Timer used to run the Magni-Silver code.
#define CFG_UPDATE_FREQ                 100                 // Frequency at which the magni silver updates all values

// Communication Protocol Nicht mehr aktuell?!
/*
 * First Word:  PWM Left Value
 * Second Word: PWM Right Value
 * Third Word:  Bit 7:  Fault Clear
 *              Bit 6:  Enable
 *              Bit 5:  Brake Left
 *              Bit 4:  Brake Right
 *              Bit 3:  Dir Left
 *              Bit 2:  Dir Right
 *              Bit 1:
 */

#define CFG_COM_DEBUG                   7                   // Position of 
#define CFG_COM_ENABLE                  7                   // position of Enable in first UART byte
#define CFG_COM_FAULTCLR                5                   // position of Fault Clear in first UART byte
#define CFG_BAUD_RATE                   38400               // BaudRate
#define CFG_BYTE_PER_MESSAGE_REC        3
#define CFG_BYTE_PER_MESSAGE_SENT       12

// Motor

#define CFG_MTR_PWM_PORT                GPIO_PORTA_BASE
#define CFG_MTR_PWM_PIN1                GPIO_PIN_6
#define CFG_MTR_PWM_PIN2                GPIO_PIN_7
#define CFG_MTR_PWM_INV                 false
#define CFG_MTR_PWM_FREQ                5000

#define CFG_MTR1_DIR_PORT               GPIO_PORTA_BASE
#define CFG_MTR1_DIR_PIN                GPIO_PIN_4
#define CFG_MTR1_DIR_DIR                GPIO_DIR_MODE_OUT
#define CFG_MTR1_DIR_PULLUP             false

#define CFG_MTR2_DIR_PORT               GPIO_PORTB_BASE
#define CFG_MTR2_DIR_PIN                GPIO_PIN_4
#define CFG_MTR2_DIR_DIR                GPIO_DIR_MODE_OUT
#define CFG_MTR2_DIR_PULLUP             false

#define CFG_MTR_EN_PORT                 GPIO_PORTE_BASE
#define CFG_MTR_EN_PIN                  GPIO_PIN_4
#define CFG_MTR_EN_DIR                  GPIO_DIR_MODE_OUT
#define CFG_MTR_EN_PULLUP               false

#define CFG_MTR1_BRAKE_PORT             GPIO_PORTD_BASE
#define CFG_MTR1_BRAKE_PIN              GPIO_PIN_3
#define CFG_MTR1_BRAKE_DIR              GPIO_DIR_MODE_OUT
#define CFG_MTR1_BRAKE_PULLUP           false

#define CFG_MTR2_BRAKE_PORT             GPIO_PORTA_BASE
#define CFG_MTR2_BRAKE_PIN              GPIO_PIN_5
#define CFG_MTR2_BRAKE_DIR              GPIO_DIR_MODE_OUT
#define CFG_MTR2_BRAKE_PULLUP           false

#define CFG_MTR_BRAKE_TRUE              false

// Motor current

#define CFG_MTR1_CURRENT_BASE           ADC1_BASE
#define CFG_MTR1_CURRENT_SAMPLESEQ      0
#define CFG_MTR1_CURRENT_ANALOGIN       ADC_CTL_CH1

#define CFG_MTR2_CURRENT_BASE           ADC1_BASE
#define CFG_MTR2_CURRENT_SAMPLESEQ      1
#define CFG_MTR2_CURRENT_ANALOGIN       ADC_CTL_CH0

// Battery voltage

#define CFG_BATT_BASE                   ADC0_BASE
#define CFG_BATT_SAMPLESEQ              0
#define CFG_BATT_ANALOGIN               ADC_CTL_CH2

// Fault

#define CFG_FLT1_PORT                   GPIO_PORTA_BASE
#define CFG_FLT1_PIN                    GPIO_PIN_2
#define CFG_FLT1_DIR                    GPIO_DIR_MODE_IN
#define CFG_FLT1_PULLUP                 false

#define CFG_FLT2_PORT                   GPIO_PORTC_BASE
#define CFG_FLT2_PIN                    GPIO_PIN_7
#define CFG_FLT2_DIR                    GPIO_DIR_MODE_IN
#define CFG_FLT2_PULLUP                 false

#define CFG_FLT_CLR_PORT                GPIO_PORTE_BASE
#define CFG_FLT_CLR_PIN                 GPIO_PIN_5
#define CFG_FLT_CLR_DIR                 GPIO_DIR_MODE_OUT
#define CFG_FLT_CLR_PULLUP              false

// RPM PB2 PB5

#define CFG_RPM1_PORT                   GPIO_PORTB_BASE
#define CFG_RPM1_PIN                    GPIO_PIN_5

#define CFG_RPM2_PORT                   GPIO_PORTB_BASE
#define CFG_RPM2_PIN                    GPIO_PIN_2

// UART

#define CFG_UART_PORT                   GPIO_PORTB_BASE
#define CFG_UART_TX                     GPIO_PIN_1
#define CFG_UART_RX                     GPIO_PIN_0

// LED1
#define CFG_LED1_PORT                   GPIO_PORTB_BASE
#define CFG_LED1_PIN                    GPIO_PIN_7
#define CFG_LED1_DIR                    GPIO_DIR_MODE_OUT
#define CFG_LED1_PULLUP                 false

// LED2
#define CFG_LED2_PORT                   GPIO_PORTD_BASE
#define CFG_LED2_PIN                    GPIO_PIN_1
#define CFG_LED2_DIR                    GPIO_DIR_MODE_OUT
#define CFG_LED2_PULLUP                 false

#define CFG_TRIG_TIME                   15 // Time in us

// Sensor 0 PD2 PD7
#define CFG_ECHO0_PORT                  GPIO_PORTD_BASE
#define CFG_ECHO0_PIN                   GPIO_PIN_2
#define CFG_TRIG0_PORT                  GPIO_PORTD_BASE
#define CFG_TRIG0_PIN                   GPIO_PIN_7

// Sensor 1 PC4 PB7
#define CFG_ECHO1_PORT                  GPIO_PORTC_BASE
#define CFG_ECHO1_PIN                   GPIO_PIN_4
#define CFG_TRIG1_PORT                  GPIO_PORTB_BASE
#define CFG_TRIG1_PIN                   GPIO_PIN_7

// Sensor 2 PD6 PA3
#define CFG_ECHO2_PORT                  GPIO_PORTD_BASE
#define CFG_ECHO2_PIN                   GPIO_PIN_6
#define CFG_TRIG2_PORT                  GPIO_PORTA_BASE
#define CFG_TRIG2_PIN                   GPIO_PIN_3

// Sensor 3 PD0 PC5
#define CFG_ECHO3_PORT                  GPIO_PORTD_BASE
#define CFG_ECHO3_PIN                   GPIO_PIN_0
#define CFG_TRIG3_PORT                  GPIO_PORTC_BASE
#define CFG_TRIG3_PIN                   GPIO_PIN_5

// Sensor 4 PC6 PE0
#define CFG_ECHO4_PORT                  GPIO_PORTC_BASE
#define CFG_ECHO4_PIN                   GPIO_PIN_6
#define CFG_TRIG4_PORT                  GPIO_PORTE_BASE
#define CFG_TRIG4_PIN                   GPIO_PIN_0

#endif

#ifdef CONTROL

/*
 * Joystick Constants
 */
#define CFG_JOY_BTN_PORT                GPIO_PORTA_BASE
#define CFG_JOY_BTN_PIN                 GPIO_PIN_2
#define CFG_JOY_BTN_DIR                 GPIO_DIR_MODE_IN
#define CFG_JOY_BTN_PULLUP              true

#define CFG_JOY_XAX_MOD                 ADC0_BASE
#define CFG_JOY_XAX_SSQ                 0
#define CFG_JOY_XAX_ANI                 ADC_CTL_CH3

#define CFG_JOY_YAX_MOD                 ADC1_BASE
#define CFG_JOY_YAX_SSQ                 0
#define CFG_JOY_YAX_ANI                 ADC_CTL_CH0

/*
 * Button Constants
 */

#define CFG_BTN1_PORT                   GPIO_PORTB_BASE
#define CFG_BTN1_PIN                    GPIO_PIN_4
#define CFG_BTN1_DIR                    GPIO_DIR_MODE_IN
#define CFG_BTN1_PULLUP                 true

#define CFG_BTN2_PORT                   GPIO_PORTA_BASE
#define CFG_BTN2_PIN                    GPIO_PIN_5
#define CFG_BTN2_DIR                    GPIO_DIR_MODE_IN
#define CFG_BTN2_PULLUP                 true

#define CFG_BTN3_PORT                   GPIO_PORTA_BASE
#define CFG_BTN3_PIN                    GPIO_PIN_6
#define CFG_BTN3_DIR                    GPIO_DIR_MODE_IN
#define CFG_BTN3_PULLUP                 true

#define CFG_BTN4_PORT                   GPIO_PORTA_BASE
#define CFG_BTN4_PIN                    GPIO_PIN_7
#define CFG_BTN4_DIR                    GPIO_DIR_MODE_IN
#define CFG_BTN4_PULLUP                 true

/*
 * LCD Constants
 * RS: Register Select
 * RW: Read/Write
 * E: Enable
 * D4-7: Data pins
 */
#define CFG_LCD_RS_PORT                 GPIO_PORTC_BASE
#define CFG_LCD_RS_PIN                  GPIO_PIN_4
#define CFG_LCD_RS_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_RS_PULLUP               false

#define CFG_LCD_RW_PORT                 GPIO_PORTC_BASE
#define CFG_LCD_RW_PIN                  GPIO_PIN_5
#define CFG_LCD_RW_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_RW_PULLUP               false

#define CFG_LCD_E_PORT                  GPIO_PORTC_BASE
#define CFG_LCD_E_PIN                   GPIO_PIN_6
#define CFG_LCD_E_DIR                   GPIO_DIR_MODE_OUT
#define CFG_LCD_E_PULLUP                false

#define CFG_LCD_D4_PORT                 GPIO_PORTD_BASE
#define CFG_LCD_D4_PIN                  GPIO_PIN_0
#define CFG_LCD_D4_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_D4_PULLUP               false

#define CFG_LCD_D5_PORT                 GPIO_PORTD_BASE
#define CFG_LCD_D5_PIN                  GPIO_PIN_1
#define CFG_LCD_D5_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_D5_PULLUP               false

#define CFG_LCD_D6_PORT                 GPIO_PORTD_BASE
#define CFG_LCD_D6_PIN                  GPIO_PIN_2
#define CFG_LCD_D6_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_D6_PULLUP               false

#define CFG_LCD_D7_PORT                 GPIO_PORTD_BASE
#define CFG_LCD_D7_PIN                  GPIO_PIN_3
#define CFG_LCD_D7_DIR                  GPIO_DIR_MODE_OUT
#define CFG_LCD_D7_PULLUP               false

/*
 * MPU Constants
 */

#define CFG_SENSOR_ADRESSBIT             0                  // State of the AD0 pin on the MPU6050
#define CFG_SENSOR_I2C_MODULE            I2C0_BASE          // I2C1: PA6 = SCL, PA7 = SDA
#define CFG_SENSOR_WHEEL_AXIS            'Y'
#define CFG_SENSOR_HOR_AXIS              'Z'
#define CFG_SENSOR_INVERT_ANGLE_RATE     false              // Rotating in driving direction is positive
#define CFG_SENSOR_INVERT_HOR            true               // Driving direction is positive
#define CFG_SENSOR_INVERT_VER            true               // Downwards is positive

/*
 * UART Constants
 */

#define CFG_UART_PORT                   GPIO_PORTB_BASE
#define CFG_UART_TX                     GPIO_PIN_1
#define CFG_UART_RX                     GPIO_PIN_0
#define CFG_BAUD_RATE                   38400
#define CFG_BYTE_PER_MESSAGE_REC        13
#define CFG_BYTE_PER_MESSAGE_SENT       3

// ESP Enable Pin

#define CFG_ESP_E_PORT                  GPIO_PORTB_BASE
#define CFG_ESP_E_PIN                   GPIO_PIN_5
#define CFG_ESP_E_DIR                   GPIO_DIR_MODE_IN
#define CFG_ESP_E_PULLUP                true

#endif
#endif /* CONFIG_H_ */
