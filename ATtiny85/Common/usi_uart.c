/**
 * Design adapted from https://becomingmaker.com/usi-serial-send-attiny/
 */
#include "usi_uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BAUDRATE 9600
#define CYCLES_PER_BIT (F_CPU / BAUDRATE)
#define FULL_BIT_TICKS (CYCLES_PER_BIT)
#define STOPBITS 1

enum USISERIAL_SEND_STATE { AVAILABLE, FIRST, SECOND };
static volatile enum USISERIAL_SEND_STATE usiserial_send_state = AVAILABLE;

static uint8_t reverse_byte(uint8_t x) {
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}

static volatile uint8_t usiserial_tx_data;

ISR(USI_OVF_vect) {
  if (usiserial_send_state == FIRST) {
    usiserial_send_state = SECOND;

    USIDR = usiserial_tx_data << 7 | 0x7F;
    USISR = 1 << USIOIF | (16 - (1 + (STOPBITS)));
  } else {
    PORTB |= 1 << PB1;
    DDRB |= (1 << PB1);
    USICR = 0;
    USISR |= 1 << USIOIF;
    usiserial_send_state = AVAILABLE;
  }
}

void usiserial_send_byte(uint8_t data) {
  while (usiserial_send_state != AVAILABLE);  // wait

  usiserial_send_state = FIRST;
  usiserial_tx_data = reverse_byte(data);

  // configure timer 0 to
  // clear timer on Compare match
  // set WGM0, flag to 010
  TCCR0A = 2 << WGM00;
  TCCR0B = _BV(CS00);

  // reset prescaler
  GTCCR |= 1 << PSR0;
  // set counter to 0
  TCNT0 = 0;

  OCR0A = FULL_BIT_TICKS;  // trigger for each bit

  // USI set up
  USICR = (0 << USIWM1) |
          (1 << USIWM0) |  // Select three wire mode so USI output on PB1
          (1 << USIOIE) |  // Enable USI Counter OVF interrupt.
          (0 << USICS1) | (1 << USICS0) |
          (0 << USICLK);  // Timer0 Compare Match as USI clock source
  DDRB |= (1 << PB1);     // Configure USI DO, PB1 as output

  USIDR = 0x00 | usiserial_tx_data >> 1;

  USISR = 1 << USIOIF | (16 - 8);
}

void usiserial_send_bytes(uint8_t* data, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    usiserial_send_byte(data[i]);  // spins internally
  }
}

uint8_t usiserial_send_string(char* str) {
  uint8_t len = strnlen(str, 255);  // capped at 255 for some safety
  for (uint8_t i = 0; i < len; i++) {
    usiserial_send_byte(str[i]);
  }

  return len;
}

int usiserial_printf(const char* fmt, ...) {
  char buf[USISERIAL_PRINTF_BUF_SIZE];
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (len < 0) return len;
  if (len > (int)sizeof(buf) - 1) len = sizeof(buf) - 1;

  for (int i = 0; i < len; i++) {
    usiserial_send_byte((uint8_t)buf[i]);
  }

  return len;
}