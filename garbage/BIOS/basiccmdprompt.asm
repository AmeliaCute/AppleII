[org 0x7c00]

mov si, bootmessage     ; Set SI to point to the bootmessage

.boot_hello:
    mov al, [si]        ; Load the byte at the address pointed to by SI to AL (one char)
    cmp al, 0           ; Compare the char (AL) with null (end of string)
    je .new_line        ; IF the char is null, jump to .new_line
    mov ah, 0x0e        ; Print the char using BIOS interrupt 0x10, func 0x0E
    int 0x10            
    inc si              ; Move to the next char in the bootmessage (egs. current 'H' -> 'e')
    jmp .boot_hello      ; Repeat the loop print to print the next char

.print_text:    
    mov ah, 0           ; Wait for a keypress using BIOS interrupt 0x16, func 0
    int 0x16            
    cmp al, 0x0D        ; Compare the pressed key (in AL) with carraige return (ENTER key)
    je .new_line        ; If Enter key is pressed, jump to .new_line
    mov ah, 0x0E        ; Print the pressed key using BIOS interrupt 0x10, func 0x0E
    int 0x10
    jmp .print_text     ; Repeat the loop to wait for and print the next key

.new_line:
    mov ah, 0x0e        ; Print a new line bby printing carriage return (13) and line feed (10)
    mov al, 13
    int 0x10
    mov al, 10
    int 0x10
    jmp .print_text     ; Jump to print_text to wait for and print the next key

bootmessage db 'Hello from the bootloader...', 0

times 510-($-$$) db 0   ; Fille the rest of the boot sector with zeros (to make a total of 512 bytes)
db 0x55, 0xAA           ; Boot signature (last 2 bytes)