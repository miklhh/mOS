// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <drivers/vga.h>
#include <io.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static const uint16_t PORT_VGA_ADDR_REG = 0x3D4;
static const uint16_t PORT_VGA_DATA_REG = 0x3D5;
static const unsigned int SCREEN_WIDTH = 80;
static const unsigned int SCREEN_HEIGHT = 25;
static uint16_t *const VIDEO_MEMORY = (uint16_t *) 0x000B8000;
static bool initialized = false;

// Static function for creating a 16-bit colorcoded character.
static uint16_t create_char(unsigned char character, uint8_t color)
{
    return (uint16_t) color << 8 | character;
}

// Initialize vga routine.
void system_init_vga()
{
    vga_cursor_enable();
    initialized = true;
}

// Routine for writing a character to the screen via the vga chip.
void vga_putchar(unsigned char character, uint8_t color, size_t x, size_t y)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    VIDEO_MEMORY[y * SCREEN_WIDTH + x] = create_char(character, color);
}

// Small routine to be used by the terminal module.
void vga_putc(char c, size_t x, size_t y)
{
    uint8_t fg = VGA_COLOR_WHITE;
    uint8_t bg = VGA_COLOR_BLACK;
    uint8_t color = vga_color(fg, bg);
    vga_putchar((unsigned char)c, color, x, y);
}

char vga_getc(size_t x, size_t y)
{
    return (char)VIDEO_MEMORY[y * SCREEN_WIDTH + x];
}

// Enable the vga cursor.
void vga_cursor_enable()
{
    outb(PORT_VGA_ADDR_REG, 0x0A);
    outb(PORT_VGA_DATA_REG, (inb(0x3D5) & 0xC0));
    outb(PORT_VGA_ADDR_REG, 0x0B);
    outb(PORT_VGA_DATA_REG, (inb(0x3E0) & 0xE0));
}

// Dissable the text mode cursor.
void vga_cursor_disable()
{
    outb(PORT_VGA_ADDR_REG, 0x0A);
    outb(PORT_VGA_DATA_REG, 0x20);
}

// Set the text mode cursor.
void vga_cursor_set_position(size_t x, size_t y)
{
    // The 'y' position is of by one.
    y++;
    const uint16_t SELECT_CURSOR_HIGH = 0x0E;
    const uint16_t SELECT_CURSOR_LOW = 0x0F;
    uint16_t pos = y * SCREEN_WIDTH + x;
    uint8_t cursor_location_low = (uint8_t)(pos & 0xff);
    uint8_t cursor_location_high = (uint8_t)((pos >> 8) & 0xFF);
    outb(PORT_VGA_ADDR_REG, SELECT_CURSOR_LOW);
    outb(PORT_VGA_DATA_REG, cursor_location_low);
    outb(PORT_VGA_ADDR_REG, SELECT_CURSOR_HIGH);
    outb(PORT_VGA_DATA_REG, cursor_location_high);
}
