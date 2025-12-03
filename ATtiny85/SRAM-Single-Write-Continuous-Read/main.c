#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #define F_CPU 1000000UL  // 1 MHz for util/delay and timing
#include <util/delay.h>

#include "../Common/timing.h"
#include "../Common/usi_uart.h"

#define SRAM_TO_USE_B 400  // ATtiny85 has 512 B of SRAM

// pattern defined as so to have it reread from flash each time
#define PATTERN \
  "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF"

int main() {
  // initialize timing and uart
  init_timing();

  // create memory
  volatile uint8_t* data =
      (volatile uint8_t*)malloc(SRAM_TO_USE_B * sizeof(uint8_t));

  // write pattern
  for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
    data[i] = PATTERN[i % sizeof(PATTERN)];
  }

  // read and report every 1s
  while (1) {
    uint32_t timestamp = secs();
    usiserial_printf("Reading at %lu:\n", timestamp);
    for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
      usiserial_printf("%x", data[i]);
    }
    usiserial_printf("\nDone (started: %lu).", timestamp);
    _delay_ms(1000);
  }

  return 0;
}