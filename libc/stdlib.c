#include "stdlib.h"

void *malloc(size_t len) {
    uint32_t addr;
    syscall(SYS_MALLOC, len, NULL, NULL, NULL, &addr);
    return (void*)addr;
}

void *calloc(size_t num, size_t size) {
    uint8_t *result = (uint8_t*) malloc(num * size);
    for (uint32_t i = 0; i < num * size; i++)
        result[i] = 0x00;

    return (void*)result;
}

static uint32_t __rand_seed = 1;

uint32_t __entropy(void) {
    uint32_t result;
    syscall(SYS_ENTROPY, NULL, NULL, NULL, NULL, &result);
    return result;
}

void srand(uint32_t seed) {
    __rand_seed = seed;
}

uint32_t rand(void) {
    __rand_seed = (RAND_A * __rand_seed + RAND_C) % RAND_M;
    return (int)(__rand_seed & 0x7FFFFFFF);
}
