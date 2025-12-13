/**
 * @file main.cpp
 * @author Tyler Nielsen
 * @brief PIO block test, creates an addition machine and uses it with sample
 * input.
 * @version 0.1
 * @date 2025-12-13
 *
 */
#include <stdint.h>
#include <stdio.h>

#include "addition.pio.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

uint32_t do_addition(PIO pio, uint sm, uint32_t a, uint32_t b) {
  pio_sm_put_blocking(pio, sm, a);
  pio_sm_put_blocking(pio, sm, b);
  return pio_sm_get_blocking(pio, sm);
}

int main() {
  stdio_init_all();

  sleep_ms(10000);  // wait 10s

  printf("Starting Test\n\n");

  PIO pio = pio0;
  uint sm = 0;
  uint offset = pio_add_program(pio, &addition_program);
  addition_program_init(pio, sm, offset);

  printf("Doing the additions:\n");
  for (uint32_t i = 0; i < 20; i++) {
    uint32_t a = i * 2;
    uint32_t b = i * 3;
    uint32_t result = do_addition(pio, sm, a, b);
    printf("%u + %u = %u\n", a, b, result);
  }

  printf("Test Complete\n");

  while (1) {
    sleep_ms(5000);
    printf("Done.\n");
  }

  return 0;
}