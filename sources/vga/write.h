#ifndef VGA_WRITE
#define VGA_WRITE

#include "../kernel/component_manager.h"
#include "cursor.h"

void k_print_char(int* position, int color, const char key);
void print(int* position, int color, const char* text);
void print(const char* text);
void clear_screen();
void k_scroll_screen();

#endif // !VGA