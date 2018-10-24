// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _VGA_H
#define _VGA_H

#include <stdint.h>
#include <stddef.h>

#define VGA_COLOR_BLACK             0
#define VGA_COLOR_BLUE              1
#define VGA_COLOR_GREEN             2
#define VGA_COLOR_CYAN              3
#define VGA_COLOR_RED               4
#define VGA_COLOR_MAGENTA           5
#define VGA_COLOR_BROWN             6
#define VGA_COLOR_LIGHT_GREY        7
#define VGA_COLOR_DARK_GREY         8
#define VGA_COLOR_LIGHT_BLUE        9
#define VGA_COLOR_LIGHT_GREEN       10
#define VGA_COLOR_LIGHT_CYAN        11
#define VGA_COLOR_LIGHT_RED         12
#define VGA_COLOR_LIGHT_MAGENTA     13
#define VGA_COLOR_LIGHT_BROWN       14
#define VGA_COLOR_WHITE             15

// Inline function for creating a vga color with foreground and background.
static inline uint8_t vga_color(uint8_t fg, uint8_t bg)
{
    return fg | bg << 4;
}

// Initialize the VGA unit.
void system_init_vga();

// VGA routines.
void vga_putchar(unsigned char character, uint8_t color, size_t x, size_t y);
void vga_putc(char c, size_t x, size_t y);
char vga_getc(size_t, size_t y);
void vga_cursor_enable();
void vga_cursor_disable();
void vga_cursor_set_position(size_t x, size_t y);

#endif
