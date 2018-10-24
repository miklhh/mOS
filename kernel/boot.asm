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

; This section contains the page directory of the kernel. The table contains two
; active 4-MB pages, one for identity mapping the first 4 MB and one for mapping 
; (virtual) 0xC0000000 to (physical) 0x00000000 (4 MB). This table will only be
; used when setting up the system.
KERNEL_VIRTUAL_BASE     equ 0xC0000000
KERNEL_PAGE_NUMBER      equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
align 0x1000
boot_page_directory:
    dd      0x00000083
    times   (KERNEL_PAGE_NUMBER - 1) dd 0
    dd      0x00000083
    times   (1024 - KERNEL_PAGE_NUMBER - 3) dd 0


section .text
global loader_phys
loader_phys equ (loader_virt - KERNEL_VIRTUAL_BASE)

loader_virt:
    ; Load the page directory. Adjust for physical addresses.
    mov     ecx, (boot_page_directory - KERNEL_VIRTUAL_BASE)
    mov     cr3, ecx            ; Load page directory (adjusted).
    mov     ecx, cr4            ; Fetch CR4.
    or      ecx, 0x00000010     ; Set 'PSE' bit CR4.
    mov     cr4, ecx            ; Store store result in CR4.
    mov     ecx, cr0            ; Fetch CR0.
    or      ecx, 0x80000000     ; Set 'Page enalbe' bit.
    mov     cr0, ecx            ; Enable paging.

    ; Paging is enable beyond this point. Jump to kernel entry point.
    lea     ecx, [_start]
    jmp     ecx

; Start symbol. This is the assembly entrypoint of the kernel. The previous
; section ... should preserve the state of GRUB loading.
global _start
_start:
    mov     dword[boot_page_directory], 0
    invlpg  [0]                     ; Invalidate the identity mapping of first 4 MB.

    cli                             ; Clear interrupt flag.
    add     ebx, KERNEL_VIRTUAL_BASE; Adjust for virtual address.
    mov     esp, stack_top          ; Setup the kernel stack.
    push    esp                     ; kmain parameter 4: 'uintptr_t stack_top'.
    push    stack_bottom            ; kmain parameter 3: 'uintptr_t stack_bottom'. 
    push    eax                     ; kmain parameter 2: 'uint32_t multiboot_magic'.
    push    ebx                     ; kmail parameter 1: 'struct multiboot_info *info'.
    extern  kmain                   ; Prepare to invoke kernel main function.
    call    kmain                   ; Invoke kernel main function.

hang:
    cli
    hlt
    jmp hang

; Halt and shutdown routine. This routine will disable interrupts and spin
; forever in order to freeze the system.
global halt_and_shutdown
halt_and_shutdown:
    cli
    hlt
    jmp     halt_and_shutdown
