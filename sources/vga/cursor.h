#ifndef VGA_CURSOR
#define VGA_CURSOR

#include "../manipulator/asm_bus.h"
#include "../header/standard_type.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int offset);

#endif // !VGA_CURSOR