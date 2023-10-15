[org 0x7c00]                                    
KERNEL_LOCATION equ 0x1000                      ; Define a constant for the kernel location
mov [BOOT_DISK], dl                             ; Store the value of register DL in memory at BOOT_DISK

xor ax, ax                                      ; Clear AX register
mov es, ax                                      ; Set ES register to 0
mov ds, ax                                      ; Set DS register to 0
mov bp, 0x8000                                  ; Set BP register to 0x8000
mov sp, bp                                      ; Set SP register to the value in BP

mov bx, KERNEL_LOCATION                         ; Load the kernel location into BX
mov dh, 20                                      ; Set the number of sectors to read (head)

mov ah, 0x02                                    ; BIOS disk read function
mov al, dh                                      ; Numbers of sectors to read
mov ch, 0x00                                    ; Cylinder number
mov dh, 0x00                                    ; Header number
mov cl, 0x02                                    ; Sector number (starting from 1)
mov dl, [BOOT_DISK]                             ; Load the boot disk number into DL
int 0x13                                        ; BIOS disk I/O interrupt
jc boot_failed                                  ; Jump to boot_failed if carry flag is set

mov ah, 0x0                                     ; Set AH to 0
mov al, 0x3                                     ; Set AL to 3 (clear screen)
int 0x10                                        ; BIOS video interrupt

CODE_SEG equ GDT_code - GDT_start               ; Calculate segment selector for code and data segments in the Global Descriptor Table (GDT)
DATA_SEG equ GDT_data - GDT_start

cli                                             ; Disable interrupts
lgdt [GDT_descriptor]                           ; Load the Global Descriptor Table (GDT) descriptor
mov eax, cr0                                    ; Modify the control register CR0 to enable protected mode, load control register CR0 to EAX
or eax, 1                                       ; Set the first bit (PE - Protection Enable) to 1
mov cr0, eax                                    ; Update control register CR0 with de modified value
jmp CODE_SEG:start_protected                    ; high jump to the start of the code segment in protected mode

boot_failed:
    mov ah, 0x0e                                ; Set AH to 0x0e (teletype output)
    mov al, 0x21                                ; Set AL to 0x21 (ASCII value for '!')
    int 0x10                                    ; BIOS video interrupt
    jmp boot_failed                             ; loop

jmp $

BOOT_DISK: db 0                                 ; Define a byte to store the boot disk number

GDT_start:                                      ; Start defining the Global Descriptor Table (GDT)
    GDT_null:                                   ; Null descriptor (Required for GDT format)
        dd 0x0                                  ; just 00000000
        dd 0x0                                  ; just 00000000
    GDT_code:                                   ; Code segment descriptor
        dw 0xffff                               ; limit 
        dw 0x0                                  ; 16 bits
        db 0x0                                  ; 8 bits
        db 0b10011010                           ; pres, priv, type = 1001 and type flags = 1010
        db 0b11001111                           ; others flag = 1100 + last 4 bits of the limit
        db 0x0                                  ; last 8 bits of the base
    GDT_data:                                   ; Data segment descriptor
        dw 0xffff                               ; limit 
        dw 0x0                                  ; 16 bits
        db 0x0                                  ; 8 bits
        db 0b10010010                           ; pres, priv, type = 1001 and type flags = 0010
        db 0b11001111                           ; others flag = 1100 + last 4 bits of the limit
        db 0x0                                  ; last 8 bits of the base

GDT_end:                                        ; End of GDT definition

GDT_descriptor:
    dw GDT_end - GDT_start - 1                  ; Length of the GDT (in bytes)
    dd GDT_start                                ; Adress of the start of the GDT


[bits 32]                                       ; Switching to 32bits mode
start_protected:       
    mov ax, DATA_SEG                            ; Load DATA_SEG into AX
        mov ds, ax                              ; Set DS register to AX
        mov ss, ax                              ; Set SS register to AX
        mov es, ax                              ; Set ES register to AX
        mov fs, ax                              ; Set FS register to AX
        mov gs, ax                              ; Set GS register to AX

        mov ebp, 0x90000                        ; Set EBP register
        mov esp, ebp                            ; Set ESP register

    jmp KERNEL_LOCATION                         ; Jump to KERNEL_LOCATION (run void main(void))

times 510-($-$$) db 0                           ; Fill the rest of the boot sector with zeros (to make a total of 512 bytes)
db 0x55, 0xAA                                   ; Boot signature (last 2 bytes)