#ifndef _PLAT_GPIO_H_
#define _PLAT_GPIO_H_

#include <stdint.h>

typedef struct 
{
  uint32_t port;
  uint8_t pin;
} ARM_IO_PIN;

typedef struct
{
  ARM_IO_PIN pin1;
  ARM_IO_PIN pin2;
  ARM_IO_PIN pin3;
  ARM_IO_PIN pin4;
} IO_PIN_DEF;

extern void init_gpio();

#endif
