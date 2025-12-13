/**
 * Do nothing for Passive Radiation Test
 *
 */
#include <pico/stdlib.h>

#define BUILT_IN_LED_PIN 25

int main() {
  gpio_init(BUILT_IN_LED_PIN);
  gpio_set_dir(BUILT_IN_LED_PIN, GPIO_OUT);
  gpio_put(BUILT_IN_LED_PIN, 1);  // turn on built-in

  while (1);  // do nothing

  return 0;
}