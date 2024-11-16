#include "unistd.h"


ssize_t write(uint8_t fd, char *buf, size_t len) {
    ssize_t res;
    syscall(SYS_WRITE, fd, buf, len, NULL, &res);
    return res;
}


ssize_t read(uint8_t fd, char *buf, size_t len) {
    ssize_t res;
    syscall(SYS_READ, fd, buf, len, NULL, &res);
    return res;
}