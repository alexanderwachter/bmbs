/**
 * @file time.c
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
#include "time.h"
#include "../arch/include/timer.h"

volatile time_t _sys_time = 0;
volatile time_t _tick_time;
volatile uint16_t _timer_off_cnt = 0;

/** \addtogroup time
 *  @{
 */
 
void tick(time_t* instance)
{
  *instance = get_actual_time();
}

time_t tock(time_t instance)
{
  return get_actual_time() - instance;
}

void wait(time_t ms)
{
  time_t start = get_actual_time();
  ms *= 1000;
  while(get_actual_time() < start + ms);
}

void start_timer_off_section()
{
  if(! _timer_off_cnt++)
    plat_stop_timer();
}

void end_timer_off_section()
{
  if(! --_timer_off_cnt)
    plat_start_timer();
}

/** @}*/
