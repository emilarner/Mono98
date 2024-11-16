#include "stdio.h"

FILE _stderr = {
    .fd = 1
};

FILE _stdout = {
    .fd = 1
};

FILE _stdin = {
    .fd = 0
};

FILE *stderr = &_stderr;
FILE *stdout = &_stdout;
FILE *stdin = &_stdin;


FILE *fopen(const char *path, const char *mode) {
    return NULL;
}


ssize_t fwrite(char *buf, size_t size, size_t len, FILE *fp) {
    return write(fp->fd, buf, size * len);
}

ssize_t fread(char *buf, size_t size, size_t len, FILE *fp) {
    return read(fp->fd, buf, size * len);
}

void fclose(FILE *fp) {

}
