/*
 * Part of mOS (Minimalistic Operating System).
 * Code may freely be used under the MIT-license.
 */

#include <boot.h>
#include <system.h>
#include <stdint.h>

#include <drivers/vga.h>

void kmain(struct multiboot_info *info, uint32_t magic, uintptr_t stack_bottom)
{
    (void) info;
    (void) magic;
    (void) stack_bottom;

    if (magic != MULTIBOOT_HEADER_MAGIC)
    {
        // PANIC
    }
    system_init_vga();
    vga_putchar(':', vga_color(VGA_COLOR_BLUE, VGA_COLOR_RED), 0, 0);
    vga_putchar('D', vga_color(VGA_COLOR_RED, VGA_COLOR_BLUE), 1, 0);
    vga_cursor_disable();
}
