#include "__internal.h"

/* kind of like our own crt0.o initialization object file */
uint8_t __entry(uint8_t argc, char *argv[]) {
    return main(argc, argv);
}