#include <stdint.h>

#include "../../libc/stdio.h"
#include "../../libc/stdlib.h"
#include "../../libc/math.h"


uint8_t main(uint8_t argc, char *argv[]) {
    /*
    uint8_t special = 0;
    uint8_t times = 0;

    for (uint8_t i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'T')
                times = argv[i + 1][0] - '0';
        }
    }

    srand(__entropy());
    char str[3];
    str[2] = '\0';
    str[1] = '\n';

    for (uint8_t i = 0; i < times; i++) {
        str[0] = '0' + (rand() % 10);
        fwrite(str, -1, -1, stdout);
    }

    fwrite((uint8_t[]){'0' + isqrt(9), '\n', '\0'}, -1, -1, stdout);

    for (uint16_t i = 0; i <= 65535; i++) {
        int32_t x = 40 * cos(2 * M_PI * i/65535);
        int32_t y = 40 * sin(2 * M_PI * i/65535);

        ((uint8_t*)0xA0000)[320 * (100 + y) + x + 160] = 0x00;
    }

    while (1);
    */

    return 0;
}