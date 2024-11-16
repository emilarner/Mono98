#include "audio.h"


void play_sound(uint32_t frequency) {
    uint32_t div;
    uint8_t tmp;

        //Set the PIT to the desired frequency
    div = 1193180 / frequency;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (div) );
    outb(0x42, (uint8_t) (div >> 8));

        //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}
 
//make it shut up
void nosound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

//Make a beep
void beep(uint32_t frequency, uint32_t milliseconds) {
    play_sound(frequency);
    __asleep(milliseconds);
    nosound();
        //set_PIT_2(old_frequency);
}

void play_fdfile(uint8_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i += 2)
        beep(((uint16_t*)&data)[i], ((uint16_t*)&data)[i + 1]);
}