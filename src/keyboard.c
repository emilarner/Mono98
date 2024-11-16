#include "keyboard.h"
#include "terminal.h"

volatile uint16_t scancode_buffer[256];
volatile uint8_t scancode_bufempty = 1;
volatile uint8_t scancode_read = 0;
volatile uint8_t scancode_write = 0; 

/* http://www.osdever.net/bkerndev/Docs/keyboard.htm */
const uint8_t kbdus[256] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

const uint8_t shift_kbdus[256] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',	/* 39 */
 '"', '~',   0,		/* Left shift */
 '|', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

#define KEYBOARD_WAITING 250
void initialize_keyboard(void) {
    outb(0x64, 0xAD);
    __asleep(KEYBOARD_WAITING);
    outb(0x64, 0xA7);
    __asleep(KEYBOARD_WAITING);
    inb(0x60);
    __asleep(KEYBOARD_WAITING);

    outb(0x60, 0xFF);
    __asleep(KEYBOARD_WAITING);

    outb(0x60, 0xF4);
    __asleep(KEYBOARD_WAITING);

    outb(0x60, 0xF0);
    __asleep(KEYBOARD_WAITING);
    outb(0x60, 0x02);

    __asleep(KEYBOARD_WAITING);
    outb(0x60, 0xED);
    __asleep(KEYBOARD_WAITING);
    outb(0x60, 0b00000111);
}

#ifdef REAL_HARDWARE
#define CURSOR_BLINKING_TIME 20000
#endif

#ifndef REAL_HARDWARE
#define CURSOR_BLINKING_TIME 500000
#endif

#define cursor_sleep(x) do {                                            \
                            for (uint32_t i = 0; i < x; i++) {          \
                                io_wait();                              \
                                io_wait();                              \
                                io_wait();                              \
                                                                        \
                                if (scancode_read != scancode_write)    \
                                    goto read_scancodes;                \
                            }                                           \
                        } while (0)                                     \

uint32_t kgetline(uint8_t echo, char *buffer, uint16_t capacity) {
    uint16_t len = 0;
    memset(scancode_buffer, 0x00, sizeof(scancode_buffer));
    scancode_read = 0;
    scancode_write = 0;

    while (1) {
#ifndef KEYBOARD_POLLING
        while (scancode_read == scancode_write) {
#endif
#ifdef KEYBOARD_POLLING
        while (1) {
        while (!(inb(0x64) & 1)) {
#endif
            kernel_terminal.show_cursor = 1;
            refresh_terminal(&kernel_terminal, 0);
            cursor_sleep(CURSOR_BLINKING_TIME);

            kernel_terminal.show_cursor = 0;
            refresh_terminal(&kernel_terminal, 0);
            cursor_sleep(CURSOR_BLINKING_TIME);
        }

#ifdef KEYBOARD_POLLING
            if (process_scancode(inb(0x60)))
                break;
        }
#endif

read_scancodes:
        for (uint8_t i = scancode_read; i != scancode_write; i = (i + 1) % sizeof(scancode_buffer)) {
            scancode_read = (scancode_read + 1) % sizeof(scancode_buffer);

            if (kbdus[(uint8_t)scancode_buffer[i]] == '\n')
                goto exit;

            if (kbdus[(uint8_t)scancode_buffer[i]] == '\b') {
                if (len) {
                    len--;
                    if (echo)
                        tputs(&kernel_terminal, (uint8_t[]){'\b', '\0'});
                }

                continue;
            }

            if (kbdus[(uint8_t)scancode_buffer[i]] != 0 && (len - 1) != capacity) {
                buffer[len] = (scancode_buffer[i] & (1 << 15)) ? 
                            shift_kbdus[(uint8_t)scancode_buffer[i]] :
                            kbdus[(uint8_t)scancode_buffer[i]];

                if (echo)
                    tputs(&kernel_terminal, (uint8_t[]){buffer[len], '\0'});

                len++;
            }
        }
    }

exit:
    kernel_terminal.show_cursor = 0;

    buffer[len] = '\0';
    tputs(&kernel_terminal, (uint8_t[]){'\n', '\0'});
    return len;
}

volatile uint8_t key_released = 0;
volatile uint8_t shift = 0;
volatile uint8_t ctrl = 0;

uint8_t process_scancode(uint8_t scancode) {
    switch (scancode) {
        /* shifts pressed */
        case 0x36:
        case 0x2A: {
            shift = 1;
            break;
        }

        /* shifts released */
        case 0xAA:
        case 0xB6: {
            shift = 0;
            break;
        }
        
        /* left ctrl pressed (who uses right ctrl?) */
        case 0x1D: {
            ctrl = 1;
            break;
        }

        /* left ctrl released */
        case 0x9D: {
            ctrl = 0;
            break;
        }

        default: {
            scancode_bufempty = 0;
            scancode_buffer[scancode_write] = (uint16_t)scancode 
                                            | ((uint16_t)shift << 15)
                                            | ((uint16_t)ctrl << 14);

            scancode_write = (scancode_write + 1) % sizeof(scancode_buffer);
            return 1;
        }
    }

    return 0;
}

void interrupt_keyboard_handler() {
    uint8_t key_scancode = inb(0x60);
    process_scancode(key_scancode);
    PIC_sendEOI(1);
}