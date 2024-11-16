#include "process.h"

void *MALLOC_START = DEFAULT_MALLOC_LOCATION;

uint32_t syscall(uint8_t syscall_no, void *arg1, void *arg2, void *arg3, void *arg4) {
    switch (syscall_no) {
        case SYS_WRITE: {
            uint8_t fd = (uint8_t)arg1;
            uint8_t *buf = (uint8_t*)arg2;
            uint32_t len = (uint32_t)arg3;

            /* when the file descriptor is 1, a null terminated string is expected. */
            /* therefore, arg4 should be NULL. */
            if (fd == 1)
                tputs(&kernel_terminal, buf);
            else {
                errno = EINVALIDOPERATION;
                return -1; 
            }


            break;
        }

        case SYS_READ: {
            uint8_t fd = (uint8_t)arg1;
            uint8_t *buf = (uint8_t*)arg2;
            uint32_t len = (uint32_t)arg3;

            if (fd == 0) {
                uint32_t safe_len = len;
                
                /* they are reading past the buffer. */
                if (kernel_terminal.text_offset + len >= kernel_terminal.text_buflen) {
                    safe_len = kernel_terminal.text_buflen - kernel_terminal.text_offset;
                }

                /* the keyboard has already been polled, no need to block again. */
                //if (kernel_terminal.text_buflen == 0)
                //    kernel_terminal.text_buflen = kgetline(1, kernel_terminal.text_buffer); 
                    
                memcpy(buf, kernel_terminal.text_buffer, len);

                kernel_terminal.text_offset += safe_len;

                if (kernel_terminal.text_offset == kernel_terminal.text_buflen) {
                    kernel_terminal.text_buflen = 0;
                    kernel_terminal.text_offset = 0;
                    memset(kernel_terminal.text_buffer, 0x00, sizeof(kernel_terminal.text_buffer));
                }

                break;
            }

            errno = EINVALIDOPERATION;
            return -1;
        }

        case SYS_MALLOC: {
            uint32_t len = (uint32_t)arg1;
            uint32_t result = MALLOC_START;

            errno = EOKAY;

            MALLOC_START += len;
            return result;
        }

        /* generate seeds for pseudorandom number generators */
        case SYS_ENTROPY: {
            uint32_t result = 0;

            /* get keyboard buffer data, for natural entropy. */
            for (uint16_t i = 0; i < sizeof(scancode_buffer); i++)
                result += scancode_buffer[i];

            /* get screen data, for visual entropy. */
            for (uint32_t i = 0xA0000; i <= 0xAFFFF; i++)
                result += *((uint8_t*)i);

            /* get unused memory data, with the hopes it contains random values. */
            for (uint32_t i = 0x00A00000; i <= 0x00AFFFFF; i++)
                result += *((uint8_t*)i);

            return result;
        }

        default: {
            errno = ESYSCALLNOENT;
            return -1;
        }
    }

    errno = EOKAY;
    return 0;
}

uint8_t load_program(uint8_t *program_data, uint32_t program_len, uint8_t argc, char *argv[]) {
    uint8_t magic[] = {
        0x50,
        0x5C,
        0xDE
    };

    if (!!memcmp(program_data, magic, 3))
        return 1;

  
    /* set the program's errno to zero. */
    *(uint32_t*)(PROGRAM_INJECTION_ADDRESS - 2 * sizeof(uint32_t)) = 0;
    

    /* copy the program into memory, starting at 0x3333333 */
    /* the program expects to start from here, as per the linker or assembly directives. */
    /* note... you probably shouldn't be making programs in assembly. */
    /* this kernel calls the entry point of the applications as if they were C functions */
    memcpy((uint8_t*)PROGRAM_INJECTION_ADDRESS, program_data, program_len);
    /* */
    ((uint8_t (*)(int, char**))PROGRAM_INJECTION_ADDRESS + 3)(argc, argv);
    MALLOC_START = DEFAULT_MALLOC_LOCATION;

    return 0;
}