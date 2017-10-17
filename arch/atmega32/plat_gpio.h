#ifndef _PLAT_GPIO_H_
#define _PLAT_GPIO_H_

#include <stdint.h>

typedef struct 
{
  volatile uint8_t *port;
  uint8_t pin;
} AVR_IO_PIN;

typedef struct
{
  AVR_IO_PIN pin1;
  AVR_IO_PIN pin2;
  AVR_IO_PIN pin3;
  AVR_IO_PIN pin4;
} __attribute__((packed)) AVR_IO_PIN_DEF;

extern void init_gpio();

#endif
