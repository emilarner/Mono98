#include "terminal.h"

struct terminal kernel_terminal;
uint8_t tty_no = 0;

/* initialize a terminal of certain rows and columns */
void init_terminal(struct terminal *term, uint16_t rows, uint16_t columns, uint8_t *background, uint8_t colour) {
    term->background = background;
    draw_background(term->background);
    
    term->show_cursor = 0;
    term->colour = colour;
    term->cursor_x = 0;
    term->cursor_y = 0;
    term->rows = rows;
    term->columns = columns;

    memset(term->text_buffer, 0x00, sizeof(term->text_buffer));
    term->text_buflen = 0;
    term->text_offset = 0;

    struct terminal_buffer_node *pool = kmalloc(sizeof(struct terminal_buffer_node) * rows);
    memset(pool, 0x00, sizeof(struct terminal_buffer_node) * rows);

    term->screen = pool;
    term->screen->buffer = kmalloc(sizeof(uint32_t) * columns);
    memset(term->screen->buffer, 0x00, sizeof(uint32_t) * columns);

    struct terminal_buffer_node *tmp = term->screen;
    
    /* set up the following linked list and allocate the buffers. */
    for (uint16_t i = 0; i < (rows - 1); i++) {
        tmp->next = &pool[i + 1];
        tmp->next->buffer = kmalloc(sizeof(uint32_t) * columns);
        memset(tmp->next->buffer, 0x00, sizeof(uint32_t) * columns);
        tmp = tmp->next;
    }

    pool[rows - 1].next = NULL;
}

void display_tty_no(void) {
    char s_tty_no[4];
    s_tty_no[3] = '\0';
    itoa(tty_no, s_tty_no);


}

void refresh_terminal(struct terminal *term, uint8_t redraw) {
    uint16_t row = 0;

    if (redraw)
        draw_background(term->background);

    for (struct terminal_buffer_node *tmp = term->screen; tmp; tmp = tmp->next, row++) {
        for (uint16_t i = 0; i < term->columns; i++) {
            if ((30 + row * 8) == area_y && (30 + i * 8) == area_x)
                restore_area();

            if (row == term->cursor_y && i == term->cursor_x && term->show_cursor) {
                save_area(30 + i * 8, 30 + row * 8);
                display_bitmap(0b11111111111111111111111110000000, 30 + i * 8, 30 + row * 8, 0);
                continue;
            }

            display_bitmap(tmp->buffer[i], 30 + i*8, 30 + row*8, 0);
        }
    }
}

void terminal_scroll(struct terminal *term) {
    struct terminal_buffer_node *tmp1 = term->screen;

    /* 2nd row becomes first, the head of the linked list. */
    term->screen = term->screen->next;
    term->cursor_y--;

    struct terminal_buffer_node *tail = NULL;
    for (struct terminal_buffer_node *tmp2 = term->screen; tmp2 != NULL; tmp2 = tmp2->next)
        tail = tmp2;


    tail->next = tmp1;
    tail->next->next = NULL;

    memset(tail->next->buffer, 0x00, sizeof(uint32_t) * term->columns);
    draw_background(term->background);
}

void tputs(struct terminal *term, const char *msg) {
#ifdef LOLCAT
    static uint8_t lolcat_index = 0;
#endif
    for (uint16_t i = 0; msg[i]; i++) {
        /* handle colour codes. */
        if ((msg[i] & 0b11110000) == 0b11110000) {
            term->colour = msg[i] & (0b00001111);
            continue;
        }

        if (term->cursor_x > (term->columns - 1)) {
            term->cursor_x = 0;
            term->cursor_y++;
        }

        /* handle newline character */
        /* debating on whether I should be autistic and use CRLF (\r\n) instead... */
        if (msg[i] == '\n') {
            term->cursor_y++;
            term->cursor_x = 0;

            if (term->cursor_y > (term->rows - 1))
                terminal_scroll(term);

            continue;
        }

        /* handle backspace character */
        if (msg[i] == '\b') {
            if (term->cursor_x == 0 && term->cursor_y == 0)
                continue;

            term->cursor_x--;

            /* underflow indicating up a previous line. */
            if (term->cursor_x == 0xFFFF) {
                term->cursor_x = term->columns - 1;
                term->cursor_y--;
            }

            struct terminal_buffer_node *tmp = term->screen;
            for (int j = 0; j < term->cursor_y; j++)
                tmp = tmp->next;

            tmp->buffer[term->cursor_x] = 0x00000000;
            refresh_terminal(term, 1);

            continue;
        }

        if (term->cursor_y > (term->rows - 1))
            terminal_scroll(term);

        /* null bitmap indicates nothing to be printed. */
        uint32_t bitmap = 0x00000000;

        /*
        if ('a' <= (msg[i] | (1 << 5)) && (msg[i] | (1 << 5) <= 'z'))
            bitmap = font_letters[(msg[i] | (1 << 5)) - 'a'];
        
        else if ('0' <= msg[i] && msg[i] <= '9')
            bitmap = font_numbers[msg[i] - '0'];
            */

#ifdef LOLCAT
        bitmap = default_font[msg[i]] | terminal_colours[lolcat_index];
        lolcat_index = (lolcat_index + 1) % 16; 
#endif
#ifndef LOLCAT
        bitmap = default_font[msg[i]] | (uint32_t)term->colour;
#endif

        struct terminal_buffer_node *tmp = term->screen;
        for (int j = 0; j < term->cursor_y; j++)
            tmp = tmp->next;

        tmp->buffer[term->cursor_x] = bitmap;
        term->cursor_x++;
    }

    refresh_terminal(term, 0);
}

void clear_terminal(struct terminal *term) {
    for (struct terminal_buffer_node *node = term->screen; node; node = node->next)
        memset(node->buffer, 0x00, term->columns * sizeof(uint32_t));

    term->cursor_x = 0;
    term->cursor_y = 0;

    refresh_terminal(term, 1);
}

const uint8_t terminal_colours[16] = {
    TERM_BLACK,
    TERM_BLUE,
    TERM_GREEN,
    TERM_CYAN,
    TERM_RED,
    TERM_MAGENTA,
    TERM_BROWN,
    TERM_LGRAY,
    TERM_DGRAY,
    TERM_BLUE,
    TERM_LGREEN,
    TERM_LCYAN,
    TERM_LRED,
    TERM_LMAGENTA,
    TERM_YELLOW,
    TERM_WHITE
};


void set_colour_terminal(struct terminal *term, enum TerminalColour terminal_colour) {
    term->colour = terminal_colour;
}