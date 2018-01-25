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
#include <kstdio.h>

void kmain(
        struct multiboot_info *info, 
        uint32_t magic, 
        uintptr_t stack_bottom, 
        uintptr_t stack_top)
{
    (void) info;
    (void) stack_bottom;

    // Initialize VGA and kernel terminal.
    system_init_vga();
    system_init_terminal();

    // Tset for successful multiboot.
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // PANIC!
        kprintf("Error: unsuccessful multiboot.\n");
        kprintf("Expected boot magic: 0x%x vs acctual: 0x%x", 
                MULTIBOOT_BOOTLOADER_MAGIC, magic);
    }
    else
    {
        kprintf("Successful multiboot. Magic: 0x%x\n", magic);
        kprintf("Kernel stack bottom: 0x%x\n", stack_bottom);
        kprintf("Kernel stack top:    0x%x\n", stack_top);
        kprintf("Kernel stack size:   0x%x\n", stack_top - stack_bottom);
    }

    while(1) { }

}
