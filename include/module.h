// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _KERNEL_MODULE_H
#define _KERNEL_MODULE_H

#include <multiboot.h>

// Module magic number.
#define MODULE_MAGIC        0xDEFFA666

// The different types of modules.
#define MOD_NO_TYPE         0x00000000
#define MOD_FLAT_BINARY     0x00000001
#define MOD_EASY_BINARY     0x00000002



void system_init_modules(
        multiboot_uint32_t flags, 
        multiboot_uint32_t mods_addr,
        multiboot_uint32_t mods_count);

#endif
