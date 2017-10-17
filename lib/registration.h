/**
 * @file registration.h
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
 * Register benchmark in bmbs
 */
#ifndef _REGISTRATION_H_
#define _REGISTRATION_H_

#include <stdint.h>

/**
* Type definition of an benchmark entry function
*/
typedef void (*benchmark_entry)(void);

/**
* Struct of a benchmark registration
* 
* This struct represents a executable benchmark.
* The framework calls entry() when an execution is triggered
*/
typedef struct {
  char* name;             /**< name of the benchmark */
  uint8_t version_major;  /**< major version number */
  uint8_t version_minor;  /**< minor version number */
  benchmark_entry entry;  /**< function that is called on execution */
} __attribute__((packed)) Benchmark;

/**
* Macro tu registe a benchmark
* 
* This macro registers a benchmark in the system.
* Only registregistered benchmarks are visible in bmbs
*
* @param _name name for this benchmark
* @param _version_major major version number
* @param _version_minor minor version number
* @param _entry function that is called on execution
*/
#define BMBS_REGISTER_BENCHMARK(_name, _version_major, _version_minor, _entry) \
  extern void _entry(void);																		                   \
  volatile Benchmark const __benchmark_##_name __attribute__((section(".bmbs_benchmarks"))) = { \
    .name			= #_name,	             \
    .version_major = _version_major, \
    .version_minor = _version_minor, \
    .entry			= &_entry,					 \
};

extern uint8_t const bmbs_benchmark_count;  //total number of registered benchmarks
extern Benchmark const bmbs_benchmarks[];   //registered benchmarks

#endif
