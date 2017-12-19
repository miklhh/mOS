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
    system_terminal.putc = vga_putc;
    current_terminal = &system_terminal;
}

static inline bool special_character(char c)
{
    // A character is considered 'special' if it is among the first 31
    // characters.
    uint8_t c_num = (size_t) c;
    return c_num < 31;
}

static terminal_handle_special_character(char c)
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
void terminal_putc(char c)
{
    const size_t X = current_terminal->x;
    const size_t Y = current_terminal->y;
    const size_t TERMINAL_WIDTH = current_terminal->width;
    const size_t TERMINAL_HEIGHT = current_terminal->height;

    if (special_character(c))
    {
        terminal_handle_special_character(c);
    }
    else
    {
        // Put the character in the buffer and than try to synchronize it with the
        // synchronize routine.
        current_terminal->buffer[Y * DEFAULT_TERMINAL_WIDTH + X] = c;
        if (current_terminal->putc != NULL)
        {
            current_terminal->putc(c, X, Y);
        }

        // Increment the terminal x and y position. Handle position overflows. 
        if ( ++(current_terminal->x) > TERMINAL_WIDTH )
        {
            current_terminal->x = 0;
            if ( ++(current_terminal->y) > TERMINAL_HEIGHT )
            {
                terminal_scroll(1);
            }
        }
    }
}

void terminal_puts(const char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        terminal_putc(str[i]);
    }
}

void terminal_scroll(unsigned rows)
{

}
