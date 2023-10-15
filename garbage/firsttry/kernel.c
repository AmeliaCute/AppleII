void print(char* text, char color)
{
    char* video_mem = (char*)0xB8000;

    for(int i = 0; text[i] != '\0'; ++i)
    {
        video_mem[i * 2] = text[i];
        video_mem[i * 2 + 1] = color;
    }
}

void main()
{
    char hello[] = "Hello from Kernel.";
    print(hello, 0x04);
    while(1);
}