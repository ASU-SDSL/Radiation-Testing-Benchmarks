#ifndef USI_UART_H
#define USI_UART_H

#include <stdint.h>

uint8_t USI_UART_Initialize_Transmitter(); 
uint8_t USI_UART_Transmit_Byte(uint8_t data);

#endif 