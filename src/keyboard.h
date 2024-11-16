#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "asm.h"
#include "std/stdlib.h"

/* circular buffer for scancodes. */
extern volatile uint16_t scancode_buffer[256];
extern volatile uint8_t scancode_bufempty;
extern volatile uint8_t scancode_read;
extern volatile uint8_t scancode_write;

/* scancode lookup tables for shifted and non-shifted keys. */
/* this assumes scancode set 1.*/
extern const uint8_t kbdus[256];
extern const uint8_t shift_kbdus[256];

/* initializes the keyboard/PS2 controller, for some systems.*/
void initialize_keyboard(void);

/* gets a line from the keyboard, blocking until a newline is received. */
/* you may choose whether you want input to echo or not. */
uint32_t kgetline(uint8_t echo, char *buffer, uint16_t capacity);

uint8_t process_scancode(uint8_t scancode);

/* the actual ISR for keyboard PS/2 interrupts. */
/* it is not wise to use __attribute__((__interrupt__)). */
/* instead, define an ISR in assembly (so you can use iret) and then call this. */
void interrupt_keyboard_handler();

#endif