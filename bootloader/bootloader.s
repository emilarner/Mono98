bits 16 ; output 16 bit machine code for now

section .boot
global bootloader
bootloader:
	; get the disk number at 0x7C02
	mov si, 0x7C02
	mov di, disk
	mov al, [si]
	mov [di], al

	mov ax, cs
	mov ds, ax
	mov ss, ax

	; set up a stack for C
	lea sp, [bootloader_stack_bottom + 2048]
	extern load
	call load

	; reset/initialize disk system
	mov ah, 0x00
    int 0x13

%ifndef CHS_LOAD
	mov ax, 0x08E0
	mov es, ax
	mov ah, 0x02     	; read sectors
   	mov al, 54      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
	mov ch, 0       	; cylinder idx
	mov dh, 0       	; head idx
	mov cl, 10       	; sector idx
	mov dl, [disk]  	; disk idx

	; the address to place data in is specified by es:bx, linear = es * 0x10 + bx
	; bx + sectors*512 cannot exceed 0xFFFF, hence multiple subsequent calls.
   	xor bx, bx
	int 0x13

	mov ax, 0x0FA0
	mov es, ax
	mov ah, 0x02     	; read sectors
   	mov al, 63      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
	mov ch, 0       	; cylinder idx
	mov dh, 1       	; head idx
	mov cl, 1       	; sector idx
	mov dl, [disk]  	; disk idx

	xor bx, bx
	int 0x13

	mov ax, 0x1780
	mov es, ax
	mov ah, 0x02     	; read sectors
   	mov al, 63      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
	mov ch, 0       	; cylinder idx
	mov dh, 2       	; head idx
	mov cl, 1       	; sector idx
	mov dl, [disk]  	; disk idx

	xor bx, bx
	int 0x13

	; 3rd time
	mov ax, 0x1F60
	mov es, ax
	mov ah, 0x02     	; read sectors
   	mov al, 24      	; read 22 sectors starting at sector 2, which means we start at sector 64 next
	mov ch, 0       	; cylinder idx
	mov dh, 2       	; head idx
	mov cl, 1       	; sector idx
	mov dl, [disk]  	; disk idx

	xor bx, bx
	int 0x13
%endif
%ifdef CHS_LOAD
%include "bootloader/chs_load.s"
%endif
	extern protected_mode
	call protected_mode

	; (I really want VESA mode, soon...)
    ; set BIOS VGA DRIVER in graphics mode (0xA0000)
	; as opposed to text mode (0xB8000)
	; this will still work in protected mode
	xor ax, ax
    mov ax, 0x13
	int 0x10

    mov al, 0xD1
    out 0x64, al
    mov al, 0xDF
    out 0x60, al

	cli ; disable interrupts before loading the gdt

	; enable the protected mode bit on the cr0 register
	; using eax as an intermediary

	mov eax, cr0
	or eax, 0x01 ; cr0 |= (1 << 0)
	mov cr0, eax

	lgdt [gdt_pointer]

	;mov ax, 0x10      ; Data segment selector (0x10 is the offset for your data segment in the GDT)
	;mov ds, ax        ; Set DS to point to the data segment
	;mov es, ax        ; Set ES to point to the data segment
	;mov ss, ax        ; Set SS to point to the data segment (you can choose a separate stack segment if you wish)
	;mov ax, 0x08      ; Code segment selector (0x08 is the offset for your code segment in the GDT)
	;mov cs, ax        ; Set CS to point to the code segment

	; we must now perform a long jump to flush or whatever
	; to the kernel main code and correctly set the new cs segment register.

	jmp 0x08:0x8E00

	

section .data
gdt_start:
	dq 0x0 ; the GDT must have a null segment, equal to 8 bytes
gdt_code:
	dw 0xFFFF ; 65536 limit with 4096 byte granularity = entire possible address space
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0
gdt_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start

global disk
disk: 
	db 0x00

bootloader_stack_bottom:
	times 2048 db 0x00
bootloader_stack_top:
