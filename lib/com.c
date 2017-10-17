#include "com.h"
#include <stdint.h>
#include <info.h>
#include <../arch/include/gpio.h>
#include "../arch/include/std_io.h"
#include "../arch/include/interrupt.h"
#include "../lib/time.h"
#include "registration.h"

volatile uint8_t printf_in_progres = 0;
volatile uint8_t device_run = 0;
volatile uint8_t run_benchmark = 0;
volatile uint8_t res_id = 0;

void responde_to(Command_data* command_data);

void get_char_handler(char c)
{
  start_timer_off_section();
  static COM_STATE state = COMMAND;
  static uint8_t data_index = 0;
  static Command_data data;
  static char buffer[COM_BUFFER_SIZE];
  
  switch(state)
  {
    case COMMAND:
      if(c > NUM_COMMANDS || c == RESET_COM_C)
        break;
      data.command = c;
      state = LENGTH;
      break;
    
    case LENGTH:
      data.length = (unsigned char)c;
      data_index = 0;
      data.data = buffer;
      if(data.length)
        state = READ;
      else
      {
        state = COMMAND;
        responde_to(&data);
      }
      break;
    
    case READ:
      if(data_index < COM_BUFFER_SIZE)
        buffer[data_index++] = c;
      if(data_index < data.length)
        break;  //continue reading
      state = COMMAND;
      responde_to(&data);
      break;
  }
  end_timer_off_section();
}

void put_string(char* s)
{
  while(*s)
    plat_putc(*(s++));
}

void put_char_as_string(char c)
{
  char s[4] = {0};
  char* s_p = s;
  do
  {
    *(s_p++) = (c % 10) + '0';
    c /= 10;
  } while(c);
  while(s_p != s)
    plat_putc(*(--s_p));
}


void put_com_data(Response_data* data)
{ 
  if(printf_in_progres)
    plat_putc('\0');  //interrupt printf
  
  plat_putc(data->command);
  plat_putc(data->length);

  if(data->length == READ_TO_TERMINATION_MARK)
  {
    put_string(data->data);
    plat_putc('\0');
  }
  else if(data->length)
  {
    char *data_ptr = data->data;
    char *data_end = data_ptr + data->length;
    while(data_ptr < data_end)
      plat_putc(*(data_ptr++));
  }
  
  if(printf_in_progres)
  {
    plat_putc(PRINT_OUT); //contionue printf
    plat_putc(READ_TO_TERMINATION_MARK);
  }
}

void send_benchmark_list()
{
  Response_data response = {
    .command = BMS_RESPONSE,
    .length = READ_TO_TERMINATION_MARK
    };
  if(printf_in_progres)
    plat_putc('\0');  //interrupt printf

  plat_putc(response.command);
  plat_putc(response.length);
  int i;
  for(i = 0; i < bmbs_benchmark_count; i++)
  {
    put_string(bmbs_benchmarks[i].name);
    plat_putc(' ');
    put_char_as_string(bmbs_benchmarks[i].version_major);
    plat_putc('.');
    put_char_as_string(bmbs_benchmarks[i].version_minor);
    plat_putc(';');
  }
  plat_putc('\0');

  if(printf_in_progres)
  {
    plat_putc(PRINT_OUT); //contionue printf
    plat_putc(READ_TO_TERMINATION_MARK);
  }
}

void responde_to(Command_data* command_data)
{
  Response_data response = {.command = RESET_COM_R, .length = 0};
  
  switch(command_data->command)
  {
    case RESET_DEV:
      reset_device();
      break;
    
    case START_BM:
      if(*(command_data->data) < bmbs_benchmark_count)
      {
        run_benchmark = *(command_data->data) + 1; //+1, because 0 is stop
        res_id = 0;
        response.command = START_CONFIRM;
        response.length = READ_TO_TERMINATION_MARK;
        response.data = bmbs_benchmarks[run_benchmark - 1].name;
      }
      break;
    
    case GET_STATE:
      response.command = STATE_RESP;
      break;
    
    case GET_INFO:
      response.command = INFO_RESP;
      break;
    
    case GET_ARCH:
      response.command = ARCH_RESPONSE;
      response.data = ARCH;
      response.length = strlen(response.data);
      break;
    
    case ECHO_:
      response.command = ECHO_RESPONSE;
      response.length = command_data->length;
      response.data = command_data->data;
      break;
      
    case GET_BMS:
      send_benchmark_list();
      return;
  }
  put_com_data(&response);
}

void reset_confirm()
{
  Response_data response = {
    .command = RESET_CONFIRM,
    .length = 0
    };
  put_com_data(&response);
}

void put_result(uint8_t res_id, time_t time)
{
  Response_data response = {
    .command = RESULT,
    .length = sizeof(uint8_t) + sizeof(time_t)
  };
  unsigned char data_buffer[sizeof(uint8_t) + sizeof(time_t)];
  unsigned char* time_ptr = data_buffer + sizeof(uint8_t);
  response.data = (char*)data_buffer;
  data_buffer[0] = res_id;
  while(time_ptr <= data_buffer + sizeof(time_t))
  {
    *time_ptr = time & 0xff;
    time_ptr++;
    time >>= 8;
  }
  put_com_data(&response);
}
