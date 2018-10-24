; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for details.
;


section .text

; C prototype:
; void load_gdt(const gdt_descriptor_t *ptr)
global load_gdt
load_gdt:
    mov     eax, [esp + 4]
    lgdt    [eax]
    jmp     0x08:flush

flush:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    ret

; C prototype:
; void load_idt(const idt_descriptor_t *ptr)
global load_idt
load_idt:
    mov     eax, [esp + 4]
    lidt    [eax]
    ret
