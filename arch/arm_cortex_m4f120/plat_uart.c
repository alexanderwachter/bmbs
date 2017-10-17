#include "config.h"
#include "plat_uart.h"
#include "../include/std_io.h"
#include "../../lib/com.h"
#include <stdint.h>


#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

#include "../include/gpio.h"

#include "../../lib/time.h"

void plat_putc(char c)
{
	UARTCharPut(UART_BASE, c);
  SysCtlDelay(50);
}

int plat_getc()
{
	return UARTCharGetNonBlocking(UART_BASE);
}

void init_std_io()
{
	//enable periphery
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), UART_BAUDRATE, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntEnable(INT_UART0);
	UARTIntEnable(UART_BASE, UART_INT_RX | UART_INT_RT);
}

void UART_ISR(void)
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(UART_BASE, true);
	UARTIntClear(UART_BASE, ulStatus);

	while(UARTCharsAvail(UART_BASE))
	{
    //UARTCharPut(UART_BASE, UARTCharGetNonBlocking(UART_BASE)); //echo
		get_char_handler(UARTCharGetNonBlocking(UART_BASE));
	}
}
