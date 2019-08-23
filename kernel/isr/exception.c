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
        kprintf("\n");
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
    // Add exception handlers.
    extern void page_fault_handler(struct regs *r);
    exception_routine[14] = page_fault_handler;

    // Forgive me father, for I have sinned. Beyond this point, there is only
    // wrath and fear to be found. No one should have to witness this, ever.
    // The deadliest of sins 'code duplication' is unforgivable.
    // Forgive me father, for I have sinned...
    
    #define CREATE_DEFAULT_HANDLER(NR)                     \
        { extern void exception_default_handler_ ## NR();  \
          idt_add_isr(NR,  idt_create_descriptor(          \
                      exception_default_handler_ ## NR,    \
                      0, INTERRUPT_GATE, 0x08)); }
    CREATE_DEFAULT_HANDLER(0);
    CREATE_DEFAULT_HANDLER(1);
    CREATE_DEFAULT_HANDLER(2);
    CREATE_DEFAULT_HANDLER(3);
    CREATE_DEFAULT_HANDLER(4);
    CREATE_DEFAULT_HANDLER(5);
    CREATE_DEFAULT_HANDLER(6);
    CREATE_DEFAULT_HANDLER(7);
    CREATE_DEFAULT_HANDLER(8);
    CREATE_DEFAULT_HANDLER(9);
    CREATE_DEFAULT_HANDLER(10);
    CREATE_DEFAULT_HANDLER(11);
    CREATE_DEFAULT_HANDLER(12);
    CREATE_DEFAULT_HANDLER(13);
    CREATE_DEFAULT_HANDLER(14);
    CREATE_DEFAULT_HANDLER(15);
    CREATE_DEFAULT_HANDLER(16);
    CREATE_DEFAULT_HANDLER(17);
    CREATE_DEFAULT_HANDLER(18);
    CREATE_DEFAULT_HANDLER(19);
    CREATE_DEFAULT_HANDLER(20);
    CREATE_DEFAULT_HANDLER(21);
    CREATE_DEFAULT_HANDLER(22);
    CREATE_DEFAULT_HANDLER(23);
    CREATE_DEFAULT_HANDLER(24);
    CREATE_DEFAULT_HANDLER(25);
    CREATE_DEFAULT_HANDLER(26);
    CREATE_DEFAULT_HANDLER(27);
    CREATE_DEFAULT_HANDLER(28);
    CREATE_DEFAULT_HANDLER(29);
    CREATE_DEFAULT_HANDLER(30);
    CREATE_DEFAULT_HANDLER(31);
}
