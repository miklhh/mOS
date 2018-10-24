// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <system.h>
#include <idt.h>
#include <kstdio.h>
#include <stdint.h>
#include <assert.h>

#define DESCRIPTOR_ENTRIES 256

static uint64_t system_idt_entries[DESCRIPTOR_ENTRIES];
static idt_descriptor_t system_idt;


void system_init_idt()
{
    // Setup the interrupt descriptor table.
    system_idt.size = sizeof(uint64_t) * DESCRIPTOR_ENTRIES;
    system_idt.offset = (uintptr_t) system_idt_entries;

    // Add the ISR default handle to all the entries.
    for (int i = 0; i < DESCRIPTOR_ENTRIES; ++i)
    {
        extern void isr_default_handler();
        system_idt_entries[i] = idt_create_descriptor(isr_default_handler, 0, INTERRUPT_GATE, 0x08);
    }

    // Load the IDT.
    load_idt(&system_idt);
}

// A word of warning. One cannot use a regular C-function and use as handler for
// an interrupt/trap/task gate. It has to be an interrupt-routine with the
// 'iretd' interrupt return. Only use extern forward decleard assembly interrupt
// routines when creating new gates.
uint64_t idt_create_descriptor(void (*handler)(void), uint8_t dpl, uint8_t type, uint16_t code_selector)
{
    assert(dpl <= 4);
    assert(type == TASK_GATE || type == INTERRUPT_GATE || type == TRAP_GATE);
    
    uint64_t entry = 0;
    entry |= ((uintptr_t) handler)                  & 0xFFFF0000;
    entry |= (1 << 15)                              & 0x00008000;
    entry |= (((uint16_t) dpl) << 13)               & 0x00006000;
    entry |= ((type == TASK_GATE) ? 1 << 12 : 0)    & 0x00001000;
    entry |= (((uint16_t) type) << 8)               & 0x00000F00;
    entry <<= 32;
    entry |= (((uint32_t) code_selector) << 16)          & 0xFFFF0000;
    entry |= ((uintptr_t) handler)                  & 0x0000FFFF;
    return entry;
}

void idt_add_isr(uint8_t vector, uint64_t descriptor)
{
    system_idt_entries[vector] = descriptor;
}
