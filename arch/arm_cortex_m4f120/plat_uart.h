#ifndef _PLAT_UART_H_
#define _PLAT_UART_H_

#define CAT(a, b, c) a ## b ## c
#define USCIA_BASE(x)     CAT(USCI_A,x,_BASE)
#define USCIA_VECTOR(x)   CAT(USCI_A,x,_VECTOR)  
#define UCAxIV(x)         CAT(UCA,x,IV)

extern void init_std_io(void);

#endif
