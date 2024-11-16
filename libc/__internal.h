#ifndef __INTERNAL_H
#define __INTERNAL_H

#include <stdint.h>

#define NULL (void*)0x00000000
#define __errno_location 0x333991
#define errno *((uint32_t*)__errno_location)
#define __syscall_location 0x333995

/* gnarly C code! */
#define syscall(a, b, c, d, e, f)  do {                                                                                         \
                                *f = ((uint32_t (*)(uint8_t, void*, void*, void*, void*))*(uint32_t*)__syscall_location)(       \
                                (uint8_t)a, (void*)b, (void*)c, (void*)d, (void*)e);                                            \
                                } while (0)                                                                                     \

extern uint8_t main(uint8_t argc, char *argv[]);
uint8_t __entry(uint8_t argc, char *argv[]);

#endif