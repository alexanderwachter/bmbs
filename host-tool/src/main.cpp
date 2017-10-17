#include <iostream>
#include "TLVstateMachine.h"
#include "Benchmarks.h"
#include "Prompt.h"
#include "../../lib/comProtocol.h"
#include <sys/stat.h>
#include <chrono>
#include <unistd.h>
#include <signal.h>

#define BAUDRATE 115200

#ifdef DEBUG
  #undef DEBUG
  #define DEBUG(...) std::cout << "DEBUG " << __LINE__ << ": " << __VA_ARGS__ << " in file "  << __FILE__ << std::endl;
#else
#define DEBUG(...)
#endif

std::mutex print_mutex;

TLVstateMachine* communication_ptr;
Benchmarks* benchmarks_ptr;

int ResetDeviceCmd(const std::vector<std::string>& argv);
int LoadBenchmarks(const std::vector<std::string>& argv);
int ShowBenchmarks(const std::vector<std::string>& argv);
int RunBenchmarks(const std::vector<std::string>& argv);
int PrintResult(const std::vector<std::string>& argv);

int ResetDevice();

void print_usage(char* progname)
{
	printf("Usage: %s </dev/ttyxxx>\n", progname);
	exit(EXIT_SUCCESS);
}

void cancelBenchmarks(int sig)
{
  communication_ptr->cancleResponse(RESULT);
  ResetDevice();
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    print_usage(argv[0]);

  try
  {
    TLVstateMachine communication(argv[1], BAUDRATE, &print_mutex);
    communication_ptr = &communication;
    Benchmarks bms(&print_mutex);
    benchmarks_ptr = &bms;

    Prompt prompt;
    prompt.addComand("reset", ResetDeviceCmd, "Reset device\nreset");
    prompt.addComand("load", LoadBenchmarks, "Load available benchmarks\nload");
    prompt.addComand("show", ShowBenchmarks, "Print a list of availabe benchmarks\nshow");
    prompt.addComand("run", RunBenchmarks, "run benchmarks\nrun       <- run all\nrun 0 1 5 <- run specific");
    prompt.addComand("result", PrintResult, "Print the Result\nresult       <- print all\nresult 0 1 5 <- print specific");
    signal(SIGINT, cancelBenchmarks);
    prompt.start("BMBS> ");
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
  catch(const char* e)
  {
    std::cout << e << std::endl;
  }
  return EXIT_SUCCESS;
}

int ResetDeviceCmd(const std::vector<std::string>& argv)
{
  return ResetDevice();
}
int ResetDevice()
{
  Command_data command = {
    .command = RESET_DEV,
    .length = 0
  };
  Response_data response = {.command = RESET_CONFIRM};
  DEBUG("Reset device");
   try
    {
      communication_ptr->ResetCom();
      if(communication_ptr->SendCommand(command, &response, std::chrono::seconds(1)))
      {
        print_mutex.lock();
        std::cout << "Device reset SUCCESS" << std::endl;
        print_mutex.unlock();
        return EXIT_SUCCESS;
      }
      else
      {
        print_mutex.lock();
        std::cout << "TIMEOUT for reset" << std::endl;
        print_mutex.unlock();
      }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const char* e)
    {
      std::cout << e << std::endl;
    }
    return EXIT_FAILURE;
}

int LoadBenchmarks(const std::vector<std::string>& argv)
{
  try
  {
    DEBUG("Load benchmarks");
    benchmarks_ptr->getBenchmarks(*communication_ptr);
    DEBUG("Benchmarks loaded");
    benchmarks_ptr->printBenchmarks(std::cout);
    std::cout << std::endl;
    return EXIT_SUCCESS;
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch(const char* e)
  {
    std::cout << e << std::endl;
    return EXIT_FAILURE;
  }

}

int ShowBenchmarks(const std::vector<std::string>& argv)
{
  benchmarks_ptr->printBenchmarks(std::cout);
  std::cout << std::endl;
  return EXIT_SUCCESS;
}

int RunBenchmark(int nr)
{
  try
  {
    DEBUG("Run Benchmark");
    if(!benchmarks_ptr->runBenchmark(nr, *communication_ptr, std::chrono::seconds(10000)))
    {
      print_mutex.lock();
      std::cout << "Timeout on waiting for result" << std::endl;
      print_mutex.unlock();
      return EXIT_FAILURE;
    }
    std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
      std::cout << e.what() << std::endl;
      return EXIT_FAILURE;
    }
    catch(const char* e)
    {
      std::cout << e << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int RunBenchmarks(const std::vector<std::string>& argv)
{
  if(!benchmarks_ptr->benchmarks.size())
    LoadBenchmarks(argv);
  if(!benchmarks_ptr->benchmarks.size())
  {
    print_mutex.lock();
    std::cout << "No benchmarks available!" << std::endl;
    print_mutex.unlock();
    return EXIT_FAILURE;
  }
  if(argv.size() == 1)
  {
    for(unsigned int i = 0; i < benchmarks_ptr->benchmarks.size(); i++)
    {
      if(RunBenchmark(i) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  for(auto it = ++argv.begin(); it != argv.end(); it++)
  {
    try{
       int nr = std::stoi(*it);
       if(RunBenchmark(nr) != EXIT_SUCCESS)
         return EXIT_FAILURE;
    }
    catch (const std::invalid_argument& ia) {
      print_mutex.lock();
      std::cerr << "Invalid argument: " << ia.what() << std::endl;
      print_mutex.unlock();
    }
  }
  return EXIT_SUCCESS;
}

int PrintResult(const std::vector<std::string>& argv)
{
  if(!benchmarks_ptr->benchmarks.size())
  {
    print_mutex.lock();
    std::cout << "No benchmarks available!" << std::endl;
    print_mutex.unlock();
    return EXIT_FAILURE;
  }
  if(argv.size() == 1)
  {
    for(unsigned int i = 0; i < benchmarks_ptr->benchmarks.size(); i++)
    {
      benchmarks_ptr->printResults(i, std::cout);
    }
    return EXIT_SUCCESS;
  }

  for(auto it = ++argv.begin(); it != argv.end(); it++)
  {
    try{
       int nr = std::stoi(*it);
       benchmarks_ptr->printResults(nr, std::cout);
    }
    catch (const std::invalid_argument& ia) {
      print_mutex.lock();
      std::cerr << "Invalid argument: " << ia.what() << std::endl;
      print_mutex.unlock();
    }
  }
  return EXIT_SUCCESS;
}
