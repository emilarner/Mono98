#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include "std/string.h"
#include "std/stdlib.h"
#include "colours.h"
#include "graphics.h"

struct terminal_buffer_node {
    uint32_t *buffer;
    struct terminal_buffer_node *next;
};

struct terminal {
    uint8_t *background;

    uint16_t cursor_x;
    uint16_t cursor_y;
    uint8_t show_cursor;

    uint8_t margin_x;
    uint8_t margin_y;

    uint16_t rows;
    uint16_t columns;

    uint8_t colour;

    struct terminal_buffer_node *screen;
    
    uint8_t text_buffer[256];
    uint8_t text_buflen;
    uint8_t text_offset;
};

/* the main kernel terminal. */
/* you can have multiple terminals for differing applications! */
extern struct terminal kernel_terminal;
extern uint8_t tty_no;

void tputs(struct terminal *term, const char *msg);
void terminal_scroll(struct terminal *term);
void init_terminal(struct terminal *term, uint16_t rows, uint16_t columns, uint8_t *background, uint8_t colour);

void display_text(const char *str, uint16_t x, uint16_t y);
void display_tty_no(void);
void refresh_terminal(struct terminal *term, uint8_t redraw);

void clear_terminal(struct terminal *term);

enum TerminalColour {
    TERM_BLACK = VGA_COLOR_BLACK,
    TERM_BLUE = VGA_COLOR_BLUE,
    TERM_GREEN = VGA_COLOR_GREEN,
    TERM_CYAN = VGA_COLOR_CYAN,
    TERM_RED = VGA_COLOR_RED,
    TERM_MAGENTA = VGA_COLOR_MAGENTA,
    TERM_BROWN = VGA_COLOR_BROWN,
    TERM_LGRAY = VGA_COLOR_LIGHT_GRAY,
    TERM_DGRAY = VGA_COLOR_DARK_GRAY,
    TERM_LBLUE = VGA_COLOR_LIGHT_BLUE,
    TERM_LGREEN = VGA_COLOR_LIGHT_GREEN,
    TERM_LCYAN = VGA_COLOR_LIGHT_CYAN,
    TERM_LRED = VGA_COLOR_LIGHT_RED,
    TERM_LMAGENTA = VGA_COLOR_LIGHT_MAGENTA,
    TERM_YELLOW = VGA_COLOR_YELLOW,
    TERM_WHITE = VGA_COLOR_WHITE
};

extern const uint8_t terminal_colours[16];
void set_colour_terminal(struct terminal *term, enum TerminalColour terminal_colour);

/* when the terminal driver receives a byte between 0xF0 and 0xFF, it sets the terminal colour */
/* to the last 4 bits of such a byte. we don't use extended ASCII, so this is fine. */
#define terminal_colour_code(color) (0xF0 | (uint8_t)color)

#endif