#ifndef _TLV_STATE_MACHINE_H_
#define _TLV_STATE_MACHINE_H_
#include "../../lib/comProtocol.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <vector>
#include <thread>
#include <stdint.h>
#include <chrono>

struct TLVresponse
{
  std::mutex mtx;
  std::condition_variable ready;
  Response_data* data;

  TLVresponse(Response_data* data) : data(data) {}
};

class TLVstateMachine
{
  public:
  TLVstateMachine(const char* serial_device, int baudrate, std::mutex *print_mutex);
  virtual ~TLVstateMachine();
  bool SendCommand(Command_data& command, Response_data* response, std::chrono::seconds timeout);
  bool GetResult(Response_data* response, std::chrono::seconds timeout);
  void ResetCom();
  void cancleResponse(RESPONSE res);
  void clearBuffer() {response_buffer.clear();}
  private:
  TLVstateMachine( const TLVstateMachine& );
  const TLVstateMachine& operator=( const TLVstateMachine& );

  typedef enum {COMMAND, LENGTH, READ, READ_TO_TERMINATION, PRINT_} COM_STATE;
  COM_STATE state;
  Response_data response_data;
  std::vector<char> data_buffer;
  int fd;
  std::map<RESPONSE, TLVresponse*> response_map;
  std::thread read_thread;
  std::mutex *print_mutex;
  std::mutex buffer_mtx;
  std::vector<Response_data> response_buffer;
  void SendCommand(Command_data& command);
  void ConfigSerial(int baudrate);
  void EmptyBuffer();
  void ReadThread();
  void GetCharHandler(char c);
  void CallResponseHandler();
  ssize_t FullWrite(const void *buf, size_t len);
  ssize_t SafeWrite(const void *buf, size_t count);
  
};
#endif
