#include <stdint.h>
#include <stdio.h>

#include <cstdlib>

#include "tusb.h"

#define PICO_MALLOC_PANIC 0

#include "addition.pio.h"
#include "hardware/pio.h"
#include "hardware/watchdog.h"
#include "pico/stdlib.h"

// if not specified use ~200kB, should be pretty safe
#ifndef SRAM_TO_USE_B
#define SRAM_TO_USE_B 200 * 1000  // ~200kB can probably be higher
#endif

// pattern defined as so to have it reread from flash each time
#define PATTERN "1234567890abcdefghijklmnopqrstuv"
#define CHUNK_SIZE 64  // max for the pico sdk usb buffer

#define WATCHDOG_TIMEOUT_US (5000 * 1000)  // 5 sec?
#define ROUGH_DELAY_BETWEEN_READS_S 5

uint32_t do_addition(PIO pio, uint sm, uint32_t a, uint32_t b) {
  pio_sm_put_blocking(pio, sm, a);
  pio_sm_put_blocking(pio, sm, b);
  return pio_sm_get_blocking(pio, sm);
}

int main() {
  stdio_init_all();

  sleep_ms(10000);  // wait 10s

  printf("Starting Test\n\n");

  printf("Using %u bytes of SRAM\n", SRAM_TO_USE_B);

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

  printf("Initializing PIO Addition\n");
  PIO pio = pio0;
  uint sm = 0;
  uint offset = pio_add_program(pio, &addition_program);
  addition_program_init(pio, sm, offset);

  printf("Starting watchdog timer\n");

  watchdog_enable(WATCHDOG_TIMEOUT_US, 1);

  // read and report
  while (1) {
    watchdog_update();
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    printf("\n[INFO] Reading at %lu:\n", timestamp);
    // transferring at max CHUNK_SIZE for pico's buffer for better speed
    for (size_t i = 0; i < SRAM_TO_USE_B; i += CHUNK_SIZE) {
      tud_cdc_write((void*)&(data[i]), CHUNK_SIZE);
      tud_cdc_write_flush();
    }
    printf("\n[INFO] Done (started: %lu).\n", timestamp);

    // wait 5s between reads but avoid watchdog timeout
    // combo with additions for PIO test
    for (int i = 0; i < ROUGH_DELAY_BETWEEN_READS_S; i++) {
      uint32_t a = i * 5;
      uint32_t b = i * 3;
      uint32_t result = do_addition(pio, sm, a, b);
      printf("%u + %u = %u\n", a, b, result);

      watchdog_update();
      sleep_ms(1000);
    }
  }

  return 0;
}