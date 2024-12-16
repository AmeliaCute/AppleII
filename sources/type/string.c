#include "string.h"

void strncpy(char* destination, const char* source, size_t length)
{
    size_t i = 0;
    while (i < length && source[i] != '\0') 
    {
        destination[i] = source[i];
        ++i;
    }
    
    destination[i] = '\0';
}