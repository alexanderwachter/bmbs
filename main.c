/******************************************************
*   This is a test file to check the build process
*   (c) Alexander Wachter
*   V1.0
*******************************************************/ 

#include "lib/time.h"
#include "arch/include/gpio.h"
#include "arch/include/plat_init.h"
#include <stdint.h>
#include "lib/com.h"
#include "lib/registration.h"

int main()
{
  time_t time_measurement;
  plat_init();
  reset_confirm();
  tick(&time_measurement);
  set_pin_high(PIN1);
  while(1)
  {
    while(!run_benchmark);
    tick(&time_measurement);
    if(run_benchmark <= bmbs_benchmark_count)
      bmbs_benchmarks[run_benchmark-1].entry();
    run_benchmark = 0;
    time_t time = tock(time_measurement);
    put_result(TOTAL_TIME_ID, time);
  }
  return 0;
}
