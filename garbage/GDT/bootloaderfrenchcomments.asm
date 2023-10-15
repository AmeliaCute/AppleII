[org 0x7C00]                        ; Définit l'origine du programme à l'adresse mémoire 0x7C00

mov [BOOT_DISK], dl                 ; Stocke la valeur du registre DL dans la mémoire à l'emplacement BOOT_DISK
                                    ; Calcule les sélecteurs de segment pour les segments de code et de données dans la table de descripteurs globaux (GDT)
CODE_SEG equ GDT_code - GDT_start   ; Calcule le sélecteur de segment pour le segment de code
DATA_SEG equ GDT_data - GDT_start   ; Calcule le sélecteur de segment pour le segment de données

cli                                 ; Désactive les interruptions
lgdt [GDT_descriptor]               ; Charge le descripteur de la table de descripteurs globaux (GDT)
mov eax, cr0                        ; Modifie le registre de contrôle CR0 pour activer le mode protégé, Charge le registre de contrôle CR0 dans EAX
or eax, 1                           ; Positionne le premier bit (PE - Protection Enable) à 1
mov cr0, eax                        ; Met à jour le registre de contrôle CR0 avec la valeur modifiée
jmp CODE_SEG:start_protected        ; Saut vers le début du segment de code en mode protégé

GDT_start:                          ; Débute la définition de la table de descripteurs globaux (GDT)
    GDT_null:                       ; Descripteur nul (nécessaire pour le format GDT)
        dd 0x0                      ; Mot double 32 bits initialisé à 0x0
        dd 0x0                      ; Mot double 32 bits initialisé à 0x0
    GDT_code:                       ; Descripteur de segment de code
        dw 0xffff                   ; Limite du segment
        dw 0x0                      ; 16 bits
        db 0x0                      ; 8 bits
        db 0b10011010               ; Présence, privilège, type = 1001 et indicateurs de type = 1010
        db 0b11001111               ; Autres indicateurs = 1100 + derniers 4 bits de la limite
        db 0x0                      ; Derniers 8 bits de la base
    GDT_data:                       ; Descripteur de segment de données
        dw 0xffff                   ; Limite du segment
        dw 0x0                      ; 16 bits
        db 0x0                      ; 8 bits
        db 0b10010010               ; Présence, privilège, type = 1001 et indicateurs de type = 1010
        db 0b11001111               ; Autres indicateurs = 1100 + derniers 4 bits de la limite
        db 0x0                      ; Derniers 8 bits de la base

GDT_end:                            ; Fin de la définition de la GDT

GDT_descriptor:
    dw GDT_end - GDT_start - 1      ; Longueur de la GDT (en octets)
    dd GDT_start                    ; Adresse du début de la GDT


[bits 32]                           ; Passage en mode 32 bits
start_protected:
    mov al, 'A'                     ; Affiche 'A' dans le coin supérieur gauche de la mémoire vidéo VGA
    mov ah, 0x0f
    mov [0xb8000], ax               ; Écrit dans la mémoire vidéo VGA pour afficher 'A'

BOOT_DISK: db 0                     ; Définit un octet pour stocker le numéro du disque de démarrage

times 510-($-$$) db 0               ; Remplit le reste du secteur de démarrage avec des zéros (pour obtenir un total de 512 octets)
db 0x55, 0xAA                       ; Signature de démarrage (derniers 2 octets)
