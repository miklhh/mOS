// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <kstdio.h>
#include <assert.h>
#include <stdint.h>
#include <system.h>
#include <idt.h>

#define EXCEPTION_COUNT     32
#define NULL                0

// The 32 exception types.
static const char *exception_name[EXCEPTION_COUNT] = {
    "Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static void (*exception_routine[EXCEPTION_COUNT])(struct regs *r) = { 0 };

// The exception handler.
void exception_handler(struct regs *r)
{
    if ( (uintptr_t)exception_routine[r->int_no] != NULL )
    {
        // The exception routine exists, call it!
        exception_routine[r->int_no](r);
    }
    else
    {
        // The exception does not exist. No going back from here...
        kprintf("--------------------------------------------------------------------------------");
        kprintf("    Unhandeled exception: [%d] '%s'.\n", r->int_no, exception_name[r->int_no]);
        kprintf("    The system cannot recover from an unhandeled exception. Halting machine.\n\n");
        kprintf("    EAX: 0x%08x   EBX: 0x%08x   ECX: 0x%08x   EDX: 0x%08x\n", r->eax, r->ebx, r->ecx, r->edx);
        kprintf("    EDI: 0x%08x   ESI: 0x%08x   EBP: 0x%08x   ESP: 0x%08x\n", r->edi, r->esi, r->ebp, r->esp);
        kprintf("    DS:  0x%08x   ES:  0x%08x   FS:  0x%08x   GS:  0x%08x\n", r->ds, r->es, r->fs, r->gs);
        kprintf("    EIP: 0x%08x   CS:  0x%08x   SS:  0x%08x   UESP:0x%08x\n", r->eip, r->cs, r->ss, r->useresp);
        kprintf("    Error code: 0x%x\n    Interrupt number: %d\n", r->err_code, r->int_no);
        kprintf("--------------------------------------------------------------------------------");
        halt_and_shutdown();
    }
}

void system_init_exceptions()
{
    // Forgive me father, for I have sinned. Beyond this point, there is only
    // wrath and fear to be found. No one should have to witness this, ever.
    // The deadliest of sins 'code duplication' is unforgivable.
    // Forgive me father, for I have sinned...
    extern void exception_default_handler_0();
    extern void exception_default_handler_1();
    extern void exception_default_handler_2();
    extern void exception_default_handler_3();
    extern void exception_default_handler_4();
    extern void exception_default_handler_5();
    extern void exception_default_handler_6();
    extern void exception_default_handler_7();
    extern void exception_default_handler_8();
    extern void exception_default_handler_9();
    extern void exception_default_handler_10();
    extern void exception_default_handler_11();
    extern void exception_default_handler_12();
    extern void exception_default_handler_13();
    extern void exception_default_handler_14();
    extern void exception_default_handler_15();
    extern void exception_default_handler_16();
    extern void exception_default_handler_17();
    extern void exception_default_handler_18();
    extern void exception_default_handler_19();
    extern void exception_default_handler_20();
    extern void exception_default_handler_21();
    extern void exception_default_handler_22();
    extern void exception_default_handler_23();
    extern void exception_default_handler_24();
    extern void exception_default_handler_25();
    extern void exception_default_handler_26();
    extern void exception_default_handler_27();
    extern void exception_default_handler_28();
    extern void exception_default_handler_29();
    extern void exception_default_handler_30();
    extern void exception_default_handler_31();
    idt_add_isr(0,  idt_create_descriptor(exception_default_handler_0,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(1,  idt_create_descriptor(exception_default_handler_1,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(2,  idt_create_descriptor(exception_default_handler_2,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(3,  idt_create_descriptor(exception_default_handler_3,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(4,  idt_create_descriptor(exception_default_handler_4,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(5,  idt_create_descriptor(exception_default_handler_5,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(6,  idt_create_descriptor(exception_default_handler_6,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(7,  idt_create_descriptor(exception_default_handler_7,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(8,  idt_create_descriptor(exception_default_handler_8,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(9,  idt_create_descriptor(exception_default_handler_9,   0, INTERRUPT_GATE, 0x08));
    idt_add_isr(10, idt_create_descriptor(exception_default_handler_10,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(11, idt_create_descriptor(exception_default_handler_11,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(12, idt_create_descriptor(exception_default_handler_12,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(13, idt_create_descriptor(exception_default_handler_13,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(14, idt_create_descriptor(exception_default_handler_14,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(15, idt_create_descriptor(exception_default_handler_15,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(16, idt_create_descriptor(exception_default_handler_16,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(17, idt_create_descriptor(exception_default_handler_17,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(18, idt_create_descriptor(exception_default_handler_18,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(19, idt_create_descriptor(exception_default_handler_19,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(20, idt_create_descriptor(exception_default_handler_20,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(21, idt_create_descriptor(exception_default_handler_21,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(22, idt_create_descriptor(exception_default_handler_22,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(23, idt_create_descriptor(exception_default_handler_23,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(24, idt_create_descriptor(exception_default_handler_24,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(25, idt_create_descriptor(exception_default_handler_25,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(26, idt_create_descriptor(exception_default_handler_26,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(27, idt_create_descriptor(exception_default_handler_27,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(28, idt_create_descriptor(exception_default_handler_28,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(29, idt_create_descriptor(exception_default_handler_29,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(30, idt_create_descriptor(exception_default_handler_30,  0, INTERRUPT_GATE, 0x08));
    idt_add_isr(31, idt_create_descriptor(exception_default_handler_31,  0, INTERRUPT_GATE, 0x08));
}
