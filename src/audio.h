#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include "std/stdlib.h"
#include "asm.h"

void play_sound(uint32_t frequency);
void nosound();
void beep(uint32_t frequency, uint32_t milliseconds);
void play_fdfile(uint8_t *data, uint32_t length);


#endif