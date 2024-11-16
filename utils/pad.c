/* pad.c - pad a file to a set size, zero filling with empty space. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    int padding_size = atoi(argv[2]);

    struct stat s;
    if (stat(input_filename, &s) < 0) {
        fprintf(stderr, "error stating file to pad: %s\n", strerror(errno));
        return -1;
    }

    int bytes_required = padding_size - s.st_size;

    if (padding_size == -1)
        bytes_required = (512 * ceil((double)s.st_size / 512.00)) - s.st_size;

    FILE *fp = fopen(input_filename, "ab");
    for (int i = 0; i < bytes_required; i++)
        fputc(0x00, fp);


    fclose(fp);


    return 0;
}