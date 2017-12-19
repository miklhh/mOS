; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for detailes.
;
section .text

; -- outb --
; void outb(uint16_t port, uint8_t value);
global outb
outb:
    mov     dx,  word[esp + 4]  ; Retrive the port.
    mov     al,  byte[esp + 8]  ; Retrive the value.
    out     dx,  al             ; Preform the output.
    ret

; -- inb --
; uint8_t inb(uint16_t port);
global inb
inb:
    mov     dx, word[esp + 4]   ; Retrive the port.
    in      al, dx              ; Preform the input.
    ret                         ; Return value returned in al.
