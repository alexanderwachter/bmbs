#include "../include/interrupt.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>

void disable_interrupt()
{
  cli();
}

void enable_interrupt()
{
  sei();
}

uint8_t get_interrupt_state()
{
  return (SREG & (1 << 7)) && 1;
}

void reset_device()
{
  wdt_enable(WDTO_15MS);
  while(1);
}
