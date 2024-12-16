#include "keyboard.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

char read_keyboard_buffer()
{
    uint8_t status, scancode;
    bool shift = false;
    
    do 
        status = inb(KEYBOARD_STATUS); // update with shortcut head for better readability
    while(!(status & 1));

    scancode = inb(KEYBOARD_DATA);

    if (scancode & 0x80) {
        if (scancode == 0x2A || scancode == 0x36) 
            shift = false;
        return 0;
    }

    if (scancode == 0x2A || scancode == 0x36) 
    { 
        shift = true;
        return 0;
    }

    int offset = (shift ? 128 : 0);
    return (scancode < 128) ? codeascii_us[scancode + offset] : 0;  
}