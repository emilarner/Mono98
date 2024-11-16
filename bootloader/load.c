#include <stdint.h>

extern uint8_t disk;

void bios_disk_read(uint8_t drive, uint8_t head, uint16_t cylinder, 
                    uint8_t sector, uint8_t num_sectors, uint16_t segment, 
                    uint16_t offset) {
    __asm__ __volatile__ (
        "int $0x13"                // Call BIOS disk interrupt
        :                          // No output operands
        : "a"(0x0200 | num_sectors), // AH=0x02 (read), AL=num_sectors
          "b"(offset),              // BX=offset within the segment
          "c"((cylinder << 8) | sector), // CH=cylinder, CL=sector
          "d"((head << 8) | drive), // DH=head, DL=drive
          "S"(segment)              // ES=segment
        : "cc", "memory"            // Clobber flags and memory
    );
}

uint8_t bios_disk_read_error(void) {
    
}

void print_msg(uint8_t row, const char *msg) {
#ifndef REAL_HARDWARE
    uint8_t wait_iterations = 4;
    uint16_t iterations = 65535;
#endif
#ifdef REAL_HARDWARE
    uint8_t wait_iterations = 1;
    uint16_t iterations = 32768;
#endif
    for (uint8_t i = 0; msg[i]; i++) {
        ((uint16_t*)0xB8000)[row * 80 + i] = ((0x0F << 8) | msg[i]);
        for (uint8_t k = 0; k < wait_iterations; k++) {
            for (uint16_t j = 0; j < iterations; j++) {
                __asm__ volatile (
                    "out %%al, %%dx"
                    :                
                    :       
                    "a"(0x80),    
                    "d"(0x00)
                );
            }
        }
            
    }
}

void protected_mode(void) {
    print_msg(2, "Entering protected mode...");
}

void jumping(void) {
    print_msg(3, "Okay... jumping to the 32 bit kernel.");
}

void load(void) {
    print_msg(0, "Loading the kernel into memory...");
    print_msg(1, "This is pretty hard to get right on real hardware!");
}

void load_chs(void) {

}