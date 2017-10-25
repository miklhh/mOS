;
; Part of mOS (Minimalistic Operating System)
; Code may freely be used under the MIT-license.
;

section .text

; -- outb --
; void outb(uint16_t port, uint8_t value)
global outb
outb:
    mov     dx,  word[esp+4]    ; Retrive the port.
    mov     al,  byte[esp+8]    ; Retrive the value.
    out     dx,  al             ; Preform the output.
    ret
