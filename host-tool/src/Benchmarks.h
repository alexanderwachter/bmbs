#ifndef _BENCHMARKS_H_
#define _BENCHMARKS_H_

#include <string.h>
#include <string>
#include <vector>
#include "TLVstateMachine.h"
#include <ostream>

struct Benchmark
{
  std::string name;
  int         version_major;
  int         version_minor;
  std::vector<Bm_result> results;
  Benchmark(char* name) : name(name), version_major(0), version_minor(0) {}
  Benchmark(char* name, int major, int minor) : name(name), version_major(major), version_minor(minor) {}
};

class Benchmarks
{
  public:
  Benchmarks(std::mutex *print_mutex) : print_mutex(print_mutex){};
  virtual ~Benchmarks(){}
  void getBenchmarks(TLVstateMachine& com);
  void printBenchmarks(std::ostream& output);
  void startBenchmark(unsigned char nr, TLVstateMachine& com);
  bool getResult(TLVstateMachine&com, Bm_result* res, std::chrono::seconds timeout);
  bool runBenchmark(unsigned char nr, TLVstateMachine& com, std::chrono::seconds timeout);
  void printResult(Bm_result& result, std::ostream& output);
  void printResults(int nr, std::ostream& output);
  void printResults(Benchmark& bm,  std::ostream& output);
  std::vector<Benchmark> benchmarks;

  private:
  Benchmarks(const Benchmarks&);
  const Benchmarks& operator=(const Benchmarks&);
  std::mutex *print_mutex;
};

#endif
