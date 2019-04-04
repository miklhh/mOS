; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for details.
;


MBALIGN     equ 1 << 0
MEMINFO     equ 1 << 1
FLAGS       equ MBALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .bss
align 4
stack_bottom:
resb 32768
stack_top:

; This section (USED TO) contain the page directory of the kernel. The table 
; contains two active 4-MB pages, one for identity mapping the first 4 MB and 
; one for mapping (virtual) 0xC0000000 to (physical) 0x00000000 (4 MB). This 
; table will only be used when setting up the system.
KERNEL_VIRTUAL_BASE     equ 0x00000000
KERNEL_PAGE_NUMBER      equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
align 0x1000
; Possible data goes here...


section .text
global _loader
_loader:
    lea     ecx, [_start]             ; Paging used to be here. Oh well.
    jmp     ecx                       ; Jump to the kernel entry.


global _start
_start:
    cli                               ; Clear interrupt flag.
    add     ebx, KERNEL_VIRTUAL_BASE  ; Adjust for virtual address.
    mov     esp, stack_top            ; Setup the kernel stack.
    push    esp                       ; kmain parameter 4: 'uintptr_t stack_top'.
    push    stack_bottom              ; kmain parameter 3: 'uintptr_t stack_bottom'. 
    push    eax                       ; kmain parameter 2: 'uint32_t multiboot_magic'.
    push    ebx                       ; kmail parameter 1: 'struct multiboot_info *info'.
    extern  kmain                     ; Prepare to invoke kernel main function.
    call    kmain                     ; Invoke kernel main function.

hang:
    cli
    hlt
    jmp hang


; Halt and shutdown routine. This routine will disable interrupts and spin
; forever in order to freeze the system.
global halt_and_shutdown
halt_and_shutdown:
    cli                               ; In case of HaS, clear interrupt flag...
    hlt                               ; and halt the machine.
    jmp     halt_and_shutdown         ; Jump back in case of NMI.
