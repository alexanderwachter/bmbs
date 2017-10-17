# Content
Implementation of architecture depending functions. With this functions we get a uniform and hardware independent interface to the hardware.

## config
Platform depending configurations.

## plat_gpio

### plat_gpio.h
Structs for PIN - Hardware mapping.

### plat_gpio.c
PIN - Hardware mapping, set/ toggle output pin.

 * init_gpio()
 * set_pin_high(PIN)
 * set_pin_low(PIN)
 * toggle_pin(PIN)

## plat_init
Initialization of Clock system, watchdog, enable global interrupts.
Call initializer

  * init_std_io()
  * init_gpio()
  * plat_init_timer()

## plat_interrupt
Functions for interrupt handling and reset:

  * disable_interrupt()
  * enable_interrupt()
  * get_interrupt_state()
  * reset_device()

## plat_timer

### plat_time.h
Clock prescaler calculation, init_function_prototype.

### plat_timer.c
Timer initialization, Timer interrupt service routine, get actual time implementation.

  * plat_init_timer()
  * Timer ISR
  * get_actual_time()

## plat_uart

### plat_uart.h
Hardware UART settings, init function prototype.

### plat_uart.c
Uart initialization, gut/get char, UART ISR

  * init_stdio()
  * int plat_getc()
  * plat_putc(char)
  * UARt ISR