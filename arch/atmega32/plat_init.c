#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/plat_init.h"
#include <stdint.h>
#include "config.h"
#include "plat_uart.h"
#include "plat_timer.h"
#include "plat_gpio.h"
#include <avr/wdt.h>

void plat_init()
{
  wdt_disable();
  init_std_io();
  init_gpio();
  plat_init_timer();
  sei();
}


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void)
{
  MCUSR = 0;
  wdt_disable();
}
