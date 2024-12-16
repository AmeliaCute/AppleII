#include "cursor.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(int offset)
{
	offset *= .5;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (offset & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((offset >> 8) & 0xFF));
}