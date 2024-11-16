#include "string.h"

int16_t memcmp(const uint8_t *data1, const uint8_t *data2, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        if (data1[i] != data2[i])
            return data1[i] - data2[i];
    }

    return 0; 
}

int16_t strcmp(const char *str1, const char *str2) {
    uint32_t i;

    for (i = 0; str1[i]; i++) {
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }

    if (str2[i] != '\0')
        return str1[i] - str2[i];

    return 0;
}

uint32_t strlen(const char *ptr) {
    uint32_t result = 0;
    while (ptr[++result]);
    return result;
}

/* set len bytes of ptr to c, same as the <string.h> version */
void memset(void *ptr, uint8_t c, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        ((uint8_t*)ptr)[i] = c;
}

void memcpy(uint8_t *ptr1, uint8_t *ptr2, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        ptr1[i] = ptr2[i];
}

char *itoa(int number, char *output) {
    char *result = output;

    uint8_t length = 0;
    while (number) {
        int frac = number % 10;
        result[length] = '0' + frac;
        length++;
        number /= 10;
    }

    uint8_t left = 0;
    uint8_t right = length - 1;

    while (left < right) {
        char tmp = output[right];
        output[right] = output[left];
        output[left] = tmp;
        
        left++;
        right--;
    }

    result[length] = '\0';
    return result;
}

uint16_t split_string(const char *str, char delimiter, char escape, char **out) {
    uint16_t i = 0;
    uint8_t escaped = 0;

    for (int32_t j, k = 0; str[j]; j++, k++) {
        if (str[j] == escape) {
            escaped = !escaped;
            k--;
        }

        if (str[j] == delimiter && !escaped) {
            out[i][k] = '\0';
            i++;
            k = -1;
            continue;
        }

        if (str[j] != escape)
            out[i][k] = str[j];
    }

    if (str[strlen(str) - 1] == delimiter)
        return i;
    else
        return i + 1;
}