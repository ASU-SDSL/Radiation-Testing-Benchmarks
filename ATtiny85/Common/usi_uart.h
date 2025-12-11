/**
 * UART Transmit Functions using Universal Serial Interface (USI)
 * Based on ATMEL AVR307 (https://ww1.microchip.com/downloads/en/AppNotes/doc4300.pdf)
 */
#ifndef USI_UART_H
#define USI_UART_H

#include <stdint.h>

void usiserial_send_byte(uint8_t data);
void usiserial_send_bytes(uint8_t* data, uint8_t len); 

void usiserial_send_volatile_byte(volatile uint8_t data);
void usiserial_send_volatile_bytes(volatile uint8_t* data, uint8_t len); 

uint8_t usiserial_send_string(char* str); 

#define USISERIAL_PRINTF_BUF_SIZE 256

int usiserial_printf(const char* fmt, ...); 

#endif 