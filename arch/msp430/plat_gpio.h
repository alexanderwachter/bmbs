#ifndef _PLAT_GPIO_H_
#define _PLAT_GPIO_H_

#include <stdint.h>

typedef struct 
{
  uint16_t port;
  uint16_t pin;
} MSP_IO_PIN;

typedef struct
{
  MSP_IO_PIN pin1;
  MSP_IO_PIN pin2;
  MSP_IO_PIN pin3;
  MSP_IO_PIN pin4;
} __attribute__((packed)) MSP_IO_PIN_DEF;

extern void init_gpio();

#endif
