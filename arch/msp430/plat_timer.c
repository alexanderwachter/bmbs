#include <msp430.h>
#include "config.h"
#include "plat_timer.h"
#include "MSPWare/MSP430F5xx_6xx/driverlib.h"
#include "../../lib/time.h"
#include "ATOMIC.h"

void plat_init_timer(void)
{
   Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER;
    param.timerPeriod = TIMER_PERIOD;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_SKIP_CLEAR;
    param.startTimer = false;
    
    Timer_A_initUpMode(TIMER_A0_BASE, &param);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_A0_VECTOR)))
#endif
void TIMER0_A1_ISR(void)
{
    Timer_A_stop(TIMER_A0_BASE);
    _sys_time += TIMER_PERIOD;
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

void plat_stop_timer()
{
  Timer_A_stop(TIMER_A0_BASE);
}

void plat_start_timer()
{
  Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

time_t get_actual_time()
{
  time_t actual_time;
  ATOMIC(actual_time = _sys_time +  HWREG16(TIMER_A0_BASE + OFS_TAxR);)
  return actual_time;
}
