// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <boot.h>
#include <system.h>
#include <stdint.h>
#include <drivers/vga.h>
#include <terminal.h>

void kmain(struct multiboot_info *info, uint32_t magic, uintptr_t stack_bottom)
{
    (void) info;
    (void) magic;
    (void) stack_bottom;

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // PANIC
    }
    system_init_vga();
    system_init_terminal();
    terminal_putc('A');
    terminal_putc('B');
    terminal_putc('C');
    terminal_putc(' ');
    terminal_puts("Hello kernel world! Have a nice day!\n");
    terminal_puts("-- WOOW! --");
    //vga_putchar(':', vga_color(VGA_COLOR_BLUE, VGA_COLOR_RED), 0, 0);
    //vga_putchar('D', vga_color(VGA_COLOR_RED, VGA_COLOR_BLUE), 1, 0);

    vga_cursor_set_position(5, 20);
    vga_cursor_disable();
    vga_cursor_enable();
    vga_cursor_set_position(20, 20);

}
