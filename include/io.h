// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _io_h
#define _io_h

#include <stdint.h>

#define IO_WAIT() \
    do                                      \
    {                                       \
        asm volatile("jmp 1f        \n\t"   \
                     "1:            \n\t"   \
                     "    jmp 2f    \n\t"   \
                     "2:                "); \
    } while(0)

uint8_t  inb  (uint16_t port);
uint16_t inw  (uint16_t port);
uint32_t inl  (uint16_t port);
void     outb (uint16_t port, uint8_t val);
void     outw (uint16_t port, uint16_t val);
void     outl (uint16_t port, uint32_t val);

#endif
