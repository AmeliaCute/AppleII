#ifndef KERNEL_COMPONENT_MANAGER
#define KERNEL_COMPONENT_MANAGER

/**
 * INCLUDE FROM COMPONENTS
 */

#include "../vga/vga.h"
#include "../vga/write.h"
#include "../header/standard_type.h"

#define MAX_COMPONENTS 128

typedef struct 
{
    const char* name;        // Variable name
    void* data;              // Variable data
    unsigned int size;       // Variable size
} ComponentData;

typedef struct 
{
    const char* randomchar;
    uint32_t randomint32;
} TestComponent;

extern ComponentData kernel_components[MAX_COMPONENTS];
extern unsigned int offset;

void* k_component_get(unsigned int offset);
void k_init_component_manager();

#endif // !KERNEL_COMPONENT_MANAGER