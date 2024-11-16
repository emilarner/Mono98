#ifndef ASM_H
#define ASM_H

#include <stdint.h>

/* reads a byte from the specified I/O port */
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(value)
                      : "Nd"(port));
    return value;
}

/* writes a byte to the specified I/O port */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1"
                      :                
                      : "a"(value),    
                        "Nd"(port));
}

/* generates a small wait (1-4 microseconds, average being 2.5), useful for imprecise waiting */
/* and reprogramming of the PIC. */
static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif