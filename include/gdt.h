// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

#define ACCESS_PRESENT      ((1 << 7) | (1 << 4))
#define ACCESS_PRIVL(x)     ((x & 0x03) << 5)
#define ACCESS_EXECUTE      (1 << 3)
#define ACCESS_DIRECTION    (1 << 2)
#define ACCESS_R_W          (1 << 1)

// Routine for activating the GDT machinery.
void system_init_gdt();

// Aggregate data type associated with the gdt.
typedef struct __attribute__((packed)) gdt_descriptor
{
    uint16_t size;
    uintptr_t offset;
} gdt_descriptor_t;

// Load and activate gdt.
extern void load_gdt(const gdt_descriptor_t *ptr);

// Create a GDT entry.
uint64_t create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access);


#endif
