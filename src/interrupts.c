#include "interrupts.h"

struct idt_entry idt[256];
struct idt_ptr idt_descriptor;

void PIC_remap(int offset1, int offset2)
{
	uint8_t a1, a2;
	
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
	
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}


void PIC_sendEOI(uint8_t irq) {
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	
	outb(PIC1_COMMAND, PIC_EOI);
}

void interrupt_timer_handler() {
    PIC_sendEOI(0);
}

void __interrupt_test(void) {
    for (uint8_t i = 0; i < 255; i++)
        for (uint8_t j = 0; j < 255; j++)
            ((uint8_t*)VIDEO_MEMORY)[i * 320 + j] = 0;
}

void interrupt_syscall_handler() {
    __interrupt_test();
}



void set_idt_entry(int num, uint32_t handler) {
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].selector = 0x08;    // Typically, 0x08 is the code segment offset in the GDT
    idt[num].zero = 0;
    idt[num].type_attr = 0x8E;   // Interrupt gate, present, privilege level 0
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
}

void load_idt() {
    idt_descriptor.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    __asm__ volatile("lidt (%0)" : : "r" (&idt_descriptor));
}

extern void asm_interrupt_timer_handler();
extern void asm_interrupt_keyboard_handler();
extern void asm_interrupt_syscall_handler();

void init_idt() {
    /* setup our Interrupt Descriptor Table entries, our ISRs */
    /* IRQ 0 - the PIT */
    set_idt_entry(0x20, (uint32_t)&asm_interrupt_timer_handler);

    /* IRQ 1 - the keyboard */
    set_idt_entry(0x21, (uint32_t)&asm_interrupt_keyboard_handler);

    /* set up the interrupt handler for our system call vector (0x33) */
    set_idt_entry(0x33, (uint32_t)&asm_interrupt_syscall_handler);
    
    /* load the Interrupt Descriptor Table into the respective register with lidt */
    load_idt();

    /* do not forget to remap the PIC! */
    /* in protected mode, CPU exceptions and default IRQ interrupts conflict, */
    /* so remapping the PIC is necessary (also, we haven't set up interrupt handlers for interrupts 0-15 yet) */
    /* and we set the IRQ1 bit on the master PIC to be unmasked, so without remapping the PIC, an IRQ1 */
    /* will trigger an interrupt without any handler--a crash! */

    PIC_remap(0x20, 0x28);

    /* set the bitmasks for the master and the slave PIC */
    /* for our keyboard, IRQ 1 needs to be enabled, so set the 2nd (IRQ 1) bit to 0. */
    /* IRQS:     76543210 */
    outb(0x21, 0b11111100);
    outb(0xA1, 0b11111111);


    __asm__ volatile("sti");
}