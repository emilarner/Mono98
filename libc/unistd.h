#ifndef UNISTD_H
#define UNISTD_H

#include <stdint.h>
#include "__internal.h"
#include "../src/syscalls.h"

typedef uint32_t size_t; 
typedef int32_t ssize_t; 

ssize_t write(uint8_t fd, char *buf, size_t len);
ssize_t read(uint8_t fd, char *buf, size_t len);

#endif