#include "inc/hw_types.h"
#include "../include/interrupt.h"
#include "driverlib/cpu.h"
#include "driverlib/sysctl.h"

void disable_interrupt()
{
  CPUcpsie();
}

void enable_interrupt()
{
  CPUcpsid();
}

uint8_t get_interrupt_state()
{
  return CPUprimask() ? 1 : 0;
}

void reset_device()
{
  SysCtlReset();
}
