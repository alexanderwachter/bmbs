#ifndef _PLAT_TIMER_H_
#define _PLAT_TIMER_H_

#include "config.h"

extern void plat_init_timer(void);

#define TIMER_FREQ (1000000ul)
#define TIMER_CLOCK_PRESCALER 64
#define TIMER_CLOCK_CONFIG (1<<CS11)
#define TIMER_OV_VALUE 0xFFFF;

#endif
