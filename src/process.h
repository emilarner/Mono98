#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "config.h"
#include "std/string.h"
#include "syscalls.h"
#include "terminal.h"
#include "keyboard.h"

#define errno *((uint32_t*)(0x333999 - 2 * sizeof(uint32_t)))
#define DEFAULT_MALLOC_LOCATION 0x2800000

/* the system call handler for this kernel -- it's just a C function that programs call! */
uint32_t syscall(uint8_t syscall_no, void *arg1, void *arg2, void *arg3, void *arg4);

/* load a program into memory at 0x333999, then call it as if it were a C function. */
uint8_t load_program(uint8_t *program_data, uint32_t program_len, uint8_t argc, char *argv[]);

#endif  