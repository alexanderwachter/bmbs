#include "TLVstateMachine.h"
#include <stdint.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#ifdef DEBUG
  #undef DEBUG
  #define DEBUG(...) std::cout << "DEBUG " << __LINE__ << ": " << __VA_ARGS__ << " in file "  << __FILE__ << std::endl;
#else
#define DEBUG(...)
#endif

#define READ_BUF_LEN 1

TLVstateMachine::TLVstateMachine(const char* serial_device, int baudrate,   std::mutex *print_mutex)
: read_thread(), print_mutex(print_mutex), buffer_mtx(), response_buffer()
{
  if((this->fd = open(serial_device, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    throw "can't open serial device";

  /*Reset O_NDELAY for blocking read*/
  int flags = fcntl(this->fd, F_GETFL, 0);
  if (flags == -1)
    throw "can't read fd flag";
  flags &= ~O_NDELAY;
  if(fcntl(this->fd, F_SETFL, flags))
    throw "can't set fd flag";

  ConfigSerial(baudrate);
  ResetCom();
  this->state = COMMAND;
  read_thread = std::thread(&TLVstateMachine::ReadThread,this);
}

TLVstateMachine::~TLVstateMachine()
{
  for(auto it = response_map.begin(); it != response_map.end(); it++)
  {
    delete it->second;
  }
  close(fd);
}

void TLVstateMachine::ReadThread()
{
  char buf[READ_BUF_LEN];
  char* buf_ptr;
  ssize_t n_chars;
  while(1)
  {

    do
    {
      n_chars = read(this->fd, buf, READ_BUF_LEN);
    }
    while (n_chars < 0 && errno == EINTR);
    buf_ptr = buf;
    while(n_chars--)
      GetCharHandler(*(buf_ptr++));
  }
}

void TLVstateMachine::GetCharHandler(char c)
{
  switch(state)
  {
    case COMMAND:
      if(c > NUM_RESPONSE || c == RESET_COM_R)
        break;
      //std::cout << (int)c << std::endl;
      this->response_data.command = (RESPONSE)c;
      state = LENGTH;
      break;

    case LENGTH:
      this->response_data.length = (unsigned char)c;
      data_buffer.clear();
      if(this->response_data.command == PRINT_OUT)
      {
        print_mutex->lock();
        std::cout << ">>>";
        state = PRINT_;
      }
      else if(this->response_data.length == READ_TO_TERMINATION_MARK)
        state = READ_TO_TERMINATION;
      else if(this->response_data.length)
        state = READ;
      else
      {
        state = COMMAND;
        CallResponseHandler();
      }
      break;

    case READ:
      data_buffer.push_back(c);
      if(data_buffer.size() < this->response_data.length)
        break;  //continue reading
      state = COMMAND;
      CallResponseHandler();
      break;

    case READ_TO_TERMINATION:
      data_buffer.push_back(c);
      if(c == '\0')
      {
        state = COMMAND;
        CallResponseHandler();
      }
      break;

      case PRINT_:
        if(c != '\0')
          std::cout << c;
        else
        {
          print_mutex->unlock();
          state = COMMAND;
        }
      break;
  }
}

void TLVstateMachine::CallResponseHandler()
{
  buffer_mtx.lock();
  auto it = response_map.find(this->response_data.command);
  //no entry yet. Buffer
  if(it == response_map.end())
  {
    if(data_buffer.size())
    {
      response_data.data = new char[data_buffer.size()]();
      memcpy(response_data.data, &(this->data_buffer[0]), data_buffer.size());
    }
    this->response_buffer.push_back(this->response_data);
  }
  else
  {
    it->second->data->length = this->response_data.length;

    if(data_buffer.size())
    {
      it->second->data->data = new char[data_buffer.size()]();
      memcpy(it->second->data->data, &(this->data_buffer[0]), data_buffer.size());
    }
    it->second->ready.notify_all();
  }
  buffer_mtx.unlock();
}

void TLVstateMachine::cancleResponse(RESPONSE res)
{
  auto it = response_map.find(res);
  if(it == response_map.end())
    return;
  it->second->data = nullptr;
  it->second->ready.notify_all();
}

bool TLVstateMachine::SendCommand(Command_data& command, Response_data* response, std::chrono::seconds timeout)
{
  if(command.command == RESET_COM_C)
    throw std::invalid_argument("response command is not valid");
  if(this->response_map.find(response->command) != this->response_map.end())
    throw std::overflow_error("Command already in list");
  DEBUG("Send command " << command.command)
  TLVresponse *tlv_resp = new TLVresponse(response);
  response_map[response->command] = tlv_resp;
  std::unique_lock<std::mutex> u_lock(tlv_resp->mtx);
  SendCommand(command);
  bool status = tlv_resp->ready.wait_for(u_lock, timeout) != std::cv_status::timeout;
  response_map.erase(response->command);
  delete tlv_resp;
  return status;
}

bool TLVstateMachine::GetResult(Response_data* response, std::chrono::seconds timeout)
{
  if(this->response_map.find(response->command) != this->response_map.end())
    throw std::overflow_error("Command already in list");

  /*Maybe there is a buffered result*/
  buffer_mtx.lock();
  for(auto it = response_buffer.begin(); it != response_buffer.end(); it++)
  {
    if(it->command == response->command)
    {
      response->length = it->length;
      response->data   = it->data;
      response_buffer.erase(it);
      buffer_mtx.unlock();
      return true;
    }
  }

  TLVresponse *tlv_resp = new TLVresponse(response);
  response_map[response->command] = tlv_resp;
  std::unique_lock<std::mutex> u_lock(tlv_resp->mtx);
  buffer_mtx.unlock();
  bool status = tlv_resp->ready.wait_for(u_lock, timeout) != std::cv_status::timeout;
  response_map.erase(response->command);
  delete tlv_resp;
  return status;
}

//GNU function
ssize_t TLVstateMachine::SafeWrite(const void *buf, size_t count)
{
  ssize_t n;

  do {
    n = write(this->fd, buf, count);
  } while (n < 0 && errno == EINTR);

  return n;
}


//GNU Function
ssize_t TLVstateMachine::FullWrite(const void *buf, size_t len)
{
  ssize_t cc;
  ssize_t total;

  total = 0;

  while (len) {
    cc = SafeWrite(buf, len);

    if (cc < 0) {
    if (total) {
        /* we already wrote some! */
        /* user can do another write to know the error code */
        return total;
      }
      return cc;  /* write() returns -1 on failure. */
    }
    total += cc;
    buf = ((const char *)buf) + cc;
    len -= cc;
  }

  return total;
}


void TLVstateMachine::SendCommand(Command_data& command)
{
  SafeWrite(&(command.command), 1);
  SafeWrite(&(command.length), 1);
  FullWrite(command.data, command.length);
  fsync(this->fd);
}

void TLVstateMachine::ConfigSerial(int baudrate)
{
  struct termios tios;
  memset(&tios, 0, sizeof(struct termios));

  /* CS8: 8 databits, CLOCAL: ignore statuslines, enable read */

  tios.c_cflag = CS8 | CLOCAL | CREAD;

  /*Raw input, Raw output (done my memset)*/
  //tios.c_iflag = 0;
  //tios.c_oflag = 0;

  //tios.c_lflag = ICANON;

  /* blocking read until 1 char arrives */
  tios.c_cc[VMIN] = 1;

  cfsetspeed(&tios, baudrate);

  tcflush(this->fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&tios);
}

void TLVstateMachine::EmptyBuffer()
{
  /*Set non blocking*/
  int flags_save = fcntl(this->fd, F_GETFL, 0);
  if (flags_save == -1)
    throw "can't read fd flag";
  if(fcntl(this->fd, F_SETFL, flags_save | O_NONBLOCK))
    throw "can't set fd flag";

  /*Read till no data is left*/
  int n;
  char buf[10];
  do {
        n = read (this->fd, buf, 10);
  } while (n > 0);

  /*recover flags*/
  fcntl(this->fd, F_SETFL, flags_save);
}

void TLVstateMachine::ResetCom()
{
  EmptyBuffer();
  char reset_code[10] = {RESET_COM_C};
  write(this->fd, reset_code, 10);
}
