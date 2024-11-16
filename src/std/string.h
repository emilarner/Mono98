#ifndef STD_STRING_H
#define STD_STRING_H

#include <stdint.h>

#define NULL (void*)0x00000000

int16_t strcmp(const char *str1, const char *str2);
int16_t memcmp(const uint8_t *data1, const uint8_t *data2, uint32_t len);

uint32_t strlen(const char *str);
void memset(void *ptr, uint8_t c, uint32_t len);
void memcpy(uint8_t *ptr1, uint8_t *ptr2, uint32_t len);

char *itoa(int number, char *output);
uint16_t split_string(const char *str, char delimiter, char escape, char **out);

#endif