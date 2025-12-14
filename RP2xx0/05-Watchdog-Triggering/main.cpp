/**
 * @file main.cpp
 * @author Tyler Nielsen
 * @brief Triggers the watchdog after a set period of time.
 * @version 0.1
 * @date 2025-12-13
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdint.h>
#include <stdio.h>

#include "hardware/watchdog.h"
#include "pico/stdlib.h"

#define WATCHDOG_TIMEOUT_US (5000 * 1000)  // 5 sec?

#define BUILT_IN_LED_PIN 25

int main() {
  stdio_init_all();

  gpio_init(BUILT_IN_LED_PIN);
  gpio_set_dir(BUILT_IN_LED_PIN, true);
  gpio_put(BUILT_IN_LED_PIN, 0);  // start off

  sleep_ms(10000);  // 10s

  if (watchdog_enable_caused_reboot()) {
    printf("Reboot caused by Watchdog\n\n");
    // 3 quick blinks ~1s total
    for (int i = 0; i < 3; i++) {
      gpio_put(BUILT_IN_LED_PIN, 1);
      sleep_ms(150);
      gpio_put(BUILT_IN_LED_PIN, 0);
      sleep_ms(150);
    }

  } else {
    printf("First boot\n");
    sleep_ms(2000);  // wait 2s
  }

  printf("Starting Test\n\n");

  // set up watchdog
  watchdog_enable(WATCHDOG_TIMEOUT_US, 1);

  // read and report
  int it = 0;
  while (1) {
    printf("Iteration %d\n", it);
    if (it < 10) {
      watchdog_update();
      // toggle built in LED based on it iteration counter
      gpio_put(BUILT_IN_LED_PIN, it & 0x1);
    }

    if (it == 10) {
      printf("Watchdog freeze\n");
    }

    sleep_ms(500);
    it++;
  }

  return 0;
}
// reboot expected after iteration 42 for pico 2s and 25 for pico 1s