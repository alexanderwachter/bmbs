#ifndef _COM_PROTOCOL_H_
#define _COM_PROTOCOL_H_

#include <stdint.h>
#define READ_TO_TERMINATION_MARK 0xFF
#define TOTAL_TIME_ID 0xFF

typedef enum commands {RESET_COM_C, RESET_DEV, START_BM, GET_STATE, GET_INFO, GET_ARCH, ECHO_, GET_BMS, NUM_COMMANDS} COMMANDS;
typedef enum response {RESET_COM_R, RESET_CONFIRM, START_CONFIRM, STATE_RESP, INFO_RESP, ARCH_RESPONSE, ECHO_RESPONSE, BMS_RESPONSE, RESULT, PRINT_OUT, NUM_RESPONSE} RESPONSE;

typedef struct {
  COMMANDS command;
  unsigned char length;
  char *data;
} Command_data;

typedef struct {
  RESPONSE command;
  unsigned char length;
  char* data;
} Response_data;

typedef struct {
  uint8_t id;
  uint64_t time;
} Bm_result;

#endif
