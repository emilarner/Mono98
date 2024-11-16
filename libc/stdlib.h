#ifndef STDLIB_H
#define STDLIB_H

#include "__internal.h"
#include "../src/syscalls.h"
#include <stdint.h>

typedef uint32_t size_t; 
typedef int32_t ssize_t; 

void *malloc(size_t len);
void *calloc(size_t num, size_t size);

#define RAND_A 1664525
#define RAND_C 1013904223
#define RAND_M 0xFFFFFFFF  // 2^32

uint32_t __entropy(void);
void srand(uint32_t seed);
uint32_t rand(void);

#endif