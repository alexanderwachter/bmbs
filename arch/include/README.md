# Content
Prototypes for architecture independent hardware access.

## gpio.h
Interface for output pins. There are four hardware independent pins defined as "OUT_PIN" enum. The mapping, from "OUT_PIN" to real hardware is done in the architecture specific directory / plat_gpio.h.

## interrupt.h
Interface to enable, disable, get state of the global interrupt and reset the device.

## plat_init.h
Platform initialization. The plat_init function is called once at startup. Here you initialize gpio, uart, clocking. etc...

## stdio.h
Platform interface to send/receive a single char.

## timer.h
Platform interface to get the actual system time in micro seconds.