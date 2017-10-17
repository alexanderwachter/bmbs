#include "config.h"
#include "plat_uart.h"
#include "../include/std_io.h"
#include "../../lib/com.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

void plat_putc(char c)
{
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = c;
}

int plat_getc()
{
  while (!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void init_std_io()
{
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  #if USE_2X
    UCSR0A |= (1 << U2X0);
  #else
    UCSR0A &= ~(1 << U2X0);
  #endif

  UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);  // enable receiver, transmitter and rx interrupt
  UCSR0C  = (1 << UCSZ01) | (1 << UCSZ00);
}

ISR(USART_RX_vect)
{
  char c = UDR0;
  get_char_handler(c);
}
