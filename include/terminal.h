// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdint.h>
#include <stddef.h>

#define DEFAULT_TERMINAL_WIDTH              80
#define DEFAULT_TERMINAL_HEIGHT             25
#define DEFAULT_TERMINAL_BUFFER_SIZE        80 * 100

// System terminal initialization routine. This routine will initialize the
// system terminal and set the current terminal to the system terminal.
void system_init_terminal();

// Routine for writing a character/string to the current terminal. These
// routines will try to synchronize the input the with the external device
// specified in the terminal structure.
void terminal_putc(char c);
void terminal_puts(const char *str);

// Routine for scrolling the current terminal. The routine will scroll the
// terminal 'rows' times.
void terminal_scroll(unsigned rows);

// Reset the current terminal.
void terminal_reset();

// This structure is used to store information about a terminal.
typedef struct terminal
{
    // Terminal buffer. Internal storage of the terminal content.
    unsigned char buffer[DEFAULT_TERMINAL_BUFFER_SIZE];
    void (*setc)(char c, size_t x, size_t y);
    void (*set_cursor)(size_t x, size_t y);

    // Terminal attributes.
    size_t width;
    size_t height;
    size_t x;
    size_t y;
} terminal_t;

#endif
