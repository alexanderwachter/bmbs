/**
 * @file gpio.h
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
 * Platform independent output-pin control.
 * Mappin is done in architecture specific part.
 */
#ifndef _GPIO_H_
#define _GPIO_H_

/**
* Enumeration of available pins
*/

#ifdef PIN1
  #undef PIN1
  #undef PIN2
  #undef PIN3
  #undef PIN4
#endif

typedef enum {PIN1, PIN2, PIN3, PIN4} OUT_PIN;

/**
* Function to set the output of "pin" to high state
*
* @param pin the pin to set high
*/
extern void set_pin_high(OUT_PIN pin);

/**
* Function to set the output of "pin" to low state
*
* @param pin the pin to set low
*/
extern void set_pin_low(OUT_PIN pin);

/**
* Function to toggle the state of "pin"
*
* @param pin the pin to set high
*/
extern void toggle_pin(OUT_PIN pin);

#endif
