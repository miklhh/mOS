// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _SYSTEM_H
#define _SYSTEM_H

//#include <drivers/vga.h>
//#include <terminal.h>
#include <io.h>

// System initialization routines. 
extern void system_init_vga();
extern void system_init_terminal();
extern void system_init_gdt();
// extern void system_init_idt();
// extern void system_init_irq();
// extern void system_init_pit();


#endif
