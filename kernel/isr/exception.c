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

void system_init_exceptions()
{
    extern void exception_default_handler_0(); // Divide by zero error.
    extern void exception_default_handler_1(); // Divide by zero error.
    extern void exception_default_handler_2(); // Divide by zero error.
    extern void exception_default_handler_9(); // Double fault.
    idt_add_isr(0, idt_create_descriptor(exception_default_handler_0, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(1, idt_create_descriptor(exception_default_handler_1, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(2, idt_create_descriptor(exception_default_handler_2, 0, INTERRUPT_GATE, 0x08));
    idt_add_isr(9, idt_create_descriptor(exception_default_handler_9, 0, INTERRUPT_GATE, 0x08));

    // Pseducode.
    char symbol_name[30];
    for (int i = 0; i < EXCEPTION_COUNT; ++i)
    {
        ksprintf(symbol_name, "exception_default_handler_%d", i);
        //void (*handler)() = symbol_find(symbol_name);
    }
}

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
        kprintf("    The system cannot recover from an unhandeled exception. Halting machine.\n");
        kprintf("--------------------------------------------------------------------------------");
        halt_and_shutdown();
    }
}
