#include "Benchmarks.h"
#include <iostream>
#include <unistd.h>


#ifdef DEBUG
  #undef DEBUG
  #define DEBUG(...) std::cout << "DEBUG " << __LINE__ << ": " << __VA_ARGS__ << " in file "  << __FILE__ << std::endl;
#else
#define DEBUG(...)
#endif

void Benchmarks::getBenchmarks(TLVstateMachine& com)
{
  this->benchmarks.clear();
  Command_data command = {.command = GET_BMS, .length = 0};
  Response_data response = {.command = BMS_RESPONSE};
  response.data = NULL;
  DEBUG("Send GET_BMS command")
  if(!com.SendCommand(command, &response, std::chrono::seconds(1)))
    throw "Timeout reading benchmarks";
  DEBUG("Got response")
  char *pch;
  pch = strtok(response.data, ";");
  while (pch != NULL)
  {
    DEBUG("Pushback benchmark")
    this->benchmarks.push_back(Benchmark(pch));
    pch = strtok(NULL, ";");
  }
  delete response.data;
}

void Benchmarks::printBenchmarks(std::ostream& output)
{
  if(!this->benchmarks.empty())
  {
    print_mutex->lock();
    for(unsigned int i = 0; i < this->benchmarks.size(); i++)
    {
      output << "[" << i << "] " << this->benchmarks[i].name << std::endl;
    }
    print_mutex->unlock();
  }
  else
  {
    output << "no benchmarks" << std::endl;
  }
}

bool Benchmarks::runBenchmark(unsigned char nr, TLVstateMachine& com, std::chrono::seconds timeout)
{
  startBenchmark(nr, com);
  Bm_result tmp_res;
  do
  {
    if(!getResult(com, &tmp_res, timeout))
      return false;
    benchmarks[nr].results.push_back(tmp_res);
    printResult(tmp_res, std::cout);
  } while(tmp_res.id != TOTAL_TIME_ID);
  return true;
}

void Benchmarks::startBenchmark(unsigned char nr, TLVstateMachine& com)
{
  com.clearBuffer();
  if(nr >= this->benchmarks.size())
    throw std::invalid_argument("nr > number of benchmarks");
  Command_data command = {.command = START_BM, .length = 1};
  command.data = (char*)(&nr);
  Response_data response = {.command = START_CONFIRM};
  print_mutex->lock();
  if(!com.SendCommand(command, &response, std::chrono::seconds(1)))
    throw "Timeout reading benchmarks";
  else
  {
    std::cout << "------Benchmark " << response.data << " started------" << std::endl << std::endl;
  }
  print_mutex->unlock();
  delete response.data;
}

bool Benchmarks::getResult(TLVstateMachine&com, Bm_result* res, std::chrono::seconds timeout)
{
  Response_data response = {.command = RESULT};
  if(!com.GetResult(&response, timeout))
    return false;
  if(response.data == nullptr)
    return false;
  res->id = response.data[0];
  uint8_t* time_as_char = (uint8_t*)&response.data[1 + 7];
  res->time = 0;
  while(time_as_char != (uint8_t*)response.data)
  {
    res->time <<= 8;
    res->time += *time_as_char--;
  }
  delete response.data;
  return true;
}

void Benchmarks::printResult(Bm_result& result, std::ostream& output)
{
  print_mutex->lock();
  output << "Result [" << (int)result.id << "] : " << (unsigned long)result.time << std::endl;
  print_mutex->unlock();
}

void Benchmarks::printResults(Benchmark& bm,  std::ostream& output)
{
  output << bm.name << ":" << std::endl;
  for(auto it = bm.results.begin(); it != bm.results.end(); it++)
    printResult(*it, output);
}

void Benchmarks::printResults(int nr, std::ostream& output)
{
  printResults(benchmarks[nr],output);
}
