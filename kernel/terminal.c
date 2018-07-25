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

// Initialization routine.
void system_init_terminal()
{
    // Setup the system terminal and set the current terminal to the system
    // terminal. 
    system_terminal.x = 0;
    system_terminal.y = 0;
    system_terminal.buffer_size = DEFAULT_TERMINAL_BUFFER_SIZE;
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

// Retrive a character from the current buffer.
static inline char terminal_buffer_getc(size_t x, size_t y)
{
    return current_terminal->buffer[current_terminal->width * y + x];
}

// Synchronize the content of the buffer with the screen.
static void terminal_synchronize()
{
    if (current_terminal->setc == NULL)
    {
        return;
    }
    else
    {
        size_t y_end = current_terminal->buffer_size / current_terminal->width;
        size_t y_start = y_end - current_terminal->height;
        for (size_t y = y_start; y < y_end; ++y)
        {
            for (size_t x = 0; x < current_terminal->width; ++x)
                current_terminal->setc(terminal_buffer_getc(x, y), x, y);
        }
    }
}

// Test if a character is a 'spectial character'.
static inline bool special_character(char c)
{
    // A character is considered 'special' if it is among the first 31
    // characters.
    uint8_t c_num = (size_t) c;
    return c_num < 31;
}

// Routine for handling special characters.
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
    size_t y_buf_end = current_terminal->buffer_size / current_terminal->width;
    size_t y_buf = y_buf_end - current_terminal->height + y;

    if (special_character(c))
    {
        terminal_handle_special_character(c);
    }
    else
    {
        // Put the character in the buffer and than try to synchronize it with the
        // synchronize routine.
        current_terminal->buffer[y_buf * current_terminal->width + x] = c;
        if (current_terminal->setc != NULL)
        {
            current_terminal->setc(c, x, y);
        }

        // Increment the terminal x and y position. Handle position overflows. 
        if ( ++(current_terminal->x) > current_terminal->width )
        {
            current_terminal->x = 0;
            if ( ++(current_terminal->y) > current_terminal->height )
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
    // Guard for 'rows'.
    if (rows > current_terminal->height) return;
    
    // Scroll the terminal content. The content at the top of the terminal
    // buffer will get permanently destroyed.
    size_t y_end = current_terminal->buffer_size / current_terminal->width;
    for (size_t y = rows; y < y_end; ++y)
    {
        for (size_t x = 0; x < current_terminal->width; ++x)
        {
            size_t width = current_terminal->width;
            char c = current_terminal->buffer[y * width + x];
            current_terminal->buffer[(y - rows) * width + x] = c;
        }
    }

    // Clear out old data from buffer.
    for (size_t y = y_end - rows; y < y_end; ++y)
    {
        for (size_t x = 0; x < current_terminal->width; ++x)
        {
            size_t width = current_terminal->width;
            current_terminal->buffer[y * width + x] = (char) 0;
        }
    }
    
    // Synchronize the buffer with the (possible) screen.
    terminal_synchronize();
}
