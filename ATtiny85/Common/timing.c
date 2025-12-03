#include "timing.h"
#include <avr/interrupt.h>

// prescaler 8192
#define PRESCALER 8192
#define TIMER_RESET_VALUE_ONE_S (256 - (F_CPU / PRESCALER))

static volatile uint32_t _secs; 

ISR(TIMER1_OVF_vect){
    _secs++; 
    TCNT1 = TIMER_RESET_VALUE_ONE_S;
}

void init_timing(){
    _secs = 0;

    // setup timer 
    TCNT1 = TIMER_RESET_VALUE_ONE_S; 
    TCCR1 = _BV(CS13) | _BV(CS12) || _BV(CS11); // prescaler of 8192
    TIMSK |= _BV(TOIE1);

    sei(); 
}

uint32_t secs(){
    uint32_t m; 
    cli(); 
    m = _secs; 
    sei(); 
    return m; 
}