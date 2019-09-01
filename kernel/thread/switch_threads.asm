; 
; Part of mOS (Minimalistic Operating System).
; This software is distributed under the MIT License.
; See the file LICENSE for details.
;
 
; C-prototype:
; struct thread *switch_threads(struct thread *cur, struct thread *next);
;
; Stack layout after register preservation in routine:
; OFFSET |        DATA
; ------------------------------
;    0   |         EDI
;    4   |         ESI
;    8   |         EBP
;   12   |         EBX
;   16   |    Return address
;   20   | struct thread *cur
;   24   | struct thread *next
;
; Switched from cur which must be the running thread, to next witch is also
; running switch_thread.

global switch_threads
switch_threads:
    ; Save the caller's registers state.
    push    ebx
    push    ebp
    push    esi
    push    edi

    ; Get offsetof(struct thread, stack) into edx.
    extern  thread_struct_stack_offset
    mov     edx, [thread_struct_stack_offset]

    ; Save current stack pointer to old thread's stack.
    mov     eax, dword[esp + 20]
    mov     dword[eax + edx], esp

    ; Restore stack pointer from new thread's stack.
    mov     ecx, [esp + 24]
    mov     esp, dword[ecx + edx]

    ; Restore caller's register state and return.
    pop     edi
    pop     esi
    pop     ebp
    pop     ebx
    ret


; Switch entry is used when initializing a new thread to make sure that the
; thread is setup in the correct manner to be switch to with switch_thread
global switch_entry
switch_entry:
    add     esp, 8
    push    eax
    extern  schedule_tail
    call    schedule_tail
    add     esp, 4
    ret
