// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <stdint.h>

// System initialization routines. Preferably, the should get called in this
// perticular order.
extern void system_init_vga();
extern void system_init_terminal();
extern void system_init_gdt();
extern void system_init_idt();
extern void system_init_exceptions();
extern void system_init_irq();
extern void system_init_pit();

// Halt and shutdown routine. Calling this routine will dissable intterupts and
// halt the processor. There is no going back from this.
extern void halt_and_shutdown();

struct regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

#endif
