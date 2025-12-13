/**
 * @file main.cpp
 * @author Tyler Nielsen
 * @brief Simple transmit over USB interface
 * @version 0.1
 * @date 2025-12-13
 *
 */
#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"

int main() {
  stdio_init_all();

  sleep_ms(10000);  // wait 10s

  printf("Starting Test\n\n");

  // read and report
  int it = 0;
  while (1) {
    printf("Iteration %d\n", it);
    it++;
  }

  return 0;
}