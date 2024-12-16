#ifndef ASM_BUS
#define ASM_BUS

#include "../header/standard_type.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif // ASM_BUS