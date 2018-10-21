// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <terminal.h>
#include <drivers/vga.h>
#include <stdbool.h>

static size_t terminal_width;
static size_t terminal_height;
static size_t terminal_x;
static size_t terminal_y;
static void (*put_char)(char c, size_t x, size_t y);
static void (*put_curs)(size_t x, size_t y);
static char (*get_char)(size_t x, size_t y);

void system_init_terminal()
{
    // Setup the system terminal.
    terminal_width = DEFAULT_TERMINAL_WIDTH;
    terminal_height = DEFAULT_TERMINAL_HEIGHT;
    terminal_x = 0;
    terminal_y = 0;
    put_char = vga_putc;
    put_curs = vga_cursor_set_position;
    get_char = vga_getc;
    put_curs(0, 0);

    // Clear the terminal.
    terminal_reset();
}

static inline bool terminal_special_character(char c)
{
    // Returns true if 'c' is considered a special character that needs special
    // processing. The first 31 characters is considnered special, in that they
    // are handeled individually.
    return ((uint8_t) c) <= 31;
}

static inline void terminal_handle_special_character(char c)
{
    const char CARRIAGE_RETURN = (char) 0x0D;
    const char LINE_FEED = (char) 0x0A;

    if (c == LINE_FEED) // Newline.
    {
        if ( ++terminal_y >= DEFAULT_TERMINAL_HEIGHT )
        {
            terminal_scroll(1);
            terminal_x = 0;
        }
        else
        {
            terminal_x = 0;
        }
        
    }
    else if (c == CARRIAGE_RETURN)
    {
        terminal_x = 0;
    }
}

static void terminal_putc_internal(char c)
{
    // Guard for special characters, otherwise put the character on screen.
    if ( terminal_special_character(c) )
    {
        terminal_handle_special_character(c);
    }
    else
    {
        put_char(c, terminal_x, terminal_y);
        if ( ++terminal_x >= DEFAULT_TERMINAL_WIDTH )
        {
            terminal_x = 0;
            if ( ++terminal_y >= DEFAULT_TERMINAL_HEIGHT )
            {
                terminal_scroll(1);
            }
        }
    }
}

void terminal_putc(char c)
{
    terminal_putc_internal(c);
    put_curs(terminal_x, terminal_y);
}

void terminal_scroll(unsigned rows)
{
    if (rows == 0) return;

    // Scroll all the terminal content. The content at the top of the terminal
    // will be permanently destroyed.
    for (unsigned y = rows; y < DEFAULT_TERMINAL_HEIGHT; ++y)
    {
        for (unsigned x = 0; x < DEFAULT_TERMINAL_WIDTH; ++x)
            put_char( get_char(x, y), x, y - rows );
    }

    // Clear the newly freed lines.
    for (unsigned y = DEFAULT_TERMINAL_HEIGHT - rows; y < DEFAULT_TERMINAL_HEIGHT; ++y)
    {
        for (unsigned x = 0; x < DEFAULT_TERMINAL_WIDTH; ++x)
            put_char(' ', x, y);
    }

    // Decrement the terminal y coordinate.
    put_curs(terminal_x, terminal_y -= rows);
}

void terminal_reset()
{
    for (unsigned y = 0; y < DEFAULT_TERMINAL_HEIGHT; ++y)
    {
        for (unsigned x = 0; x < DEFAULT_TERMINAL_WIDTH; ++x)
        {
            vga_putc(' ', x, y);
        }
    }
    put_curs(0, 0);
}
