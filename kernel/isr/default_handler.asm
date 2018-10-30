; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for details.
;


; --- ISR default handler.
section .text
global isr_default_handler
isr_default_handler:
    ; Just print the message and return.
    extern  kprintf
    push    default_string
    call    kprintf
    add     esp, 0x04
    iretd

section .rodata
default_string:
    db  'Default interrupt service routine called.',0x0A,0x00


; --- Exception default handler.
section .text
%macro EXCEPTION_ISR_NOERR 1
    global exception_default_handler_%1
    exception_default_handler_%1:
        cli
        push    0x00
        push    %1
        jmp     exception_common
%endmacro

%macro EXCEPTION_ISR_ERR 1
    global exception_default_handler_%1
    exception_default_handler_%1:
        cli
        push    %1
        jmp     exception_common
%endmacro

exception_common:
    pusha                       ; Push all general purpose registers.
    push    ds
    push    es                  ;
    push    fs                  ; Preserve segment registers.
    push    gs                  ;
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax              ;
    mov     fs, ax              ; Kernel data segment when handling exception.
    mov     gs, ax              ;
    cld

    push    esp                 ; Push the 'struct regs *' argument being used.
    extern  exception_handler
    call    exception_handler   ; Call the high level exception handler.
    add     esp, 4              ; Remove 'struct regs *' argument from stack.

    pop     gs                  ;
    pop     fs                  ; Restore segment register.
    pop     es                  ;
    pop     ds

    popa                        ; Cleanup all registers.
    add     esp, 8              ; Cleanup error code and handler number.
    iret                        ; Return from exception.

; All the exception low level handlers.
EXCEPTION_ISR_NOERR 0
EXCEPTION_ISR_NOERR 1
EXCEPTION_ISR_NOERR 2
EXCEPTION_ISR_NOERR 3
EXCEPTION_ISR_NOERR 4
EXCEPTION_ISR_NOERR 5
EXCEPTION_ISR_NOERR 6
EXCEPTION_ISR_NOERR 7
EXCEPTION_ISR_ERR   8
EXCEPTION_ISR_NOERR 9
EXCEPTION_ISR_ERR   10
EXCEPTION_ISR_ERR   11
EXCEPTION_ISR_ERR   12
EXCEPTION_ISR_ERR   13
EXCEPTION_ISR_ERR   14
EXCEPTION_ISR_NOERR 15
EXCEPTION_ISR_NOERR 16
EXCEPTION_ISR_ERR   17
EXCEPTION_ISR_NOERR 18
EXCEPTION_ISR_NOERR 19
EXCEPTION_ISR_NOERR 20
EXCEPTION_ISR_NOERR 21
EXCEPTION_ISR_NOERR 22
EXCEPTION_ISR_NOERR 23
EXCEPTION_ISR_NOERR 24
EXCEPTION_ISR_NOERR 25
EXCEPTION_ISR_NOERR 26
EXCEPTION_ISR_NOERR 27
EXCEPTION_ISR_NOERR 28
EXCEPTION_ISR_NOERR 29
EXCEPTION_ISR_ERR   30
EXCEPTION_ISR_NOERR 31


; --- IRQ default handler.
section .text
%macro IRQ_ISR_NOERR 1
    global irq_default_handler_%1
    irq_default_handler_%1:
        cli
        push    0x00
        push    %1
        jmp     irq_common
%endmacro

; All the IRQ low level handlers
IRQ_ISR_NOERR 32
IRQ_ISR_NOERR 33
IRQ_ISR_NOERR 34
IRQ_ISR_NOERR 35
IRQ_ISR_NOERR 36
IRQ_ISR_NOERR 37
IRQ_ISR_NOERR 38
IRQ_ISR_NOERR 39
IRQ_ISR_NOERR 40
IRQ_ISR_NOERR 41
IRQ_ISR_NOERR 42
IRQ_ISR_NOERR 43
IRQ_ISR_NOERR 44
IRQ_ISR_NOERR 45
IRQ_ISR_NOERR 46
IRQ_ISR_NOERR 47

irq_common:
    pusha                       ; Push all general purpose registers.
    push    ds
    push    es                  ;
    push    fs                  ; Preserve segment registers.
    push    gs                  ;
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax              ;
    mov     fs, ax              ; Kernel data segment when handling exception.
    mov     gs, ax              ;
    cld

    push    esp                 ; Push the 'struct regs *' argument being used.
    extern  irq_handler
    call    irq_handler         ; Call the high level exception handler.
    add     esp, 4              ; Remove 'struct regs *' argument from stack.

    pop     gs                  ;
    pop     fs                  ; Restore segment register.
    pop     es                  ;
    pop     ds

    popa                        ; Cleanup all registers.
    add     esp, 8              ; Cleanup error code and handler number.
    iret                        ; Return from exception.
