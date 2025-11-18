#include <stdint.h>
#include <stdio.h>

#include <cstdlib>

#define PICO_MALLOC_PANIC 0

#include "pico/stdlib.h"

// if not specified use ~200kB, should be pretty safe
#ifndef SRAM_TO_USE_B
#define SRAM_TO_USE_B 200 * 1000  // ~200kB can probably be higher
#endif

// pattern defined as so to have it reread from flash each time
#define PATTERN \
  "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF"

int main() {
  stdio_init_all();

  // create memory
  // on the RP2xx0 the SRAM is striped across blocks, but really we don't care
  // about that because we are reading it as a logical block so can just use an
  // arbitrarily large dynamic memory allocation with a volatile designation
  volatile uint8_t* data =
      (volatile uint8_t*)malloc(SRAM_TO_USE_B * sizeof(uint8_t));

  // write pattern
  for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
    data[i] = PATTERN[i % sizeof(PATTERN)];
  }

  // read and report every 1s
  while (1) {
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    printf("Reading at %lu:\n", timestamp);
    for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
      printf("%x", data[i]);
    }
    printf("\nDone (started: %lu).", timestamp);
  }

  return 0;
}