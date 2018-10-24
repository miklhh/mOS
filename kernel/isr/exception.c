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

// The low level exception handler.
extern void exception_default_handler();

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

static void (*exception_routine[EXCEPTION_COUNT])(uint32_t error_code) = { 0 };

void system_init_exceptions()
{
    for (int i = 0; i < EXCEPTION_COUNT; ++i)
    {
        idt_add_isr(i, idt_create_descriptor(exception_default_handler, 0, INTERRUPT_GATE, 0x08));
    }
}

// The exception handler.
void exception_handler(int vector, uint32_t error_code)
{
    // Test if there is a routine for the exception.
    assert(vector < 32);
    if ( (uintptr_t)exception_routine[vector] != NULL )
    {
        // The exception routine exists, call it!
        exception_routine[vector](error_code);
    }
    else
    {
        // The exception does not exist. No going back from here...
        kprintf("Unhandeled exception: [%d] %s.\n", vector, exception_name[vector]);
        kprintf("This is a fatal error and the system will halt.");
        halt_and_shutdown();
    }
}
