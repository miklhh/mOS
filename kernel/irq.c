// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <irq.h>
#include <stdint.h>
#include <io.h>
#include <idt.h>
#include <system.h>
#include <kstdio.h>
#include <assert.h>

#define IRQ_COUNT       16
#define NULL            0

// Send End-of-interrupt to the slave of master PIC device.
void irq_send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(SLAVE_PIC_COMMAND, PIC_EOI);
    }
    outb(MASTER_PIC_COMMAND, PIC_EOI);
}

// Array of IRQ routines.
static void (*irq_routine[IRQ_COUNT])(struct regs *r) = { 0 };

// Add a service routine for an interrupt request.
void irq_add(void (*handler)(struct regs *r), uint8_t irq_number)
{
    assert(irq_number < IRQ_COUNT);
    irq_routine[irq_number] = handler;
}

// The IRQ handler.
void irq_handler(struct regs *r)
{
    // Clear interrupt flag.
    cli();

    // Make sure this handler was called correctly.
    if (r->int_no <= 0x4f && r->int_no >= 0x20)
    {
        if ( (uintptr_t)irq_routine[r->int_no - 32] == NULL )
        {
            // No IRQ-routine exists for this
            kprintf("IRQ: 0x%x not handled.\n", r->int_no - 32);
        }
        else
        {
            // The routine exists. Call it.
            irq_routine[r->int_no - 32](r);
        }
        irq_send_eoi(r->int_no - 32);
    }
    else
    {
        // This handler was somehow called by the wrong underlying handler.
        // PANIC!?!
        kprintf("--------------------------------------------------------------------------------");
        kprintf("    'irq_handler()' called with inproper interrupt number: .\n", r->int_no);
        kprintf("    %s:%d (%s)\n", __FILE__, __LINE__, __func__);
        kprintf("    The system cannot recover from such an error. Halting machine.\n\n");
        kprintf("    EAX: 0x%08x   EBX: 0x%08x   ECX: 0x%08x   EDX: 0x%08x\n", r->eax, r->ebx, r->ecx, r->edx);
        kprintf("    EDI: 0x%08x   ESI: 0x%08x   EBP: 0x%08x   ESP: 0x%08x\n", r->edi, r->esi, r->ebp, r->esp);
        kprintf("    DS:  0x%08x   ES:  0x%08x   FS:  0x%08x   GS:  0x%08x\n", r->ds, r->es, r->fs, r->gs);
        kprintf("    EIP: 0x%08x   CS:  0x%08x   SS:  0x%08x   UESP:0x%08x\n", r->eip, r->cs, r->ss, r->useresp);
        kprintf("    Error code: 0x%x\n    Interrupt number: %d\n", r->err_code, r->int_no);
        kprintf("--------------------------------------------------------------------------------");
        halt_and_shutdown();
    }

    // Set interrupt flag again.
    sti();
}

// Routine for activating the IRQ machinery.
void system_init_irq()
{
    // Store the '8259' masks.
    uint8_t a1 = inb(MASTER_PIC_DATA);
    uint8_t a2 = inb(SLAVE_PIC_DATA);

    // Initialze the PIC devices.
    outb(MASTER_PIC_COMMAND, 0x10 | 0x01); IO_WAIT();
    outb(SLAVE_PIC_COMMAND,  0x10 | 0x01); IO_WAIT();

    // Set the PIC ISR vector offsets.
    outb(MASTER_PIC_DATA, 0x20); IO_WAIT();
    outb(SLAVE_PIC_DATA,  0x28); IO_WAIT();

    // Cascade identity with slave PIC at IRQ2.
    outb(MASTER_PIC_DATA, 0x04); IO_WAIT();
    outb(SLAVE_PIC_DATA,  0x02); IO_WAIT();

    // Request 8086 mode on each PIC.
    outb(MASTER_PIC_DATA, 0x01); IO_WAIT();
    outb(SLAVE_PIC_DATA,  0x01); IO_WAIT();

    // Restore masks.
    outb(MASTER_PIC_DATA, a1);
    outb(SLAVE_PIC_DATA,  a2);

    // Forgive me father, for I have sinned. Beyond this point, there is only
    // wrath and fear to be found. No one should have to witness this, ever.
    // The deadliest of sins 'code duplication' is unforgivable.
    // Forgive me father, for I have sinned...
    extern void irq_default_handler_32();
    extern void irq_default_handler_33();
    extern void irq_default_handler_34();
    extern void irq_default_handler_35();
    extern void irq_default_handler_36();
    extern void irq_default_handler_37();
    extern void irq_default_handler_38();
    extern void irq_default_handler_39();
    extern void irq_default_handler_40();
    extern void irq_default_handler_41();
    extern void irq_default_handler_42();
    extern void irq_default_handler_43();
    extern void irq_default_handler_44();
    extern void irq_default_handler_45();
    extern void irq_default_handler_46();
    extern void irq_default_handler_47();
    idt_add_isr(32, idt_create_descriptor(irq_default_handler_32, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(33, idt_create_descriptor(irq_default_handler_33, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(34, idt_create_descriptor(irq_default_handler_34, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(35, idt_create_descriptor(irq_default_handler_35, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(36, idt_create_descriptor(irq_default_handler_36, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(37, idt_create_descriptor(irq_default_handler_37, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(38, idt_create_descriptor(irq_default_handler_38, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(39, idt_create_descriptor(irq_default_handler_39, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(40, idt_create_descriptor(irq_default_handler_40, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(41, idt_create_descriptor(irq_default_handler_41, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(42, idt_create_descriptor(irq_default_handler_42, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(43, idt_create_descriptor(irq_default_handler_43, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(44, idt_create_descriptor(irq_default_handler_44, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(45, idt_create_descriptor(irq_default_handler_45, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(46, idt_create_descriptor(irq_default_handler_46, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(47, idt_create_descriptor(irq_default_handler_47, 0, INTERRUPT_GATE, 0x08));
}
