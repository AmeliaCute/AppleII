#ifndef port_keyboard
#define port_keyboard 

#include "../manipulator/asm_bus.h"
#include "../header/standard_type.h"

static const char codeascii_us[256] = {
    0, 27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  // 0x00 - 0x0F
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // 0x10 - 0x1F
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 0x20 - 0x2C
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // 0x2D - 0x39
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // 0x3A - 0x4F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x50 - 0x5F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x60 - 0x6F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x70 - 0x7F
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  // 0x80 - 0x8F (Shifted)
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',  // 0x90 - 0x9C (Shifted)
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, // 0xA0 - 0xAF (Shifted)
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // 0xB0 - 0xBF (Shifted)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xC0 - 0xCF (Unused/No Operation)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xD0 - 0xDF (Unused/No Operation)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xE0 - 0xEF (Unused/No Operation)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xF0 - 0xFF (Unused/No Operation)
};



char read_keyboard_buffer();

#endif //! port_keyboard