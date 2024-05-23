#ifndef UART_H_
#define UART_H_

#include "System.h"

#define RX 3
#define TX 4

class UART
{
public:
    UART();
    virtual ~UART();
    void init(System *sys, uint32_t portBase, uint32_t tx, uint32_t rx);
    void send(const char *transmit, uint8_t length);
    bool receive(char *receive, uint8_t length);
    bool checkAvailability();

private:
    System *sys;
    uint32_t base;
    uint32_t uartConfig[7][5]={{GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_1, GPIO_PB0_U1RX, GPIO_PB1_U1TX},
                                   {GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_7, GPIO_PD6_U2RX, GPIO_PD7_U2TX},
                                   {GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_7, GPIO_PC6_U3RX, GPIO_PC7_U3TX},
                                   {GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_5, GPIO_PC4_U4RX, GPIO_PC5_U4TX},
                                   {GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_5, GPIO_PE4_U5RX, GPIO_PE5_U5TX},
                                   {GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_PIN_5, GPIO_PD4_U6RX, GPIO_PD5_U6TX},
                                   {GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_1, GPIO_PE0_U7RX, GPIO_PE1_U7TX}};
};



#endif /* UART_H_ */
