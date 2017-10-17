#include "../include/interrupt.h"
#include <msp430.h>

void disable_interrupt()
{
  __bic_SR_register(GIE);
}

void enable_interrupt()
{
  __bis_SR_register(GIE);
}

uint8_t get_interrupt_state()
{
  return (_get_SR_register() & GIE) && 1;
}

void reset_device()
{
  PMMCTL0 |= PMMSWPOR;
}
