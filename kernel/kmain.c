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
#include <string.h>
#include <format_string.h>
#include <idt.h>

void kmain(
        struct multiboot_info *info, 
        uint32_t magic, 
        uintptr_t stack_bottom, 
        uintptr_t stack_top)
{
    (void) info;
    (void) stack_bottom;

    // Initialze GDT.
    system_init_gdt();

    // Initialize VGA and kernel terminal.
    system_init_vga();
    system_init_terminal();

    // Test for successful multiboot.
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // PANIC!
        kprintf("Error: unsuccessful multiboot.\n");
        kprintf("Expected boot magic: '0x%x' vs acctual: '0x%x'", 
                MULTIBOOT_BOOTLOADER_MAGIC, magic);
    }

    // Print stack information.
    char available_stack[10];
    format_memory(available_stack, stack_top - stack_bottom);
    kprintf("Kernel stack size: %s\n", available_stack);

    // Acquire memory information.
    if ( !(info->flags & MULTIBOOT_INFO_MEMORY) )
    {
        kprintf("Error, no memory-info provided by bootloader.");
    }
    else
    {
        char available_memory[10];
        format_memory(available_memory, info->mem_upper * 0x400);
        kprintf("Available memory: %s\n", available_memory);
    }

    // Acquire boot device.
    if ( !(info->flags & MULTIBOOT_INFO_BOOTDEV) )
    {
        kprintf("Error, no boot-device-info provided by bootloader.");
    }
    else
    {
        kprintf("Root partition: 0x%x\n", info->boot_device);
    }

    kprintf("'kmain' location: 0x%x\n", (uintptr_t) kmain);

    system_init_idt();
    system_init_exceptions();
    sti();
    
    volatile int i = 5;
    volatile int j = 0;
    i / j;

    
    while(1) { }
}
