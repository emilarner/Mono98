#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stdint.h>
#include "__internal.h"
#include "unistd.h"

struct FILE {
    uint8_t fd;
};

typedef struct FILE FILE;

extern FILE _stderr;
extern FILE _stdout;
extern FILE _stdin;

extern FILE *stderr;
extern FILE *stdout;
extern FILE *stdin;

FILE *fopen(const char *path, const char *mode);
ssize_t fwrite(char *buf, size_t size, size_t len, FILE *fp);
ssize_t fread(char *buf, size_t size, size_t len, FILE *fp);

void fclose(FILE *fp);


#endif