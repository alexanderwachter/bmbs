/**
 * @file time.h
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
 * Time measurement
 */
#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

typedef uint64_t time_t;

extern volatile time_t _sys_time;
extern volatile time_t _tick_time;

/**
* Function to start a time measurement
* 
* This functions starts a time measurement of a given instance.
* you can start as many measurements as you have instances.
* Restarting with already set instance is possible.
*
* @param instance pointer to instance
*/
extern void tick(time_t* instance);

/**
* Function to get the elapsed time since corresponding tick()
*
* @param instance pointer to instance
* 
* @return elapsed time
*/
extern time_t tock(time_t instance);

/**
* wait for x ms
*
* @param ms time in ms
*/
extern void wait(time_t ms);

/**
* Stops the timer
*
*/
extern void start_timer_off_section();

/**
* Start the timer
* 
* Start the timer if this function is called as often as start_timer_off_section()
*/
extern void end_timer_off_section();


#endif
