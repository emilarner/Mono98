#include "stdlib.h"

/* where does that memory start? */
void *KMALLOC_START = 0x1F0000;

/* give me memory! */
void *kmalloc(uint32_t amnt) {
    void *result = KMALLOC_START;
    KMALLOC_START += amnt;
    return result;
}

#define PIT_FREQUENCY 1193182  // Base frequency for the PIT
#define PIT_CHANNEL_0 0x40     // Channel 0 data port (for delays)
#define PIT_COMMAND   0x43     // Mode/Command register

void set_pit_frequency(uint16_t divisor) {
    outb(PIT_COMMAND, 0x36);                // Set command byte
    outb(PIT_CHANNEL_0, (uint8_t)(divisor & 0xFF));  // Set low byte
    io_wait();
    outb(PIT_CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF)); // Set high byte
    io_wait();
}

void __asleep(uint32_t milliseconds) {
#ifndef REAL_HARDWARE
    milliseconds *= 18;
#endif

    // Calculate the divisor based on the requested delay
    uint16_t divisor = (PIT_FREQUENCY / 1000) * milliseconds;
    set_pit_frequency(divisor);

    // Wait until the delay has elapsed
    for (volatile uint32_t i = 0; i < milliseconds * 100; i++) {
        io_wait();  // Add small waits to prevent overloading CPU
    }

    // Reset the PIT to its default rate (optional, depending on your needs)
    set_pit_frequency(0xFFFF);
}

/*
#define SUBTRACTOR -2000
void __asleep(uint32_t milliseconds) {
    int iterations = milliseconds * 400 + SUBTRACTOR;
    for (int i = 0; i < iterations; i++)
        io_wait();
}
*/