/**
 * UART Transmit Functions using Universal Serial Interface (USI)
 * Based on ATMEL AVR307 (https://ww1.microchip.com/downloads/en/AppNotes/doc4300.pdf)
 */
#ifndef USI_UART_H
#define USI_UART_H

#include <stdint.h>

uint8_t USI_UART_Initialize_Transmitter(); 
uint8_t USI_UART_Transmit_Byte(uint8_t data);

#endif 