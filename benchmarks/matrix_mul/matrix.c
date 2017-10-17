#include "matrix.h"
#include <string.h>
#include <registration.h>
#include <xprintf.h>
#include <time.h>
#include <timer.h>
#include <com.h>

#define NUM_ITERATIONS 1000
#define N_COLUMNS  5
#define M_ROWS     10

void matrix_mul();

#define MULTIPLY_MATRIX(bits) \
int multiply_##bits(Matrix_##bits * a, Matrix_##bits * b, Matrix_##bits * res)     \
{                                                                                  \
  if(a->n != b->m)                                                                 \
    return -1;                                                                     \
  if(res->m < a->m || res->n < b->n)                                               \
    return -2;                                                                     \
  res->m = a->m;                                                                   \
  res->n = b->n;                                                                   \
                                                                                   \
  memset((void*)(res->data), 0, res->n * res->m);                                  \
                                                                                   \
  int i,j,k;                                                                       \
  for(i = 0; i < res->m; i++)                                                      \
  {                                                                                \
    for(j = 0; j < res->n; j++)                                                    \
    {                                                                              \
      for(k = 0; k < a->n ;k++)                                                    \
        res->data[i * res->n + j] += a->data[i * a->n + k] * b->data[k * b->n + j];\
    }                                                                              \
  }                                                                                \
  return 0;                                                                        \
}

#define FAST_MULTIPLY_MATRIX(bits) \
int fast_multiply_##bits(Matrix_##bits * a, Matrix_##bits * b, Matrix_##bits * res) \
{                                                                                  \
  if(a->n != b->m)                                                                 \
    return -1;                                                                     \
  if(res->m < a->m || res->n < b->n)                                               \
    return -2;                                                                     \
  res->m = a->m;                                                                   \
  res->n = b->n;                                                                   \
                                                                                   \
  memset((void*)(res->data), 0, res->n * res->m);                                  \
  int##bits##_t *res_data = res->data, *a_data, *a_data_save, *b_data;             \
  int i,j,k;                                                                       \
  for(i = 0, a_data_save = a->data ; i < res->m; i++, a_data_save += a->n)         \
  {                                                                                \
    for(j = 0; j < res->n; j++, res_data++)                                        \
    {                                                                              \
      b_data = b->data + j;                                                        \
      for(k = 0, a_data = a_data_save; k < a->n ;k++, b_data += b->n, a_data++)    \
        *res_data += (*a_data) * (*b_data);                                        \
    }                                                                              \
  }                                                                                \
  return 0;                                                                        \
}

MULTIPLY_MATRIX(8)
MULTIPLY_MATRIX(16)
MULTIPLY_MATRIX(32)
FAST_MULTIPLY_MATRIX(8)
FAST_MULTIPLY_MATRIX(16)
FAST_MULTIPLY_MATRIX(32)

void print_matrix(Matrix_8* m)
{
  int i,j;
  xprintf("[%dx%d]\n", m->m, m->n);
  for(i = 0; i < m->m; i++ )
  {
    for(j = 0; j < m->n; j++)
    {
      xprintf("%d ", m->data[i * m->n + j]);
    }
    xprintf("\n");
  }
}

#define SET_MATRIX(bits)                                   \
void set_matrix_##bits(Matrix_##bits *m, int max)          \
{                                                          \
  int i,j;                                                 \
  for(i = 0; i < m->m; i++)                                \
   for(j = 0; j < m->n; j++)                               \
    m->data[i * m->n + j] = (i * m->n + j + 1) % max;      \
}

SET_MATRIX(8)
SET_MATRIX(16)
SET_MATRIX(32)

void matrix_mul()
{
  time_t time_meas;
  int32_t matrix_a[M_ROWS * N_COLUMNS];
  int32_t res_matrix[M_ROWS * M_ROWS];
  int ret,i;

  Matrix_8 a_8 = {(int8_t*)matrix_a, M_ROWS, N_COLUMNS};
  Matrix_8 b_8 = {(int8_t*)matrix_a, N_COLUMNS, M_ROWS};
  Matrix_8 res_8 = {(int8_t*)res_matrix, M_ROWS, M_ROWS};

  Matrix_16 a_16 = {(int16_t*)matrix_a, M_ROWS, N_COLUMNS};
  Matrix_16 b_16 = {(int16_t*)matrix_a, N_COLUMNS, M_ROWS};
  Matrix_16 res_16 = {(int16_t*)res_matrix, M_ROWS, M_ROWS};

  Matrix_32 a_32 = {matrix_a, M_ROWS, N_COLUMNS};
  Matrix_32 b_32 = {matrix_a, N_COLUMNS, M_ROWS};
  Matrix_32 res_32 = {res_matrix,M_ROWS, M_ROWS};


  set_matrix_8(&a_8, 8);
  xprintf("8bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
     ret = multiply_8(&a_8,&b_8,&res_8);
  }
  put_result(10, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);
  //print_matrix(&res_8);

  res_8.m = M_ROWS; res_8.n = M_ROWS;
  xprintf("optimized 8bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
    ret = fast_multiply_8(&a_8,&b_8,&res_8);
  }
  put_result(11, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);
  //print_matrix(&res_8);

  set_matrix_16(&a_16, 30);
  xprintf("16bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
     ret = multiply_16(&a_16,&b_16,&res_16);
  }
  put_result(20, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);

  res_16.m = M_ROWS; res_16.n = M_ROWS;
  xprintf("optimized 16bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
    ret = fast_multiply_16(&a_16,&b_16,&res_16);
  }
  put_result(21, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);
  else

  set_matrix_32(&a_32, 100);
  xprintf("32bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
     ret = multiply_32(&a_32,&b_32,&res_32);
  }
  put_result(30, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);

  res_32.m = M_ROWS; res_32.n = M_ROWS;
  xprintf("optimized 32bit [%dx%d] * [%dx%d] multiplication\n", M_ROWS, N_COLUMNS, N_COLUMNS, M_ROWS);
  tick(&time_meas);
  for(i = 0; i < NUM_ITERATIONS; i++)
  {
    ret = fast_multiply_32(&a_32,&b_32,&res_32);
  }
  put_result(31, tock(time_meas)/NUM_ITERATIONS);
  if(ret)
    xprintf("Error %d", ret);

  xprintf("Finished\n");

}

BMBS_REGISTER_BENCHMARK(MatrixMult,2 , 0, matrix_mul)
