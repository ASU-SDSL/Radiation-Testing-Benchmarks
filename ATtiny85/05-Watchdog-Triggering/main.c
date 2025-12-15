/**
 * @file main.c
 * @author Tyler Nielsen
 * @brief Triggers the watchdog after a set period of time.
 * @version 0.1
 * @date 2025-12-13
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 1000000UL  // 1 MHz for util/delay and timing
#endif

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "../Common/timing.h"
#include "../Common/usi_uart.h"

int main() {
  uint8_t mcusr_copy = MCUSR;
  MCUSR = 0;  // clear all reset flags
  wdt_disable();

  // timing and uart init
  init_timing();

  // set PB4 to output low
  DDRB |= _BV(DDB4);
  PORTB &= ~_BV(PB4);
  PINB |= _BV(PB4);  // toggle

  _delay_ms(1000);

  // check if we were reset by the watchdog
  if (mcusr_copy & _BV(WDRF)) {
    usiserial_printf("Watchdog Reset Detected\n");
  }

  usiserial_printf("Starting Watchdog Triggering Test\n");

  // enable watchdog timer module
  wdt_enable(WDTO_4S);  // set for 4s timeout

  int it = 0;
  // read and report every 1s
  while (1) {
    if (it < 10) {
      wdt_reset();
    }
    usiserial_printf("Iteration: %d\n", it);

    if (it == 10) {
      wdt_reset();
      usiserial_printf("Freezing watchdog\n");
    }

    PINB |= _BV(PB4);  // toggle led
    it++;
    _delay_ms(500);
  }

  return 0;
}