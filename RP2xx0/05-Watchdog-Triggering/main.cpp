#include <stdint.h>
#include <stdio.h>

#include "hardware/watchdog.h"
#include "pico/stdlib.h"

#define WATCHDOG_TIMEOUT_US (5000 * 1000)  // 5 sec?

int main() {
  stdio_init_all();

  if (watchdog_enable_caused_reboot()) {
    printf("Reboot caused by Watchdog\n\n");
    sleep_ms(1000);
  } else {
    printf("First boot\n");
    sleep_ms(10000);  // wait 10s
  }

  printf("Starting Test\n\n");

  // set up watchdog
  watchdog_enable(WATCHDOG_TIMEOUT_US, 1);

  // read and report
  int it = 0;
  while (1) {
    printf("Iteration %d\n", it);
    if (it < 6) {
      watchdog_update();
    }

    if (it == 6) {
      printf("Watchdog freeze\n");
    }

    it++;
  }

  return 0;
}