#ifndef SYSCALLS_H
#define SYSCALLS_H

enum SystemCalls {
    SYS_EXIT,
    SYS_READ,
    SYS_WRITE,
    SYS_MALLOC,
    SYS_OPEN, // unimplemented
    SYS_EXEC, // unimplemented
    SYS_ENTROPY
};

enum ErrnoCode {
    EOKAY,
    ESYSCALLNOENT,
    EINVALIDOPERATION,
    ENOENT
};

#endif