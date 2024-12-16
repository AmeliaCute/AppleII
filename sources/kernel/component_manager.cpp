#include "component_manager.h"

ComponentData kernel_components[MAX_COMPONENTS];
unsigned int offset = 0;

int k_create_component(const char* name, void* p_component, unsigned int size)
{
    if(offset >= MAX_COMPONENTS) return -1; // Too many components

    kernel_components[offset].name = name;
    kernel_components[offset].data = p_component;
    kernel_components[offset].size = size;

    ++offset;
    return 0;
}

void k_init_component_manager()
{
    static TestComponent test_component = { "dfsds", 32183 };
    k_create_component("test", &test_component, sizeof(TestComponent));
    
    static VgaComponent vga_component = { 0, 0x0F };
    k_create_component("vga", &vga_component, sizeof(VgaComponent));

}

void* k_component_get(unsigned int index)
{
    void* data = kernel_components[index].data;
    if(!data) return nullptr;

    return data;
}