// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <system.h>
#include <kstdio.h>

void page_fault_handler(struct regs *r)
{
    unsigned page_fault_linear_address;
    asm volatile ("mov %%cr2, %0" : "=r"(page_fault_linear_address));

    kprintf("\n");
    kprintf("--------------------------------------------------------------------------------");
    kprintf("    Unhandeled exception: [%d] '%s'.\n", r->int_no, "Page fault");
    kprintf("    The system cannot recover from an unhandeled exception. Halting machine.\n\n");
    kprintf("    EAX: 0x%08x   EBX: 0x%08x   ECX: 0x%08x   EDX: 0x%08x\n", r->eax, r->ebx, r->ecx, r->edx);
    kprintf("    EDI: 0x%08x   ESI: 0x%08x   EBP: 0x%08x   ESP: 0x%08x\n", r->edi, r->esi, r->ebp, r->esp);
    kprintf("    DS:  0x%08x   ES:  0x%08x   FS:  0x%08x   GS:  0x%08x\n", r->ds, r->es, r->fs, r->gs);
    kprintf("    EIP: 0x%08x   CS:  0x%08x   SS:  0x%08x   UESP:0x%08x\n", r->eip, r->cs, r->ss, r->useresp);
    kprintf("    Error code: 0x%x\n    Interrupt number: %d\n", r->err_code, r->int_no);
    kprintf("    Page fault linear address: 0x%08x\n", page_fault_linear_address);
    kprintf("--------------------------------------------------------------------------------");
    halt_and_shutdown();
}
