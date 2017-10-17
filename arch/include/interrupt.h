/**
 * @file interrupt.h
 * @author  Alexander Wachter <alexander.wachter@student.tugraz.at>
 * @version 1.0
 *
 * @section LICENSE
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Alexander Wachter
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Platform independent global interrupt control.
 */
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stdint.h>

/**
* Function to disable global interrupts
*/
void disable_interrupt();

/**
* Function to enable global interrupts
*/
void enable_interrupt();

/**
* Function to disable global interrupts
* 
* @return 1 if interrupt is enabled, else 0
*/
uint8_t get_interrupt_state();

/**
* Function to reset the device
* 
* If this function is called, a power on reset is executed.
*/
void reset_device();

#endif
