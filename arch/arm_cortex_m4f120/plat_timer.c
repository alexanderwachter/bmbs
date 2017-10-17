#include "config.h"
#include "plat_timer.h"
#include "../../lib/time.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

uint64_t timer_prescaler;

void plat_init_timer(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
  TimerConfigure(WTIMER0_BASE, TIMER_CFG_PERIODIC_UP);
  timer_prescaler = SysCtlClockGet() / 1000000UL;
  TimerEnable(WTIMER0_BASE, TIMER_A);
}

void plat_stop_timer()
{
  TimerDisable(WTIMER0_BASE, TIMER_A);
}

void plat_start_timer()
{
  TimerEnable(WTIMER0_BASE, TIMER_A);
}

time_t get_actual_time()
{
  time_t value;
  value = TimerValueGet64(WTIMER0_BASE) / timer_prescaler;
  return value;
}
