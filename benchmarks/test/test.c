
#include "../../lib/registration.h"
#include "../../lib/xprintf.h"
#include "../../lib/time.h"
#include "../../arch/include/timer.h"
#include "../../arch/include/gpio.h"
#include "../../lib/com.h"

/*Entry function*/
void benchmarktest(void)
{
  
  time_t measurement_1;
  
  //print i.e. for debugging
  xprintf("Benchmark test\n");
  
  //start time measurement
  int i;
  tick(&measurement_1);
  for(i = 0; i < 5000; i++)
  {
    toggle_pin(PIN3);
    toggle_pin(PIN1);

    time_t actual_time = get_actual_time();

    while(get_actual_time() <= actual_time + (time_t)50000);
  }
  //get passed time since tick()
  time_t passed_time = tock(measurement_1);
  put_result(1, passed_time);
}

/*Register Benchmark:    name        , version major, version minor, entry function */
BMBS_REGISTER_BENCHMARK(Benchmarktest, 1            , 0            , benchmarktest)
