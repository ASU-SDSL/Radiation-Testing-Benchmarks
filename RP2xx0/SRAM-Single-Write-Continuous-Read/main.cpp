#include <stdint.h>
#include <stdio.h>

#include <cstdlib>

#include "tusb.h"

#define PICO_MALLOC_PANIC 0

#include "pico/stdlib.h"

// if not specified use ~200kB, should be pretty safe
#ifndef SRAM_TO_USE_B
#define SRAM_TO_USE_B 200 * 1000  // ~200kB can probably be higher
#endif

// pattern defined as so to have it reread from flash each time
#define PATTERN "1234567890abcdefghijklmnopqrstuv"
#define CHUNK_SIZE 64  // max for the pico sdk usb buffer

int main() {
  stdio_init_all();

  sleep_ms(10000);  // wait 10s

  printf("Starting Test\n\n");

  // create memory
  // on the RP2xx0 the SRAM is striped across blocks, but really we don't care
  // about that because we are reading it as a logical block so can just use an
  // arbitrarily large dynamic memory allocation with a volatile designation
  volatile uint8_t* data =
      (volatile uint8_t*)malloc(SRAM_TO_USE_B * sizeof(uint8_t));

  // write pattern
  for (size_t i = 0; i < SRAM_TO_USE_B; i++) {
    data[i] = PATTERN[i % (sizeof(PATTERN) - 1)];  // -1 to avoid the \0
  }

  printf("Data written\n");

  // read and report
  while (1) {
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    printf("\n[INFO] Reading at %lu:\n", timestamp);
    // transferring at max CHUNK_SIZE for pico's buffer for better speed
    for (size_t i = 0; i < SRAM_TO_USE_B; i += CHUNK_SIZE) {
      tud_cdc_write((void*)&(data[i]), CHUNK_SIZE);
      tud_cdc_write_flush();
    }
    printf("\n[INFO] Done (started: %lu).\n", timestamp);
    sleep_ms(5000);  // wait 5s between reads
  }

  return 0;
}