#include "romdisk.h"


uint8_t valid_romdisk(uint8_t *romdisk) {
    return !!memcmp(((struct romdisk_header*)romdisk)->magic, ROMDISK_MAGIC, 7);
}

void list_romdisk(uint8_t *romdisk, void (*callback)(struct romdisk_entry*)) {
    for (uint16_t i = 0; i < ((struct romdisk_header*)romdisk)->entries; i++)
        callback((struct romdisk_entry*)(romdisk + sizeof(struct romdisk_header)) + i);
}

struct romdisk_file get_romdisk_file(uint8_t *romdisk, const char *filename) {
    for (uint16_t i = 0; i < ((struct romdisk_header*)romdisk)->entries; i++) {
        struct romdisk_entry *entry = (struct romdisk_entry*)(romdisk + sizeof(struct romdisk_header)) + i;

        if (!strcmp(entry->filename, filename))
            return (struct romdisk_file){romdisk + entry->offset, entry->length, 0x00}; 
    }

    return (struct romdisk_file){0x00, 0x00, 0x01};
}