[org 0x7c00]

mov bp, 0x8000
mov sp, bp

mov bh, 'A'
push bx

mov bh, 'B'
mov ah, 0x0e
mov al, bh
int 0x10

pop bx
mov al, bh
int 0x10

times 510-($-$$) db 0   ; Fille the rest of the boot sector with zeros (to make a total of 512 bytes)
db 0x55, 0xAA           ; Boot signature (last 2 bytes)