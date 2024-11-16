#ifndef MATH_H
#define MATH_H

#include <stdint.h>

typedef float float16_t;
typedef double float32_t;

#define M_PI        3.141592
#define M_E         2.718281
#define M_ROOT2     1.414213
#define M_ROOT3     1.732050

uint32_t isqrt(float32_t number);

#define __SQRT_ITERATIONS 3
float32_t sqrt(float32_t number);

#define __SIN_COS_ITERATIONS 10

float32_t pow(float32_t x, uint32_t n);
uint32_t factorial(uint32_t n);
float32_t sin(float32_t x);
float32_t cos(float32_t x);

#endif