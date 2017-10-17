#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "plat_gpio.h"


#define fMCLK             50000000UL
#define XT2_FREQ          16000000UL
#define XT1_FREQ          32768UL
#define TIMER_PERIOD      0xFFFF

#define UART_PORT         SYSCTL_PERIPH_GPIOA   ///< the MCU port to use for the UART
#define UART_TX_PIN       GPIO_PA1_U0TX      	///< the port pin at UART_PORT to use as TX line
#define UART_RX_PIN       GPIO_PA0_U0RX      	///< the port pin at UART_PORT to use as RX line
#define UART_BAUDRATE     115200ul       		///< the desired UART baudrate in bps
#define UART_MOD          SYSCTL_PERIPH_UART0	///< the desired UART module
#define UART_BASE         UART0_BASE


//extern MSP_IO_PIN_DEF out_pins; //defined in plat_gpio.c



#endif
