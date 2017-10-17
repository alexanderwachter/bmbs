#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <stdint.h>

/*Matrix with m rows and n columns*/
typedef struct
{
  int8_t* data;
  int m;
  int n;
} Matrix_8;

typedef struct
{
  int16_t* data;
  int m;
  int n;
} Matrix_16;

typedef struct
{
  int32_t* data;
  int m;
  int n;
} Matrix_32;

int multiply_8(Matrix_8* a, Matrix_8* b, Matrix_8* res);
void zeros(Matrix_8* m);

#endif /*_MATRIX_H_*/
