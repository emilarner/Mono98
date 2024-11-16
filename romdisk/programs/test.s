; programs are injected at address 0x333999, but the first three bytes are not executable code.
org 0x333999

; we're in protected mode, so... 32 bit code.
bits 32 

; a valid executable for this kernel must start with the bytes 0x50, 0x5C, 0xDE
magic:
    db 0x50
    db 0x5C
    db 0xDE

main:
    ; the system call handler, a jump table essentially, is a C function which uses the cdecl calling convention
    ; arguments are put onto the stack (the last arguments pushed first)
    ; uint32_t syscall(uint8_t, void*, void*, void*, void*)
    push 0x00000000     ; 4 bytes
    push 0xFFFFFFFF     ; 4 bytes 
    push str            ; 4 bytes
    push 0x00000001     ; 4 bytes 
    push 0x02           ; 1 byte
    push return

    ; the system call handler address is set in address 0x333995
    mov eax, [0x333995]
    jmp eax
return:

    ; clean up the stack, so ret knows where to jump back to
    add esp, 20
    ret

str:
    db "hello, world from asm", 0x0A, 0x00
