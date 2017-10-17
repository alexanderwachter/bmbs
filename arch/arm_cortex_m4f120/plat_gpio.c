#include "config.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "../include/gpio.h"

const IO_PIN_DEF out_pins = {
  {GPIO_PORTF_BASE, GPIO_PIN_1}, // RED LED
  {GPIO_PORTF_BASE, GPIO_PIN_2}, // BLUE LED
  {GPIO_PORTF_BASE, GPIO_PIN_3}, // GREEN LED
  {GPIO_PORTF_BASE, GPIO_PIN_0}};

void init_gpio()
{
  ARM_IO_PIN *arm_io_pin = (ARM_IO_PIN*)&out_pins;
  ARM_IO_PIN *last_arm_io_pin = arm_io_pin + (sizeof(IO_PIN_DEF)/sizeof(ARM_IO_PIN));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  for(; arm_io_pin < last_arm_io_pin; arm_io_pin++)
  {
    GPIOPinTypeGPIOOutput(arm_io_pin->port, arm_io_pin->pin);
    GPIOPinWrite(arm_io_pin->port, arm_io_pin->pin, 0);
  }
  
}
void set_pin_high(OUT_PIN pin)
{
  ARM_IO_PIN *arm_io_pin = ((ARM_IO_PIN*)&out_pins) + pin;
  GPIOPinWrite(arm_io_pin->port, arm_io_pin->pin, arm_io_pin->pin);
}
void set_pin_low(OUT_PIN pin)
{
  ARM_IO_PIN *arm_io_pin = ((ARM_IO_PIN*)&out_pins) + pin;
  GPIOPinWrite(arm_io_pin->port, arm_io_pin->pin, 0);
}
void toggle_pin(OUT_PIN pin)
{
  ARM_IO_PIN *arm_io_pin = ((ARM_IO_PIN*)&out_pins) + pin;
  uint32_t state = GPIOPinRead(arm_io_pin->port, arm_io_pin->pin);
  GPIOPinWrite(arm_io_pin->port, arm_io_pin->pin, ~state);
}
