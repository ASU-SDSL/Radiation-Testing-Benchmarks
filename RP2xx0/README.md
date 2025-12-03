# RP2xx0 Benchmarks 

| # | Name | Description | Target MCU(s) | Written |
|---|------|-------------|----------------|---------|
| 0 | Passive No Operation | Does nothing | All | |
| 1 | SRAM Single Write Continuous Read | Writes a pattern to unused SRAM blocks, then continuously cycles through them and reports via USB/UART | RP2040 / RP2350 | |
| 2 | Simple UART | Basic UART/USB output; validates UART/USB integrity for RP2xx0 and USI integrity for ATtiny85 | All | |
| 3 | SRAM Fixed N Cycle Read/Write | Writes a pattern to unused SRAM, then repeatedly reads back and reports via USB/UART or USI/UART | All | |
| 4 | Timer Pulses | Uses built-in timer modules for delay and interrupt generation | All | |
| 5 | PIO Test | Tests PIO block integrity and operation on RP family devices | RP2040 / RP2350 | |
| 6 | Watchdog Triggering | Sets up the watchdog and intentionally triggers reboots | All | |
