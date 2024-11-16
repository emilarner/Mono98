#ifndef ROMDISK_H
#define ROMDISK_H

#include <stdint.h>

#ifndef LINUX
#include "std/string.h"
#endif

#define ROMDISK_MAGIC "ROMDISK"

struct __attribute__((__packed__)) romdisk_header {
    uint8_t magic[7];
    uint16_t entries;
};

struct __attribute__((__packed__)) romdisk_entry {
    uint8_t filename[64]; // must be null terminated!
    uint32_t offset;
    uint32_t length;
};

struct romdisk_file {
    uint8_t *file;
    uint32_t length;
    uint8_t noent;
};

uint8_t valid_romdisk(uint8_t *romdisk);
void list_romdisk(uint8_t *romdisk, void (*callback)(struct romdisk_entry*));
struct romdisk_file get_romdisk_file(uint8_t *romdisk, const char *filename);

#endif