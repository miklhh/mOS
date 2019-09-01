// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <module.h>
#include <multiboot.h>
#include <kstdio.h>
#include <system.h>

static char *mod_name[] = {
    "No type",
    "Flat binary",
    "Easy binary v1.0"
};

void system_init_modules(
        multiboot_uint32_t flags, 
        multiboot_uint32_t mods_addr,
        multiboot_uint32_t mods_count)
{
    if ( !(flags & MULTIBOOT_INFO_MODS) )
    {
        // No modules provided by boot loader.
        kprintf("No module information provided by bootloader. Fatal error!");
        halt_and_shutdown();
    }

    // Print and store modules.
    kprintf("Modules loaded by bootloader: %d\n", mods_count);
    multiboot_module_t *const module_array = (multiboot_module_t *) mods_addr;
    for (unsigned i = 0; i < mods_count; ++i)
    {
        uint32_t *module_start_address = (uint32_t *)module_array[i].mod_start;

        // Continue here...
        void (* func)() = (void (*)()) module_start_address;
        func();
    }
    
    // REMOVE LATER!
    //kprintf("Temporarly stopped.");
    //halt_and_shutdown();
}

