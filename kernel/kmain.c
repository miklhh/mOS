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
#include <module.h>
#include <thread.h>
#include <memory/kalloc.h>
#include <assert.h>

static void kmain2(void *aux)
{
    (void) aux;

    volatile int i = 0;
    while(1)
    {
        for (i; i < 40000000; ++i) { }
        kprintf("Call from: %s\n", thread_current()->name);
        i = 0;
    }
}


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
        // Multiboot failure.
        kprintf("Error: unsuccessful multiboot.\n");
        kprintf("Expected boot magic: '0x%x' vs acctual: '0x%x'", MULTIBOOT_BOOTLOADER_MAGIC, magic);
        halt_and_shutdown();
    }

    // Print kernel size.
    extern uintptr_t __kernel_start;
    extern uintptr_t __kernel_end;
    char kernel_size[10];
    format_memory(kernel_size, (uintptr_t)&__kernel_end - (uintptr_t)&__kernel_start);
    kprintf("Kernel position:      0x%08x -> 0x%08x [%s]\n", 
            &__kernel_start, &__kernel_end, kernel_size);

    // Print kernel stack information.
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

    // Initialze Interrupts & Co.
    system_init_idt();
    system_init_exceptions();
    system_init_irq();
    sti();

    // Initialize programmable interrupt timer.
    system_init_pit();

    // Initialize the early threading system.
    system_init_threading();
    thread_create("test_thread", PRI_DEFAULT, kmain2, NULL);

    for (volatile int i = 0; i < 20000000; ++i) { }
    volatile int j = 0;
    while(1)
    {
        for (j; j < 40000000; ++j) { }
        kprintf("Call from: %s\n", thread_current()->name);
        j = 0;
    }

    //
    // SCIPTICISM FROM THIS POINT.
    //

    // Initialize kernel dynamic allocation system. It should be initialzed to
    // start directly after the last module was loaded.
    system_init_kalloc(get_kalloc_start(info));

    // Initialze paging and finalize the heap allocation system.
    system_init_paging(info->mem_upper * 0x400);
    system_init_heap(info->mem_upper * 0x400 + 0x100000);


    // Testing module.
    system_init_modules(info->flags, info->mods_addr, info->mods_count);
    kprintf("info->flags & MULTIBOOT_INFO_MODS = %d\n", info->flags & MULTIBOOT_INFO_MODS);
    kprintf("info->mods_count = %d\n", info->mods_count);
    kprintf("info->mods_addr = %d\n", info->mods_addr);
    //kprintf("Error, could not load module.");
    halt_and_shutdown();

    sti();
    while(1) 
    { 
        asm("hlt");
    }
}
