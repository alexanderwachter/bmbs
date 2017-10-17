#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "../include/timer.h"
#include "plat_timer.h"
#include "../../lib/time.h"
#include "ATOMIC.h"

void plat_init_timer(void)
{
  TCCR1B |= TIMER_CLOCK_CONFIG;
  TIMSK1 |= (1<<TOIE1);
}

ISR(TIMER1_OVF_vect)
{
  _sys_time += TIMER_OV_VALUE;
}

void plat_stop_timer()
{
  TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));
}

void plat_start_timer()
{
  TCCR1B |= TIMER_CLOCK_CONFIG;
}

time_t get_actual_time()
{
  time_t actual_time;
  ATOMIC(actual_time = _sys_time + TCNT1;)
  /*Divide by 2 because the timer counts with 2MHz instead of 1MHz*/
  actual_time >>= 1;
  return actual_time;
}
