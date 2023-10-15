ORG 0x7c00
BITS 16

section .text

    xor ax, ax 
    mov ds, ax
    mov es, ax

    mov si, testmessage

    ; Loading kernel in the memory
    mov ax, 0x07e0
    mov es, ax
    mov bx, 0x0000
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    int 0x13

    ; Calling kernel
    call 0x07e0:0x0000
    
.loop:
    mov al, [si]
    cmp al, 0
    je .done
    mov ah, 0x0E
    int 0x10
    inc si
    jmp .loop

.done:
    jmp .start

.start:
    

.endless_loop:
    jmp .endless_loop

testmessage db 'Booting kernel from bootloader..', 0

times 510-($ - $$) db 0
dw 0xAA55