#include <avr/io.h>
#include "../include/gpio.h"
#include "plat_gpio.h"

AVR_IO_PIN_DEF out_pins = {
  {&PORTB, (1 << PINB5)},   // Yellow LED
  {&PORTD, (1 << PIND1)},
  {&PORTD, (1 << PIND2)},
  {&PORTD, (1 << PINB3)}};

void init_gpio()
{
  AVR_IO_PIN *avr_io_pin;
  AVR_IO_PIN *last_avr_io_pin = (AVR_IO_PIN*)&out_pins + (sizeof(AVR_IO_PIN_DEF)/sizeof(AVR_IO_PIN));
  for(avr_io_pin = (AVR_IO_PIN*)&out_pins; avr_io_pin < last_avr_io_pin; avr_io_pin++)
  {
    *(avr_io_pin->port - 1) |= avr_io_pin->pin;
    *(avr_io_pin->port)     &= ~(avr_io_pin->pin);
  }
}
void set_pin_high(OUT_PIN pin)
{
  AVR_IO_PIN *avr_io_pin = ((AVR_IO_PIN*)&out_pins) + pin;
  *(avr_io_pin->port) |= avr_io_pin->pin;
}
void set_pin_low(OUT_PIN pin)
{
  AVR_IO_PIN *avr_io_pin = ((AVR_IO_PIN*)&out_pins) + pin;
  *(avr_io_pin->port) &= ~(avr_io_pin->pin);
}
void toggle_pin(OUT_PIN pin)
{
  AVR_IO_PIN *avr_io_pin = ((AVR_IO_PIN*)&out_pins) + pin;
  if(*(avr_io_pin->port) & avr_io_pin->pin)
    *(avr_io_pin->port) &= ~(avr_io_pin->pin);
  else
    *(avr_io_pin->port) |= avr_io_pin->pin;
}
