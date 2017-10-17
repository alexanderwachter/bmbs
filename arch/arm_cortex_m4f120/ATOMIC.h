#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#define ATOMIC(x) \
  {\
	tBoolean int_state = IntMasterDisable(); \
	x \
	if(!int_state) {IntMasterEnable();} \
  }
  
#endif
