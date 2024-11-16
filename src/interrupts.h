#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define PIC1		            0x20		/* IO base address for master PIC */
#define PIC2		            0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	        PIC1
#define PIC1_DATA	            (PIC1+1)
#define PIC2_COMMAND	        PIC2
#define PIC2_DATA	            (PIC2+1)
#define PIC_EOI		            0x20


#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#include <stdint.h>
#include "asm.h"
#include "terminal.h"
#include "keyboard.h"

extern volatile uint8_t key_released;

struct __attribute__((__packed__)) idt_entry {
    uint16_t offset_low;    // Lower 16 bits of handler address
    uint16_t selector;      // Code segment selector in GDT, will be 0x08, right after the null entry
    uint8_t zero;           // Set to zero
    uint8_t type_attr;      // Type and attributes
    uint16_t offset_high;   // Upper 16 bits of handler address
};

struct __attribute__((__packed__)) idt_ptr {
    uint16_t limit;   // Limit of the IDT (size - 1)
    uint32_t base;    // Base address of the IDT
};


void PIC_remap(int offset1, int offset2);
void PIC_sendEOI(uint8_t irq);

void interrupt_timer_handler();
void interrupt_keyboard_handler();
void interrupt_syscall_handler();

void set_idt_entry(int num, uint32_t handler);
void load_idt();
void init_idt();

#endif