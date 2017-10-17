#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "plat_gpio.h"
#include "MSPWare/MSP430F5xx_6xx/gpio.h"
#include <msp430.h>

#define fMCLK             24000000UL
#define fMSCLK            24000000UL
#define XT2_FREQ          4000000UL
#define XT1_FREQ          32768UL
#define TIMER_PERIOD      0xFFFF
#define UART_PORT         GPIO_PORT_P3    ///< the MCU port to use for the UART
#define UART_TX_PIN       GPIO_PIN3      ///< the port pin at UART_PORT to use as TX line
#define UART_RX_PIN       GPIO_PIN4      ///< the port pin at UART_PORT to use as RX line
#define UART_BAUDRATE     115200ul       ///< the desired UART baudrate in bps
#define USCIA_NR          0             ///< the desired USCI module

extern MSP_IO_PIN_DEF out_pins; //defined in plat_gpio.c

#endif
