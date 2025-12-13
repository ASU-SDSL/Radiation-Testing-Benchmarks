#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 1000000UL  // 1 MHz for util/delay and timing
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "../Common/usi_uart.h"

int main() {
  // set PB4 to output low
  DDRB |= _BV(DDB4);
  PORTB &= ~_BV(PB4);
  PINB |= _BV(PB4);  // toggle

  usiserial_printf("Starting Simple UART Test\n");

  int it = 0;
  // read and report every 1s
  while (1) {
    usiserial_printf("Iteration: %d\n", it);
    _delay_ms(1000);
    PINB |= _BV(PB4);  // toggle
    it++;
  }

  return 0;
}