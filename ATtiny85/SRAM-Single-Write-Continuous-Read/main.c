#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 1000000UL  // 1 MHz for util/delay and timing
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "../Common/timing.h"
#include "../Common/usi_uart.h"

// pattern defined as so to have it reread from flash each time
#define PATTERN "abcdefghijklmnopqrstuvwxyz1234"  // 30 bytes (excluding \0)

// ATtiny85 has 512 B of SRAM
// I think with everything running on it, we are using most of it already
// If I try to use more than like 100 B, it doesn't crash but
// I do start to see stack corruption issues
#define SRAM_TO_USE_B 90

int main() {
  // initialize timing and uart
  init_timing();

  // create memory
  volatile uint8_t data[SRAM_TO_USE_B];

  // write pattern
  for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
    data[i] = PATTERN[i % (sizeof(PATTERN) - 1)];
  }

  // set PB4 to output low
  DDRB |= _BV(DDB4);
  PORTB &= ~_BV(PB4);
  PINB |= _BV(PB4);  // toggle

  // read and report every 1s
  while (1) {
    uint32_t timestamp = secs();
    usiserial_printf("\nReading at %lu:\n", timestamp);

    for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
      usiserial_send_volatile_byte(data[i]);
    }

    usiserial_printf("\nDone (started: %lu).", timestamp);
    _delay_ms(1000);
    PINB |= _BV(PB4);  // toggle
  }

  return 0;
}