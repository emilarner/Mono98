; MEMORY ADDRESS 0x8E00
; MAIN ENTRY POINT OF KERNEL
section .text
bits 32

global entry
entry:
	; set up segment registers
	; this should be done IMMEDIATELY after performing the long jump
	; after loading the GDT
	; (we don't set up the segment selector for cs, because it was set by the long jump)

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov ebp, kernel_stack_bottom
    mov esp, kernel_stack_top
	extern kmain
	call kmain

	hlt

; some interrupt handlers 
global asm_interrupt_timer_handler
asm_interrupt_timer_handler:
	pusha
	extern interrupt_timer_handler
	call interrupt_timer_handler
	popa
	iret

global asm_interrupt_keyboard_handler
asm_interrupt_keyboard_handler:
	pusha
	extern interrupt_keyboard_handler
	call interrupt_keyboard_handler
	popa
	iret

global test_interrupts
test_interrupts:
	int 0x33
	ret

global asm_interrupt_syscall_handler
asm_interrupt_syscall_handler:
	pusha
	extern interrupt_syscall_handler
	call interrupt_syscall_handler
	popa
	iret


section .bss
align 4
kernel_stack_bottom: equ $
	resb 32768 ; 16 KB
kernel_stack_top:

section .rodata
global main_romdisk
main_romdisk: incbin "romdisk/ROMDISK"