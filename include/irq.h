// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _IRQ_H
#define _IRQ_H

#include <stdint.h>
#include <system.h>

#define MASTER_PIC_COMMAND      0x20
#define MASTER_PIC_DATA         0x21
#define SLAVE_PIC_COMMAND       0xA0
#define SLAVE_PIC_DATA          0xA1
#define PIC_EOI                 0x20

// Routine for activating the IRQ machinery.
void system_init_irq();

// Send End-of-interrupt to the slave of master PIC device.
void irq_send_eoi(uint8_t irq);

// Add a service routine for an interrupt request.
void irq_add(void (*handler)(struct regs *r), uint8_t irq_number);

#endif


