// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <gdt.h>
#include <stdint.h>

#define DESCRIPTOR_SEGMENTS 3

static uint64_t system_gdt_entries[DESCRIPTOR_SEGMENTS];
static gdt_descriptor_t system_gdtd;

void system_init_gdt()
{
    // Initialize by creating these descriptors.
    // - NULL descriptor.                     Offset: 0x00
    // - Code segment descriptor for kernel.  Offset: 0x08
    // - Data segment descriptor for kernel.  Offset: 0x10
    // - A TSS segment.                       Offset: 0x18
    uint8_t cs_kernel = (ACCESS_PRESENT | ACCESS_PRIVL(0) | ACCESS_R_W | ACCESS_EXECUTE);
    uint8_t ds_kernel = (ACCESS_PRESENT | ACCESS_PRIVL(0) | ACCESS_R_W);
    system_gdt_entries[0] = 0;
    system_gdt_entries[1] = create_gdt_entry(0x0, 0x000fffff, cs_kernel);
    system_gdt_entries[2] = create_gdt_entry(0x0, 0x000fffff, ds_kernel);

    system_gdtd.size = sizeof(uint64_t) * DESCRIPTOR_SEGMENTS - 1;
    system_gdtd.offset = (uintptr_t) system_gdt_entries;
    load_gdt(&system_gdtd);
}

uint64_t create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access)
{
    uint64_t descriptor = 0;
    descriptor |=  limit        & 0x000F0000;
    descriptor |= (0xC0 << 16)  & 0x00F00000;
    descriptor |= (access << 8) & 0x0000FF00;
    descriptor |= (base >> 16)  & 0x000000FF;
    descriptor |=  base         & 0xFF000000;

    descriptor <<= 32;
    descriptor |= (base << 16)  & 0xFFFF0000;
    descriptor |=  limit        & 0x0000FFFF;
    return descriptor;
}

