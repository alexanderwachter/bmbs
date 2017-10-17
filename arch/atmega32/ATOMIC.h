#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define ATOMIC(x) \
	uint8_t sr_save = SREG; \
	cli(); \
	x \
	SREG = sr_save;
  
#endif
