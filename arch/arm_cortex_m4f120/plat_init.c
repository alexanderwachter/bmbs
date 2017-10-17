#include <stdint.h>

#include "../include/plat_init.h"
#include "config.h"
#include "plat_uart.h"
#include "plat_timer.h"
#include "plat_gpio.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

void plat_init()
{ 
  /*===============Init clocks====================*/
  SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);
 
  init_std_io();
  init_gpio();
  plat_init_timer();
  IntMasterEnable();
}
