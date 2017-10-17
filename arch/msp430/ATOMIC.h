#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#define ATOMIC(x) \
	uint16_t sr_save = _get_SR_register(); \
	__bic_SR_register(GIE); \
	x \
	__bis_SR_register(sr_save & GIE);
  
#endif
