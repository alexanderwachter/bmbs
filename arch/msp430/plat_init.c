#include "../include/plat_init.h"
#include <msp430.h>
#include <stdint.h>
#include "config.h"
#include "plat_uart.h"
#include "plat_timer.h"
#include "plat_gpio.h"
#include "MSPWare/MSP430F5xx_6xx/driverlib.h"

void plat_init()
{
  /*disable Watchdog*/
  WDT_A_hold(WDT_A_BASE);
  
  /*============ Init Core Voltage ===============*/
  /*Set VCORE regarding to datasheet*/
  uint8_t level;
  if (fMCLK <= 8000000UL)
    level = PMMCOREV_0;
  else if (fMCLK <= 12000000UL)
    level = PMMCOREV_1;
  else if (fMCLK <= 20000000UL)
    level = PMMCOREV_2;
  else
    level = PMMCOREV_3;

  PMM_setVCore(level);
  
  /*===============Init clocks====================*/
  /*Set Crystal IOs*/
  //GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3 + GPIO_PIN5);
  GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2 + GPIO_PIN4 + GPIO_PIN3 + GPIO_PIN5);

  /*Turn non Crystals*/
  UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
  UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_0);

  /*Only for calculation*/
  UCS_setExternalClockSource(XT1_FREQ,XT2_FREQ);

  /*Set FLL ref to XT2*/
  UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);   

  /*Init FLL and wait for transient condition*/
  UCS_initFLLSettle(24000, 24000000UL/XT2_FREQ);

  /*Set MCLK to DCO*/
   UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

  /*Set SMCLK* to DCO*/
  UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

  /*Set ACLK* to XT1*/
  UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

  init_std_io();
  init_gpio();
  plat_init_timer();
  __bis_SR_register(GIE);

}
