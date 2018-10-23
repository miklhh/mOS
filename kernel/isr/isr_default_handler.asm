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
    iret

section .rodata
default_string:
    db  'Hello ISR world',0
