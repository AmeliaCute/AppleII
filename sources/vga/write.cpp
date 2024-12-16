#include "write.h"

char* buffer = (char*)0xb8000;

void print(const char* text)
{
    VgaComponent* vga_c = (VgaComponent*)k_component_get(1);
    int position = vga_c->cursor_position;

    if(!text) return;
    for(int i = 0; text[i] != 0x00; ++i)
        k_print_char(&position, vga_c->color, text[i]);

    update_cursor(position);

    vga_c->cursor_position = position;
}

void print(int* position, int color, const char* text)
{
    if(!text) return;
    for(int i = 0; text[i] != 0x00; ++i)
        k_print_char(position, color, text[i]);
}

void k_print_char(int* position, int color, char key)
{
    if (key == '\n') 
        *position = (*position / 160 + 1) * 160;

    else 
    {
        buffer[(*position)++] = key;
        buffer[(*position)++] = color;
    }

    if (*position >= 160 * 25) 
    {
        k_scroll_screen();
        *position = 160 * 24;
    }
}

void k_scroll_screen() 
{
    for (int row = 1; row < 25; ++row) 
        for (int col = 0; col < 160; ++col)
            buffer[(row - 1) * 160 + col] = buffer[row * 160 + col];
        
    

    // Clear the last row by filling it with spaces and the default color attribute.
    for (int col = 0; col < 160; col += 2) 
    {
        buffer[24 * 160 + col] = ' ';       
        buffer[24 * 160 + col + 1] = 0x0F;
    }
}