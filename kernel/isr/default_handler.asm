; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for details.
;

section .text
global isr_default_handler
isr_default_handler:
    ; Just print the message and return.
    extern  kprintf
    push    default_string
    call    kprintf
    add     esp, 0x04
    iretd

globale exception_default_handler
exception_default_handler:
    

section .rodata
default_string:
    db  'Default interrupt service routine called.',0x0A,0x00
