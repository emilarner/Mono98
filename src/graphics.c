#include "graphics.h"

/* default bitmap codes for 7 bit ASCII characters. */
uint32_t default_font[128] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
/* printable*/
    0,
    0b00100001000010000000001000000000, /* ! */
    0b11011110110000000000000000000000, /* " */
    0b01010111110101011111010100000000, /* # */
    0b11111101001111100101111110000000, /* $ */
    0b10001000100010001000100010000000, /* % */
    0b01010010101111101010010100000000, /* & */
    0b10000100000000000000000000000000, /* ' */
    0b01111100001000010000011110000000, /* ( */
    0b11110000010000100001111100000000, /* ) */
    0b10101011101111101110101010000000, /* * */
    0b00100001001111100100001000000000, /* + */
    0b00000000000000001000100000000000, /* , */
    0b00000000001111100000000000000000, /* - */
    0b00000000000000011000110000000000, /* . */
    0b00001000100010001000100000000000, /* / */
    n_0,
    n_1,
    n_2,
    n_3,
    n_4,
    n_5,
    n_6,
    n_7,
    n_8,
    n_9,
    0b00100001000000000100001000000000, /* : */
    0b00100001000000000100010000000000, /* ; */
    0b00100010001000001000001000000000, /* < */
    0b00000111110000011111000000000000, /* = */
    0b10000010000010001000100000000000, /* > */
    0b11111100010011000000001000000000, /* ? */
    0b00111110101011011110101100000000, /* @ */
    l_a,
    l_b,
    l_c,
    l_d,
    l_e,
    l_f,
    l_g,
    l_h,
    l_i,
    l_j,
    l_k,
    l_l,
    l_m,
    l_n,
    l_o,
    l_p,
    l_q,
    l_r,
    l_s,
    l_t,
    l_u,
    l_v,
    l_w,
    l_x,
    l_y,
    l_z,
    0b11111100001000010000111110000000, /* [ */
    0b10000010000010000010000010000000, /* \ */
    0b11111000010000100001111110000000, /* ] */
    0b00100010101000100000000000000000, /* ^ */
    0b00000000000000000000111110000000, /* _ */
    0b10101101010000001110011100000000, /* ` */
    l_a,
    l_b,
    l_c,
    l_d,
    l_e,
    l_f,
    l_g,
    l_h,
    l_i,
    l_j,
    l_k,
    l_l,
    l_m,
    l_n,
    l_o,
    l_p,
    l_q,
    l_r,
    l_s,
    l_t,
    l_u,
    l_v,
    l_w,
    l_x,
    l_y,
    l_z,
    0b01100100000100010000011000000000, /* { */
    0b11111111111101111111111110000000, /* | */
    0b00110000010001000001001100000000, /* } */
    0b00001000110010101001111110000000, /* ~ */
    0
};

/* draw the background, given a pointer to data describing the framebuffer. */
void draw_background(uint8_t *img) {
#ifdef NO_BACKGROUND
#ifndef BACKGROUND_COLOUR
#define BACKGROUND_COLOUR 15
#endif
    for (uint16_t i = 0; i < KERNEL_RES_WIDTH * KERNEL_RES_HEIGHT; i++)
        ((uint8_t*)VIDEO_MEMORY)[i] = BACKGROUND_COLOUR;

    return;
#endif

    for (uint16_t i = 0; i < KERNEL_RES_WIDTH * KERNEL_RES_HEIGHT; i++)
        ((uint8_t*)VIDEO_MEMORY)[i] = img[i];
}


/* display a 5x5 bitmap character at some screen coordinate */
/* must be an odd dimensional bitmap, so there is a centre. */
/* probably a bad algorithm. */
void display_bitmap(uint32_t code, uint16_t x, uint16_t y, int multiplier) {
    for (int yi = y - 2; yi <= y + 2; yi++) {
        for (int xi = x - 2; xi <= x + 2; xi++) {
            int bitrow = yi - (y - 2);
            int bitcolumn = xi - (x - 2);

            if (code & (1 << (31 - bitrow*5 - bitcolumn)))
                ((uint8_t*)VIDEO_MEMORY)[KERNEL_RES_WIDTH * yi + xi] = ((uint8_t)code & 0b00001111);
        }
    }
}

uint8_t area[25];
uint16_t area_x = 65535;
uint16_t area_y = 65535;

void save_area(uint16_t x, uint16_t y) {
    area_x = x;
    area_y = y;

    for (uint16_t i = y - 2; i <= y + 2; i++)
        for (uint16_t j = x - 2; j <= x + 2; j++)
            area[(i - (y - 2)) * 5 + (j - (x - 2))] = ((uint8_t*)VIDEO_MEMORY)[KERNEL_RES_WIDTH * i + j];
}


void restore_area() {
    uint16_t x = area_x;
    uint16_t y = area_y;

    if (x == 65535 && y == 65535)
        return;

    for (uint16_t i = y - 2; i <= y + 2; i++)
        for (uint16_t j = x - 2; j <= x + 2; j++)
            ((uint8_t*)VIDEO_MEMORY)[KERNEL_RES_WIDTH * i + j] = area[(i - (y - 2)) * 5 + (j - (x - 2))];
}