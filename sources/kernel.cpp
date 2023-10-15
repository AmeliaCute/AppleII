int CURSOR_POSITION = 0;
int COLOR = 0x0F;

void print(char* buffer,const char* text)
{
    for(int i = 0; text[i] != 0x00; ++i)
    {
        if (text[i] == '\n') CURSOR_POSITION = (CURSOR_POSITION / 160 + 1) * 160;
        else 
        {
            buffer[CURSOR_POSITION] = text[i];
            buffer[CURSOR_POSITION + 1] = COLOR;
            CURSOR_POSITION += 2;
        }
    }
}

extern "C" void main()
{
    char* vga_buffer = (char*)0xb8000;

    COLOR = 0x0C;
    print(vga_buffer, "\n ..\n\n");
    COLOR = 0x0F;
    print(vga_buffer, "idk..\nbut this is the first time i code a bootloader ");
    COLOR = 0x0D;
    print(vga_buffer, "<3");
    return;
}

