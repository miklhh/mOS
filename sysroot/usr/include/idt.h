// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define TASK_GATE               0x5
#define INTERRUPT_GATE          0xE
#define TRAP_GATE               0xF

// Routine for activating the IDT machinery.
void system_init_idt();

// Aggregate data type associated with the idt.
typedef struct __attribute__((packed)) idt_descriptor
{
    uint16_t size;
    uintptr_t offset;
} idt_descriptor_t;

// Create an IDT entry.
uint64_t idt_create_descriptor(void (*handler)(void), uint8_t dpl, uint8_t type, uint16_t selector);

// Add a new service routine to the the system IDT. Please make sure to first
// dissable interrups when using this routine.
void idt_add_isr(uint8_t vector, uint64_t descriptor);

// Load and activate idt.
extern void load_idt(const idt_descriptor_t *ptr);

// Inline functions for activating and deactivating interrupts.
static inline void sti() { asm("sti"); }
static inline void cli() { asm("cli"); }

#endif

