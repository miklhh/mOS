// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <system.h>
#include <idt.h>
#include <kstdio.h>
#include <stdint.h>

#define DESCRIPTOR_ENTIRES 256

static uint64_t system_idt_entries[DESCRIPTOR_ENTIRES];
static idt_descriptor_t system_idt;


void system_init_idt()
{

}

uint64_t idt_create_entry(uintptr_t handler_offset, int dpl, int type, unsigned selector)
{
    (void) handler_offset;
    (void) dpl;
    (void) type;
    (void) selector;
    return 0;
}
