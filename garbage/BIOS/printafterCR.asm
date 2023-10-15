[org 0x7C00] 

chars times 128 db 0        ; Define a buffer of 128 bytes to store input characters
mov si, chars               ; Set SI and DI to point to the beginning of the buffer
mov di, si

.wait_for_inputs: 
    mov ah, 0               ; Wait for a keypress using BIOS interrupt 0x16, func 0
    int 0x16

    cmp al, 0x0D            ; Check if the pressed key is Enter (carriage return)
    je .print_inputs        ; If Enter is pressed, jump to print_inputs
    mov [si], al            ; Store the pressed key in the buffer
    inc si                  ; Move to the next position in the buffer
    jmp .wait_for_inputs    ; Repeat the loop to wait for the next key

.print_inputs:
    mov al, [di]            ; Load a character from the buffer using DI into AL
    cmp al, 0               ; Check if the character is null (end of inputs)
    je .end                 ; If null, jump to the end
    mov ah, 0x0E            ; Print the character using BIOS interrupt 0x10, function 0x0E
    int 0x10

    inc di                  ; Move to the next character in the buffer
    jmp .print_inputs       ; Repeat the loop to print the next character

.end:
    mov si, chars           ; Reset SI and DI to point to the beginning of the buffer
    mov di, si
   
    mov ah, 0x0E            ; Print a new line by printing carriage return (13) and line feed (10)
    mov al, 13
    int 0x10
    mov al, 10
    int 0x10
    
    jmp .wait_for_inputs    ; Repeat the loop to wait for and print the next set of inputs


times 510-($-$$) db 0       ; Fill the rest of the boot sector with zeros (to make a total of 512 bytes)
db 0x55, 0xAA               ; Boot signature (last 2 bytes)