// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//
// The terminal module is used to display text and data to the screen with the
// VGA-unit.
//

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdint.h>
#include <stddef.h>

#define DEFAULT_TERMINAL_WIDTH              80
#define DEFAULT_TERMINAL_HEIGHT             24

// System terminal initialization routine. This routine will initialize the
// system terminal and set the current terminal to the system terminal.
void system_init_terminal();

void terminal_putc(char c);
void terminal_puts(const char *str);
void terminal_scroll(unsigned rows);
void terminal_reset();

#endif
