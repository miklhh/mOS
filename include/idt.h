// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define TASK_GATE               0x5
#define INTERUPT_GATE           0xE
#define TRAP_GATE               0xF

uint64_t idt_create_entry(uintptr_t handler_offset, int dpl, int type, unsigned selector);

#endif

