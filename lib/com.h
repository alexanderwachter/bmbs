#ifndef _COM_H_
#define _COM_H_
#include <stdint.h>
#include "comProtocol.h"
#include "registration.h"
#include "time.h"

#define COM_BUFFER_SIZE 10

typedef enum {COMMAND, LENGTH, READ, READ_TO_TERMINATION} COM_STATE;

extern volatile uint8_t printf_in_progres;
extern volatile uint8_t run_benchmark;

extern void get_char_handler(char c);
extern void reset_confirm();
extern void put_result(uint8_t res_id, time_t time);

#endif
