/* 
 * Part of mOS (Minimalistic Operating System).
 * Code may freely be used under the MIT-license.
 */

#include <drivers/vga.h>
#include <io.h>
#include <stddef.h>
#include <stdint.h>

static const uint16_t PORT_VGA_ADDR_REG = 0x3D4;
static const uint16_t PORT_VGA_DATA_REG = 0x3D5;
static const unsigned int SCREEN_WIDTH = 80;
static const unsigned int SCREEN_HEIGHT = 25;
static uint16_t *const VIDEO_MEMORY = (uint16_t *) 0xB8000;


static uint16_t create_char(unsigned char character, uint8_t color)
{
    return (uint16_t) color << 8 | character;
}

/* Initialize vga routine. */
void system_init_vga()
{
    /* Enable cursor. */
}

/* Routine for writing a character to the screen via the vga chip. */
void vga_putchar(unsigned char character, uint8_t color, size_t x, size_t y)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
    {
        return;
    }
    else
    {
        VIDEO_MEMORY[y * SCREEN_WIDTH + x] = create_char(character, color);
    }
}

void vga_cursor_enable()
{

}

void vga_cursor_disable()
{
    outb(PORT_VGA_ADDR_REG, 0x0A);
    outb(PORT_VGA_DATA_REG, 0x20);
}

void vga_cursor_set_position(size_t x, size_t y)
{
        (void) x;
        (void) y;
}
