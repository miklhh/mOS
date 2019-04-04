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
#include <irq.h>
#include <io.h>
#include <memory/kalloc.h>
#include <assert.h>


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
        kprintf("Expected boot magic: '0x%x' vs acctual: '0x%x'", MULTIBOOT_BOOTLOADER_MAGIC, magic);
    }

    // Print kernel size.
    extern uintptr_t __kernel_start;
    extern uintptr_t __kernel_end;
    char kernel_size[10];
    format_memory(kernel_size, (uintptr_t)&__kernel_end - (uintptr_t)&__kernel_start);
    kprintf("Kernel position:      0x%08x -> 0x%08x [%s]\n", 
            &__kernel_start, &__kernel_end, kernel_size);

    // Print stack information.
    char stack_size[10];
    format_memory(stack_size, stack_top - stack_bottom);
    kprintf("Kernel stack:         0x%08x -> 0x%08x [%s]\n", 
            stack_bottom, stack_top, stack_size);

    // Acquire memory information.
    if ( !(info->flags & MULTIBOOT_INFO_MEMORY) )
    {
        kprintf("Fatal: No memory-info provided by bootloader.\n");
        kprintf("The system cannot continue. Halting and shuting down.");
        halt_and_shutdown();
    }
    else
    {
        char available_memory[10];
        format_memory(available_memory, info->mem_upper * 0x400);
        kprintf("Available memory:     0x%08x -> 0x%08x [%s]\n", 
                0x00100000, 
                0x00100000 + info->mem_upper*0x400, 
                available_memory);
    }

    // Acquire boot device.
    if ( !(info->flags & MULTIBOOT_INFO_BOOTDEV) )
    {
        kprintf("Notice: No boot-device-info provided by bootloader.");
    }

    // Initialze Interrupts & Co.
    system_init_idt();
    system_init_exceptions();
    system_init_irq();
    system_init_pit();

    // Initialze kernel heap allocation system. 
    system_init_kalloc();

    // Initialze paging and finalize the heap allocation system.
    system_init_paging(info->mem_upper * 0x400);
    system_init_heap(info->mem_upper * 0x400 + 0x100000);

    sti();
    while(1) 
    { 
        asm("hlt");
    }
}
