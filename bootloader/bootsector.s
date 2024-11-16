org 0x7C00
bits 16
jmp bootsector
disk:
	db 0xFF

disc_info:
	times 64 db 0x00

bootsector:
	; if you're wondering why your bootloader/kernel doesn't work on real hardware,
	; this is why: you need to clear the segment registers
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

	mov [disk], dl

    ; enable the A20 to access more than 1MB of memory (useful for protected mode!)
	; (this method is not guaranteed to work on all systems, but it works on all of mine)
	mov ax, 0x2401
	int 0x15

    ; enable VGA text mode for bootloader output.
	xor ax, ax
    mov ax, 0x03
	int 0x10

	mov ah, 0x02     	; read sectors
    mov al, 8      	    ; read 8 sectors for the rest of the bootloader.
	mov ch, 0       	; cylinder idx
	mov dh, 0       	; head idx
    mov cl, 2       	; sector idx

    xor bx, bx
    mov es, bx
	mov bx, 0x7E00
	int 0x13

	; jump to the 2nd stage of the bootloader. 
	jmp 0x7E00


times 510 - ($-$$) db 0x00
db 0x55
db 0xAA