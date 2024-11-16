#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "../asm.h"

#define NULL (void*)0x00000000

void *kmalloc(uint32_t amnt);

/* (approximately) sleep for some milliseconds */
void __asleep(uint32_t milliseconds);

#endif