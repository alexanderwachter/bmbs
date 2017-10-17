#include "template.h"
#include <registration.h>
#include <xprintf.h>
#include <time.h>
#include <timer.h>
#include <com.h>

/*Entry function*/
void template(void)
{
  //measurement instance 
  time_t measurement_1;
  
  //print i.e. for debugging
  xprintf("Template test\n");
  
  //start time measurement
  tick(&measurement_1);
  
  /*do some benchmark things*/
  //get the actual system time in micro seconds
  time_t actual_time = get_actual_time();
  
  //wait for 1 second
  while(get_actual_time() < actual_time + 1000000);
  
  
  //get passed time since tick()
  time_t passed_time = tock(measurement_1);
  put_result(0, passed_time);
}

/*Register Benchmark:    name            , version major, version minor, entry function */
BMBS_REGISTER_BENCHMARK(Benchmarktemplate, 0            , 1            , template)
