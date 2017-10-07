MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
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


section .text
global _start
_start:
    cli                         ; Clear interrupt flag.
    mov     esp, stack_top      ; Setup the kernel stack.
    push    esp                 ; kmain parameter 3: 'uintptr_t stack_bottom'.
    push    eax                 ; kmain parameter 2: 'uint32_t multiboot_magic'.
    push    ebx                 ; kmail parameter 1: 'struct multiboot_info info'.
    extern  kmain               ; Prepare to invode kernel main function.
    call    kmain               ; Invoke kernel main function.

hang:
    cli
    hlt
    jmp hang
