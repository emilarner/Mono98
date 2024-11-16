#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>

#define LINUX
#include "../src/romdisk.h"

const char *basename(const char *filename, char c) {
    char *first = filename;
    char *second = strchr(filename, c) + 1;

    while (1) {
        first = second;
        second = strchr(second, c);

        if (!second)
            break;
    }

    return first;
}

bool basename_sanity(const char *bname) {
    for (size_t i = 0; bname[i]; i++) {
        if (!('A' <= bname[i] && bname[i] <= 'Z'))
            return false;
    }

    return true;
}

void read_files(struct romdisk_entry *entry) {
    puts(entry->filename);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "error: we need at least two arguments: the output filename and at least one file.\n");
        fprintf(stderr, "... or alternatively, the first argument being --read and the second arg being the file.\n");

        return -1;
    }

    if (!strcmp(argv[1], "--read")) {
        char *input = argv[2];
        FILE *input_fp = fopen(input, "rb");
        struct stat s;

        if (stat(input, &s) < 0) {
            fprintf(stderr, "error stat() while reading ROMDISK file: %s\n", strerror(errno));
            return 01;
        }

        uint8_t *file = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fileno(input_fp), 0);
        list_romdisk(file, &read_files);
        struct romdisk_file f = get_romdisk_file(file, "HELLO");
        write(STDOUT_FILENO, f.file, f.length);

        munmap(file, s.st_size);

        return 0;
    }


    char *output = argv[1];
    FILE *output_fp = fopen(output, "wb");

    uint16_t entries = argc - 2;

    /* write the header */
    struct romdisk_header header;
    memcpy(header.magic, ROMDISK_MAGIC, 7);
    header.entries = entries;
    fwrite(&header, 1, sizeof(header), output_fp);

    uint32_t offset = sizeof(header) + sizeof(struct romdisk_entry) * entries;
    
    /* calculate offsets and lengths for the romdisk entries. */
    for (int i = 2; i < argc; i++) {
        struct stat s;
        if (stat(argv[i], &s) < 0) {
            fprintf(stderr, "error stat on file '%s': %s\n", argv[i], strerror(errno));
            return -1;
        }

        struct romdisk_entry entry;
        
        const char *file_basename = basename(argv[i], '/');
        if (!basename_sanity(file_basename)) {
            fprintf(stderr, "error: sanity check failed for file '%s'\n", file_basename);
            fprintf(stderr, "hint: files under the ROMDISK format can only be A-Z\n");
            return -1;
        }

        strncpy(entry.filename, file_basename, sizeof(entry.filename));
        entry.offset = offset;
        entry.length = s.st_size;
        fwrite(&entry, 1, sizeof(entry), output_fp);

        offset += s.st_size;
    }

    for (int i = 2; i < argc; i++) {
        FILE *input_fp = fopen(argv[i], "rb");

        char buffer[4096];
        while (!feof(input_fp)) {
            size_t len = fread(buffer, 1, sizeof(buffer), input_fp);
            fwrite(buffer, 1, len, output_fp);
        }

        fclose(input_fp);
    }

    fclose(output_fp);
    return 0;
}