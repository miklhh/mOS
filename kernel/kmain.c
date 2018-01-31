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

    // Test for successful multiboot.
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // PANIC!
        kprintf("Error: unsuccessful multiboot.\n");
        kprintf("Expected boot magic: 0x%x vs acctual: 0x%x", 
                MULTIBOOT_BOOTLOADER_MAGIC, magic);
    }

    // Print stack information.
    kprintf("Kernel stack bottom: 0x%08x\n", stack_bottom);
    kprintf("Kernel stack top:    0x%08x\n", stack_top);
    kprintf("Kernel stack size:   0x%x\n", stack_top - stack_bottom);

    // Acquire memory information.
    if (!(info->flags & MULTIBOOT_INFO_MEMORY))
    {
        kprintf("Error, no memory-info provided by bootloader.");
    }
    else
    {
        uintptr_t mem_lower = info->mem_lower * 0x400;
        uintptr_t mem_upper = info->mem_upper * 0x400;
        kprintf("Memory lower: 0x%08x\n", mem_lower);
        kprintf("Memory upper: 0x%08x\n", mem_upper);
    }

    // Acquire boot device
    if (!(info->flags & MULTIBOOT_INFO_BOOTDEV))
    {
        kprintf("Error, no boot-device-info provided by bootloader.");
    }
    else
    {
        kprintf("Root partition: 0x%x\n", info->boot_device);
    } 

    while(1) { }
}
