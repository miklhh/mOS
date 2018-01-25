// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <terminal.h>
#include <drivers/vga.h>
#include <stdbool.h>

static terminal_t  system_terminal;
static terminal_t *current_terminal;

void system_init_terminal()
{
    // Setup the system terminal and set the current terminal to the system
    // terminal. 
    system_terminal.x = 0;
    system_terminal.y = 0;
    system_terminal.width = DEFAULT_TERMINAL_WIDTH;
    system_terminal.height = DEFAULT_TERMINAL_HEIGHT;
    system_terminal.setc = vga_putc;
    system_terminal.set_cursor = vga_cursor_set_position;
    current_terminal = &system_terminal;

    // Clear the terminal.
    memset(system_terminal.buffer, 0, DEFAULT_TERMINAL_BUFFER_SIZE);
    for (size_t y = 0; y < DEFAULT_TERMINAL_HEIGHT; ++y)
    {
        for (size_t x = 0; x < DEFAULT_TERMINAL_WIDTH; ++x)
            current_terminal->setc((char)0, x, y);
    }

    // Reset the cursor in the terminal.
    current_terminal->set_cursor(0, 0);
}

static inline bool special_character(char c)
{
    // A character is considered 'special' if it is among the first 31
    // characters.
    uint8_t c_num = (size_t) c;
    return c_num < 31;
}

static void terminal_handle_special_character(char c)
{
    // Handle 'Carriage return', 'Line feed' and 'Newline'.
    const char CARRIAGE_RETURN = (char) 0x0D;
    const char LINE_FEED = (char) 0x0A;
    if (c == CARRIAGE_RETURN || c == LINE_FEED)
    {
        current_terminal->x = 0;
    }
    if (c == LINE_FEED)
    {
        const size_t TERMINAL_HEIGHT = current_terminal->height;
        if (current_terminal->y >= TERMINAL_HEIGHT)
        {
            terminal_scroll(1);
        }
        else
        {
            ++(current_terminal->y);
        }
    }
}

// Routine for puting a character to the current terminal.
static void terminal_putc_internal(char c)
{
    size_t x = current_terminal->x;
    size_t y = current_terminal->y;
    size_t terminal_width = current_terminal->width;
    size_t terminal_height = current_terminal->height;

    if (special_character(c))
    {
        terminal_handle_special_character(c);
    }
    else
    {
        // Put the character in the buffer and than try to synchronize it with the
        // synchronize routine.
        current_terminal->buffer[y * DEFAULT_TERMINAL_WIDTH + x] = c;
        if (current_terminal->setc != NULL)
        {
            current_terminal->setc(c, x, y);
        }

        // Increment the terminal x and y position. Handle position overflows. 
        if ( ++(current_terminal->x) > terminal_width )
        {
            current_terminal->x = 0;
            if ( ++(current_terminal->y) > terminal_height )
            {
                terminal_scroll(1);
            }
        }
    }
}

void terminal_putc(char c)
{
    terminal_putc_internal(c);
    if (current_terminal->set_cursor != NULL)
    {
        current_terminal->set_cursor(current_terminal->x, current_terminal->y);
    }
}

void terminal_puts(const char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        terminal_putc_internal(str[i]);
    }
    if (current_terminal->set_cursor != NULL)
    {
        current_terminal->set_cursor(current_terminal->x, current_terminal->y);
    }
}

void terminal_scroll(unsigned rows)
{

}
