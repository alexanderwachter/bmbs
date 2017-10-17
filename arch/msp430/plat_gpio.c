#include <msp430.h>
#include "config.h"
#include "MSPWare/MSP430F5xx_6xx/driverlib.h"
#include "../include/gpio.h"

MSP_IO_PIN_DEF out_pins = {
  {GPIO_PORT_P1, GPIO_PIN0}, // RED LED
  {GPIO_PORT_P4, GPIO_PIN7}, // GREEN LED
  {GPIO_PORT_P4, GPIO_PIN1},
  {GPIO_PORT_P4, GPIO_PIN2}};

void init_gpio()
{
  MSP_IO_PIN *msp_io_pin;
  MSP_IO_PIN *last_msp_io_pin = (MSP_IO_PIN*)&out_pins + (sizeof(MSP_IO_PIN_DEF)/sizeof(MSP_IO_PIN));
  for(msp_io_pin = (MSP_IO_PIN*)&out_pins; msp_io_pin < last_msp_io_pin; msp_io_pin++)
  {
    GPIO_setAsOutputPin(msp_io_pin->port,msp_io_pin->pin);
    GPIO_setOutputLowOnPin(msp_io_pin->port,msp_io_pin->pin);
  }
}
void set_pin_high(OUT_PIN pin)
{
  MSP_IO_PIN *msp_io_pin = ((MSP_IO_PIN*)&out_pins) + pin;
  GPIO_setOutputHighOnPin(msp_io_pin->port,msp_io_pin->pin);
}
void set_pin_low(OUT_PIN pin)
{
  MSP_IO_PIN *msp_io_pin = ((MSP_IO_PIN*)&out_pins) + pin;
  GPIO_setOutputLowOnPin(msp_io_pin->port,msp_io_pin->pin);
}
void toggle_pin(OUT_PIN pin)
{
  MSP_IO_PIN *msp_io_pin = ((MSP_IO_PIN*)&out_pins) + pin;
  GPIO_toggleOutputOnPin(msp_io_pin->port,msp_io_pin->pin);
}
