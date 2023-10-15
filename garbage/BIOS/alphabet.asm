
mov ah, 0x0e
mov al, 'A'
int 0x10

.loop:
    inc al
    int 0x10
    cmp al, 90 ; equivalent of 'Z' in decimal
    jl .loop

jmp $

times 510-($-$$) db 0
db 0x55, 0xaa