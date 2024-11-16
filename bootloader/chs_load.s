; floppy = 2880 logical sectors
; sector = (logical_sector % 18) + 1
; head = (logical_sector / 18) % 2
; cylinder = (logical_sector / 36)
; we need to read logical sectors 2 - 2880 inclusive
; 18 sectors per track, 

; 9th sector
mov ax, 0x08E0
mov es, ax
mov ah, 0x02     	; read sectors
mov al, 9      	    ; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 0       	; cylinder idx
mov dh, 0       	; head idx
mov cl, 10       	; sector idx
mov dl, [disk]  	; disk idx

xor bx, bx
int 0x13

; 18th sector
mov ax, 0x0A00
mov es, ax
mov ah, 0x02        ; read sectors
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 0       	; cylinder idx
mov dh, 1       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 36th sector
mov ax, 0x0C40
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 1       	; cylinder idx
mov dh, 0       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 54th sector - 27648 bytes
mov ax, 0x0E80
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 1       	; cylinder idx
mov dh, 1       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 72nd sector - 36864 bytes
mov ax, 0x10c0
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 2       	; cylinder idx
mov dh, 0       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 90th sector - 46080 bytes
mov ax, 0x1300
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 2       	; cylinder idx
mov dh, 1       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 108th sector - 55296 bytes
mov ax, 0x1540
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 3       	; cylinder idx
mov dh, 0       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 126th sector - 64512 bytes
mov ax, 0x1780
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 3       	; cylinder idx
mov dh, 1       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 144th sector - 73728 bytes
mov ax, 0x19c0
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 4       	; cylinder idx
mov dh, 0       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13

; 162nd sector - 82944 bytes
mov ax, 0x1c00
mov es, ax
mov ah, 0x02
mov al, 18      	; read 62 sectors starting at sector 2, which means we start at sector 64 next
mov ch, 4       	; cylinder idx
mov dh, 1       	; head idx
mov cl, 1       	; sector idx
mov dl, [disk]  	; disk idx
xor bx, bx
int 0x13
