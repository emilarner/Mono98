#include <stdint.h>

#include "std/stdlib.h"
#include "colours.h"
#include "audio.h"
#include "config.h"
#include "interrupts.h"
#include "graphics.h"
#include "terminal.h"
#include "keyboard.h"
#include "romdisk.h"
#include "process.h"

extern uint8_t main_romdisk[];
extern void test_interrupts(void);

void kputs(const char *msg) {
    tputs(&kernel_terminal, msg);
}

void kputc(const char c) {
    kputs((uint8_t[]){c, '\0'});
}

#define __ROMDISK_SPACING sizeof("           ") - 1
void __list_romdisk(struct romdisk_entry *entry) {
    char filesize[16];
    itoa(entry->length, filesize);

    kputs(entry->filename);
    for (uint8_t i = 0; i < (__ROMDISK_SPACING) + (8 - strlen(entry->filename)); i++)
        kputc(' ');

    kputs(filesize);
    kputs("\n");
}

void text_sandbox(void) {
    while (1) {
        while (scancode_read == scancode_write);

        for (uint8_t i = scancode_read; i < scancode_write; i++, scancode_read = (scancode_read + 1) % sizeof(scancode_buffer)) {
            uint16_t scancode = scancode_buffer[i];
            kputc((scancode & (1 << 15)) ? shift_kbdus[(uint8_t)scancode] : kbdus[(uint8_t)scancode]);
        }
    }
}

char command[256];

void display_romdisk(void) {
    kputs("Contents of main romdisk\n");
    kputs("FILENAME           LENGTH\n");

    list_romdisk(main_romdisk, &__list_romdisk);
    kputc('\n');
}

#define MAX_PROGRAM_ARGS 32
#define MAX_PROGRAM_ARG_LENGTH 64

char **program_args = NULL;

void handle_command(char *command) {
    if (!strcmp(command, "PING"))
        kputs("\nPONG\n");

    else if (!strcmp(command, "SANDBOX"))
        text_sandbox();

    else if (!strcmp(command, "CLEAR"))
        clear_terminal(&kernel_terminal);

    else if (!strcmp(command, "LS-ROMDISK"))
        display_romdisk();

    else {
        for (uint16_t i = 0; i < MAX_PROGRAM_ARGS; i++)
            memset(program_args[i], 0x00, MAX_PROGRAM_ARG_LENGTH);

        uint16_t argc = split_string(command, ' ', '\"', program_args);
        struct romdisk_file program = get_romdisk_file(main_romdisk, program_args[0]);
        
        if (program.noent) {
            kputc(terminal_colour_code(TERM_RED));
            kputs("Command '");
            kputs(program_args[0]);
            kputs("' not found.\n");
            kputc(terminal_colour_code(TERM_BLACK));
            return;
        }

        if (load_program(program.file, program.length, argc, program_args)) {
            kputc(terminal_colour_code(TERM_RED));
            kputs("Program invalid--is this a valid binary?\n");
            kputc(terminal_colour_code(TERM_BLACK));
        }
    }
}

void shell(void) {
    program_args = (char**) kmalloc(sizeof(char*) * 32);
    for (uint8_t i = 0; i < 32; i++)
        program_args[i] = (char*) kmalloc(sizeof(char) * 64);

    refresh_terminal(&kernel_terminal, 1);

    for (uint8_t i = 0; KERNEL_MOTD[i]; i++) {
        kputc(KERNEL_MOTD[i]);
        __asleep(500);
    }

    display_romdisk();

#ifdef DEBUG
    kputc('\n');

    struct romdisk_file testasm = get_romdisk_file(main_romdisk, "TESTASM");
    load_program(testasm.file, testasm.length, 0, NULL);

    struct romdisk_file testc = get_romdisk_file(main_romdisk, "TESTC");
    load_program(testc.file, testc.length, 0, NULL);
#endif DEBUG

    kputc('\n');

    while (1) {
        memset(command, 0x00, sizeof(command));
        kputc('>');
        kgetline(1, command, sizeof(command));
        handle_command(command);
    }

    while (1) {
        __asm__ __volatile__ ("hlt");
    }

    return 0;
}


void init_fpu(void) {
    __asm__ volatile("finit");
}

/* entry point into the kernel */
int kmain(void) {
    init_fpu();
    memset(scancode_buffer, 0x00, sizeof(scancode_buffer));

    init_terminal(
        &kernel_terminal, 
        KERNEL_TERMINAL_ROWS, 
        KERNEL_TERMINAL_COLUMNS, 
#ifndef NO_BACKGROUND
        get_romdisk_file(main_romdisk, "BACKGROUND").file, 
#endif
#ifdef NO_BACKGROUND
        NULL,
#endif
        VGA_COLOR_BLACK
    );

    init_idt();

    /* set up the system call function pointer, so programs can utilize the jump table. */
    *(uint32_t*)((PROGRAM_INJECTION_ADDRESS - sizeof(uint32_t))) = &syscall;

    initialize_keyboard();

#ifdef DEBUG
    test_interrupts();
#endif

    /* start up jingle */
    for (uint8_t i = 0; i < 4; i++) {
        beep(2500 + 5 * ((i & 1) ? -1 : 1), 350);
        beep(1000 + 5 * ((i & 1) ? -1 : 1), 350);
        beep(2500 + 5 * ((i & 1) ? -1 : 1), 350);
    }

    shell();
}