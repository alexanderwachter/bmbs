#include "config.h"
#include "plat_uart.h"
#include "../include/std_io.h"
#include "../../lib/com.h"
#include <msp430.h>
#include <stdint.h>
#include "MSPWare/MSP430F5xx_6xx/driverlib.h"

void plat_putc(char c)
{
	USCI_A_UART_transmitData(USCIA_BASE(USCIA_NR), c);
}

int plat_getc()
{
	return USCI_A_UART_receiveData(USCIA_BASE(USCIA_NR));
}

void init_std_io()
{
	uint32_t N_BRATE = fMSCLK /  UART_BAUDRATE;
  USCI_A_UART_initParam param = {0};

  if(N_BRATE >= 16) //use oversampling
  {
    param.firstModReg = (N_BRATE & 0x0000000F) + 1;
    param.secondModReg = ((N_BRATE & 0x0000000F) + 1) >> 1;
    param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    param.clockPrescalar = (N_BRATE >> 4);

  }
  else
  {
    param.firstModReg = ((N_BRATE >> 1)  & 0x0000000F);
    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
    param.clockPrescalar = (N_BRATE >> 4);
  }
  
  param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
  param.parity = USCI_A_UART_NO_PARITY;
  param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
  param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
  param.uartMode = USCI_A_UART_MODE;
  
  GPIO_setAsPeripheralModuleFunctionInputPin(
      UART_PORT,
      UART_TX_PIN + UART_RX_PIN
  );

  USCI_A_UART_init(USCIA_BASE(USCIA_NR), &param);
  USCI_A_UART_enable(USCIA_BASE(USCIA_NR));
  USCI_A_UART_clearInterrupt(USCIA_BASE(USCIA_NR), USCI_A_UART_RECEIVE_INTERRUPT);
  USCI_A_UART_enableInterrupt(USCIA_BASE(USCIA_NR), USCI_A_UART_RECEIVE_INTERRUPT);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIA(VEKTOR)
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCIA_VECTOR(USCIA_NR))))
#endif
void USCI_ISR(void)
{
    char c;
    switch(UCAxIV(USCIA_NR))
    {
    //Vector 2 - RXIFG
    case 2:
        c = USCI_A_UART_receiveData(USCIA_BASE(USCIA_NR));
        get_char_handler(c);
        break;
    default: break;
    }
}

